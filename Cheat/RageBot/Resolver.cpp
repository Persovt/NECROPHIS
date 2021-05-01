#include "Resolver.h"
#include "Ragebot.h"
#include "Autowall.h"
#include "LagComp.h"

CResolver* Resolver = new CResolver;

#define FLOW_OUTGOING	0
#define FLOW_INCOMING	1
#define MAX_FLOWS		2	

void CResolver::AnimationFix(CBaseEntity* pEnt)
{
	
	static float oldSimtime[65];
	static float storedSimtime[65];
	static float ShotTime[65];
	static float SideTime[65][3];
	static int LastDesyncSide[65];
	static bool Delaying[65];
	static CAnimationLayer StoredLayers[64][15];
	static CCSGOAnimState* StoredAnimState[65];
	static float StoredPosParams[65][24];
	static Vector oldEyeAngles[65];
	static float oldGoalfeetYaw[65];
	float* PosParams = (float*)((uintptr_t)pEnt + 0x2774);
	bool update = false;
	bool shot = false;

	static bool jittering[65];

	auto * AnimState = pEnt->GetAnimState();
	if (!AnimState || !pEnt->GetAnimOverlay() || !PosParams)
		return;

	auto RemapVal = [](float val, float A, float B, float C, float D) -> float
	{
		if (A == B)
			return val >= B ? D : C;
		return C + (D - C) * (val - A) / (B - A);
	};

	if (storedSimtime[pEnt->EntIndex()] != pEnt->GetSimTime())
	{
		jittering[pEnt->EntIndex()] = false;
		pEnt->ClientAnimations(true);
		pEnt->UpdateClientAnimation();

		memcpy(StoredPosParams[pEnt->EntIndex()], PosParams, sizeof(float) * 24);
		memcpy(StoredLayers[pEnt->EntIndex()], pEnt->GetAnimOverlay(), (sizeof(CAnimationLayer) * 15));

		oldGoalfeetYaw[pEnt->EntIndex()] = AnimState->m_flGoalFeetYaw;
		
		if (pEnt->GetActiveWeapon() && !mLocal->m_pWeaponEntity->IsKnife())
		{
			

			if (ShotTime[pEnt->EntIndex()] != pEnt->GetLastShotTime())
			{
				shot = true;
				ShotTime[pEnt->EntIndex()] = pEnt->GetLastShotTime();
			}
			else
				shot = false;
		}
		else
		{
			shot = false;
			ShotTime[pEnt->EntIndex()] = 0.f;
		}

		float angToLocal = Math::NormalizeYaw(Math::CalcAngle(LocalPlayer->GetOrigin(), pEnt->GetOrigin()).y);

		float Back = Math::NormalizeYaw(angToLocal);
		float DesyncFix = 0;
		float Resim = Math::NormalizeYaw((0.24f / (pEnt->GetSimTime() - oldSimtime[pEnt->EntIndex()])) * (oldEyeAngles[pEnt->EntIndex()].y - pEnt->GetEyeAngles().y));

		if (Resim > 58.f)
			Resim = 58.f;
		if (Resim < -58.f)
			Resim = -58.f;

		if (pEnt->GetVelocity().Length2D() > 0.5f && !shot)
		{
			float Delta = Math::NormalizeYaw(Math::NormalizeYaw(Math::CalcAngle(Vector(0, 0, 0), pEnt->GetVelocity()).y) - Math::NormalizeYaw(Math::NormalizeYaw(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60)) + Resim));

			int CurrentSide = 0;

			if (Delta < 0)
			{
				CurrentSide = 1;
				SideTime[pEnt->EntIndex()][1] = Interfaces::GlobalVars()->curtime;
			}
			else if (Delta > 0)
			{
				CurrentSide = 2;
				SideTime[pEnt->EntIndex()][2] = Interfaces::GlobalVars()->curtime;
			}

			if (LastDesyncSide[pEnt->EntIndex()] == 1)
			{
				Resim += (58.f - Resim);
				DesyncFix += (58.f - Resim);
			}
			if (LastDesyncSide[pEnt->EntIndex()] == 2)
			{
				Resim += (-58.f - Resim);
				DesyncFix += (-58.f - Resim);
			}

			if (LastDesyncSide[pEnt->EntIndex()] != CurrentSide)
			{
				Delaying[pEnt->EntIndex()] = true;

				if (.5f < (Interfaces::GlobalVars()->curtime - SideTime[pEnt->EntIndex()][LastDesyncSide[pEnt->EntIndex()]]))
				{
					LastDesyncSide[pEnt->EntIndex()] = CurrentSide;
					Delaying[pEnt->EntIndex()] = false;
				}
			}

			if (!Delaying[pEnt->EntIndex()])
				LastDesyncSide[pEnt->EntIndex()] = CurrentSide;
		}
		else if (!shot)
		{
			float Brute = UseFreestandAngle[pEnt->EntIndex()] ? Math::NormalizeYaw(Back + UseFreestandAngle[pEnt->EntIndex()]) : pEnt->GetLowerBodyYaw();

			float Delta = Math::NormalizeYaw(Math::NormalizeYaw(Brute - Math::NormalizeYaw(Math::NormalizeYaw(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60))) + Resim));

			if (Delta > 58.f)
				Delta = 58.f;
			if (Delta < -58.f)
				Delta = -58.f;

			Resim += Delta;
			DesyncFix += Delta;

			if (Resim > 58.f)
				Resim = 58.f;
			if (Resim < -58.f)
				Resim = -58.f;
		}

		float Equalized = Math::NormalizeYaw(Math::NormalizeYaw(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60)) + Resim);

		float JitterDelta = fabs(Math::NormalizeYaw(oldEyeAngles[pEnt->EntIndex()].y - pEnt->GetEyeAngles().y));

		if (JitterDelta >= 70.f && !shot)
			jittering[pEnt->EntIndex()] = true;

		if (pEnt != LocalPlayer && pEnt->GetTeam() != LocalPlayer->GetTeam() && (pEnt->GetFlags() & FL_ONGROUND))
		{
			if (jittering[pEnt->EntIndex()])
				AnimState->m_flGoalFeetYaw = Math::NormalizeYaw(pEnt->GetEyeAngles().y + DesyncFix);
			else
				AnimState->m_flGoalFeetYaw = Equalized;

			pEnt->SetLowerBodyYaw(AnimState->m_flGoalFeetYaw);
		}

		StoredAnimState[pEnt->EntIndex()] = AnimState;

		oldEyeAngles[pEnt->EntIndex()] = pEnt->GetEyeAngles();

		oldSimtime[pEnt->EntIndex()] = storedSimtime[pEnt->EntIndex()];

		storedSimtime[pEnt->EntIndex()] = pEnt->GetSimTime();

		update = true;
	}

	pEnt->ClientAnimations(false);

	if (pEnt != LocalPlayer && pEnt->GetTeam() != LocalPlayer->GetTeam() && (pEnt->GetFlags() & FL_ONGROUND))
		pEnt->SetLowerBodyYaw(AnimState->m_flGoalFeetYaw);

	AnimState = StoredAnimState[pEnt->EntIndex()];

	memcpy((void*)PosParams, &StoredPosParams[pEnt->EntIndex()], (sizeof(float) * 24));
	memcpy(pEnt->GetAnimOverlay(), StoredLayers[pEnt->EntIndex()], (sizeof(CAnimationLayer) * 15));

	if (pEnt !=LocalPlayer && pEnt->GetTeam() != LocalPlayer->GetTeam() && (pEnt->GetFlags() & FL_ONGROUND) && jittering[pEnt->EntIndex()])
		pEnt->SetAbsAngles(Vector(0, pEnt->GetEyeAngles().y, 0));
	else
		pEnt->SetAbsAngles(Vector(0, oldGoalfeetYaw[pEnt->EntIndex()], 0));

	*reinterpret_cast<int*>(uintptr_t(pEnt) + 0xA30) = Interfaces::GlobalVars()->framecount;
	*reinterpret_cast<int*>(uintptr_t(pEnt) + 0xA28) = 0;

}


void HandleBackUpResolve(CBaseEntity * pEnt) {

	

	if (pEnt->GetTeam() == LocalPlayer->GetTeam())
		return;

	const auto player_animation_state = pEnt->GetAnimState();

	if (!player_animation_state)
		return;

	if (MissedShots[pEnt->EntIndex()] > 2) {
		switch (MissedShots[pEnt->EntIndex()] % 4) {
		case 0: player_animation_state->m_flGoalFeetYaw = player_animation_state->m_flGoalFeetYaw + 45; break;
		case 1: player_animation_state->m_flGoalFeetYaw = player_animation_state->m_flGoalFeetYaw - 45; break;
		case 2: player_animation_state->m_flGoalFeetYaw = player_animation_state->m_flGoalFeetYaw - 25; break;
		case 3: player_animation_state->m_flGoalFeetYaw = player_animation_state->m_flGoalFeetYaw + 25; break;
		case 4: player_animation_state->m_flGoalFeetYaw = player_animation_state->m_flGoalFeetYaw + 90; break;
		case 5: player_animation_state->m_flGoalFeetYaw = player_animation_state->m_flGoalFeetYaw - 90; break;
		}
	}
	else {

		float m_flLastClientSideAnimationUpdateTimeDelta = fabs(player_animation_state->m_iLastClientSideAnimationUpdateFramecount - player_animation_state->m_flLastClientSideAnimationUpdateTime);

		auto v48 = 0.f;

		if (player_animation_state->m_flFeetSpeedForwardsOrSideWays >= 0.0f)
		{
			v48 = fminf(player_animation_state->m_flFeetSpeedForwardsOrSideWays, 1.0f);
		}
		else
		{
			v48 = 0.0f;
		}

		float v49 = ((player_animation_state->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * v48;

		float flYawModifier = v49 + 1.0;

		if (player_animation_state->m_fDuckAmount > 0.0)
		{
			float v53 = 0.0f;

			if (player_animation_state->m_flFeetSpeedUnknownForwardOrSideways >= 0.0)
			{
				v53 = fminf(player_animation_state->m_flFeetSpeedUnknownForwardOrSideways, 1.0);
			}
			else
			{
				v53 = 0.0f;
			}
		}

		float flMaxYawModifier = player_animation_state->pad10[516] * flYawModifier;
		float flMinYawModifier = player_animation_state->pad10[512] * flYawModifier;

		float newFeetYaw = 0.f;

		float eyeYaw = player_animation_state->m_flEyeYaw;

		float lbyYaw = player_animation_state->m_flGoalFeetYaw;

		float eye_feet_delta = fabs(eyeYaw - lbyYaw);

		if (eye_feet_delta <= flMaxYawModifier)
		{
			if (flMinYawModifier > eye_feet_delta)
			{
				newFeetYaw = fabs(flMinYawModifier) + eyeYaw;
			}
		}
		else
		{
			newFeetYaw = eyeYaw - fabs(flMaxYawModifier);
		}

		float v136 = fmod(newFeetYaw, 360.0);

		if (v136 > 180.0)
		{
			v136 = v136 - 360.0;
		}

		if (v136 < 180.0)
		{
			v136 = v136 + 360.0;
		}

		player_animation_state->m_flGoalFeetYaw = v136;
	}
	
}

void HandleHits(CBaseEntity * pEnt)
{
	auto NetChannelInfo = Interfaces::Engine()->GetNetChannelInfo();

	if (!NetChannelInfo)
		return;

	static float predTime[65];
	static bool init[65];


	if (Shot[pEnt->EntIndex()])
	{
		if (init[pEnt->EntIndex()])
		{
			predTime[pEnt->EntIndex()] = Interfaces::GlobalVars()->curtime + NetChannelInfo->GetAvgLatency(FLOW_INCOMING) + NetChannelInfo->GetAvgLatency(FLOW_OUTGOING) + TICKS_TO_TIME(1) + TICKS_TO_TIME(Interfaces::Engine()->GetNetChannel()->m_nChokedPackets);
			init[pEnt->EntIndex()] = false;
		}

		if (Interfaces::GlobalVars()->curtime > predTime[pEnt->EntIndex()] && !Hit[pEnt->EntIndex()])
		{
			MissedShots[pEnt->EntIndex()] += 1;
			Shot[pEnt->EntIndex()] = false;
		}
		else if (Interfaces::GlobalVars()->curtime <= predTime[pEnt->EntIndex()] && Hit[pEnt->EntIndex()])
			Shot[pEnt->EntIndex()] = false;
	}
	else
		init[pEnt->EntIndex()] = true;
}

void CResolver::OnCreateMove() 
{
	
	if (LocalPlayer->IsDead())
		return;

	if (!LocalPlayer->GetActiveWeapon() || mLocal->m_pWeaponEntity->IsKnife())
		return;


	for (int i = 1; i < Interfaces::Engine()->GetMaxClients(); ++i)
	{
		CBaseEntity* pPlayerEntity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(i);

		if (!pPlayerEntity
			|| pPlayerEntity->IsDead()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == LocalPlayer
			|| pPlayerEntity->GetTeam() == LocalPlayer->GetTeam())
		{
			UseFreestandAngle[i] = false;
			continue;
		}

		if (abs(pPlayerEntity->GetVelocity().Length2D()) > 29.f)
			UseFreestandAngle[pPlayerEntity->EntIndex()] = false;

		if (abs(pPlayerEntity->GetVelocity().Length2D()) <= 29.f && !UseFreestandAngle[pPlayerEntity->EntIndex()])
		{
			bool Autowalled = false, HitSide1 = false, HitSide2 = false;

			if (Autowalled)
			{
				if (HitSide1 && HitSide2)
					UseFreestandAngle[pPlayerEntity->EntIndex()] = false;
				else
					UseFreestandAngle[pPlayerEntity->EntIndex()] = true;
			}
		}
	}
}

void CResolver::FrameStage(ClientFrameStage_t stage)
{
	

	if (!LocalPlayer)
		return;

	for (int i = 1; i < Interfaces::Engine()->GetMaxClients(); ++i)
	{
		CBaseEntity* pPlayerEntity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(i);

		if (!pPlayerEntity || pPlayerEntity->IsDead())
			continue;
		

		if (stage == ClientFrameStage_t::FRAME_RENDER_START)
		{
			HandleHits(pPlayerEntity);
			AnimationFix(pPlayerEntity);
		}

		if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) 
			HandleBackUpResolve(pPlayerEntity);
		

		if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_END && pPlayerEntity != LocalPlayer)
		{
			auto VarMap = reinterpret_cast<uintptr_t>(pPlayerEntity) + 36;
			auto VarMapSize = *reinterpret_cast<int*>(VarMap + 20);

			for (auto index = 0; index < VarMapSize; index++)
				* reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(VarMap) + index * 12) = 0;
		}
	}
}