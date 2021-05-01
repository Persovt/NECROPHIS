#include "Ragebot.h"
#include "Autowall.h"
#include "LagComp.h"

CRagebot* Ragebot = new CRagebot;

float CRagebot::SynchronizeHitchance() 
{
	if (!LocalPlayer->GetActiveWeapon())
		return 0;

	if (LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_SCAR20 || LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_G3SG1)
		return settings.RageBot.auto_hitchance;
	
	else if (LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_SSG08)
		return settings.RageBot.scout_hitchance;
	
	else if (LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_AWP)
		return settings.RageBot.awp_hitchance;
	
	else if (LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_REVOLVER || LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_DEAGLE)
		return settings.RageBot.heavy_pistol_hitchance;

	else 
		return settings.RageBot.other_hitchance;
}

float CRagebot::SynchronizeMinDamage() 
{
	if (!LocalPlayer->GetActiveWeapon())
		return 0;

	if (LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_SCAR20 || LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_G3SG1)
		return settings.RageBot.auto_mindamage;

	else if (LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_SSG08)
		return settings.RageBot.scout_mindamage;

	else if (LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_AWP)
		return settings.RageBot.awp_mindamage;

	else if (LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_REVOLVER || LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_DEAGLE)
		return settings.RageBot.heavy_pistol_mindamage;	

	else 
		return settings.RageBot.other_mindamage;
}

void CRagebot::Autostop(CUserCmd* cmd)
{
	if (settings.RageBot.autostop_mode == 0)
		return;

	Vector Velocity = LocalPlayer->GetVelocity();

	if (Velocity.Length2D() == 0)
		return;

	static float Speed = 450.f;

	Vector Direction;
	Vector RealView;
	Math::VectorAngles2(Velocity, Direction);
	Interfaces::Engine()->GetViewAngles(RealView);
	Direction.y = RealView.y - Direction.y;

	Vector Forward;
	Math::AngleVectors(Direction , &Forward,0,0);
	Vector NegativeDirection = Forward * -Speed;

	cmd->forwardmove = NegativeDirection.x;
	cmd->sidemove = NegativeDirection.y;
}
float accepted_inaccuracy(CBaseWeapon* weapon)
{
	
	if (!LocalPlayer) return 0;

	if (!weapon) return 0;
	if (LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_TASER) return 0;

	float inaccuracy = weapon->GetInaccuracy();
	if (inaccuracy == 0) inaccuracy = 0.0000001;
	inaccuracy = 1 / inaccuracy;
	return inaccuracy;
}

bool CRagebot::HitChance(CBaseEntity* pEnt, CBaseWeapon* pWeapon, Vector Angle, Vector Point,  int chance)
{
	if (chance == 0 || SynchronizeHitchance() == 0)
		return true;

	float Seeds = 256.f;

	Vector forward, right, up;

	Math::AngleVectors(Angle, &forward, &right, &up);

	int Hits = 0, neededHits = (Seeds * (chance / 100.f));

	float weapSpread = pWeapon->GetSpread(), weapInaccuracy = pWeapon->GetInaccuracy();

	for (int i = 0; i < Seeds; i++)
	{
		float Inaccuracy = Math::RandomFloat(0.f, 1.f) * weapInaccuracy;
		float Spread = Math::RandomFloat(0.f, 1.f) * weapSpread;

		Vector spreadView((cos(Math::RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (cos(Math::RandomFloat(0.f, 2.f * M_PI)) * Spread), (sin(Math::RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (sin(Math::RandomFloat(0.f, 2.f * M_PI)) * Spread), 0), direction;
		direction = Vector(forward.x + (spreadView.x * right.x) + (spreadView.y * up.x), forward.y + (spreadView.x * right.y) + (spreadView.y * up.y), forward.z + (spreadView.x * right.z) + (spreadView.y * up.z)).Normalized();

		Vector viewanglesSpread, viewForward;

		Math::VectorAngles(direction, up, viewanglesSpread);
		Math::NormalizeAngle(viewanglesSpread);

		Math::AngleVectors2(viewanglesSpread, &viewForward);
		viewForward.NormalizeInPlace();

		viewForward = LocalPlayer->GetEyePosition() + (viewForward * pWeapon->GetCSWpnData()->range);

		trace_t Trace;
		
		Interfaces::EngineTrace()->ClipRayToEntity(Ray_t(LocalPlayer->GetEyePosition(), viewForward), mask_shot | contents_grate, pEnt, &Trace);

		//if (Trace.m_pEnt == pEnt)
			Hits++;

		if (((Hits / Seeds) * 100.f) >= chance)
			return true;

		if ((Seeds - i + Hits) < neededHits)
			return false;
	}
	

	return false;
}

bool ShouldBaim(CBaseEntity* pEnt) // probably dosnt make sense
{
	static float oldSimtime[65];
	static float storedSimtime[65];

	static float ShotTime[65];
	static float NextShotTime[65];
	static bool BaimShot[65];

	if (storedSimtime[pEnt->EntIndex()] != pEnt->GetSimTime())
	{
		oldSimtime[pEnt->EntIndex()] = storedSimtime[pEnt->EntIndex()];
		storedSimtime[pEnt->EntIndex()] = pEnt->GetSimTime();
	}

	float simDelta = storedSimtime[pEnt->EntIndex()] - oldSimtime[pEnt->EntIndex()];

	bool Shot = false;

	if (pEnt->GetActiveWeapon() && !mLocal->m_pWeaponEntity->IsKnife())
	{
		if (ShotTime[pEnt->EntIndex()] != pEnt->GetLastShotTime())
		{
			Shot = true;
			BaimShot[pEnt->EntIndex()] = false;
			ShotTime[pEnt->EntIndex()] = pEnt->GetLastShotTime();
		}
		else
			Shot = false;
	}
	else
	{
		Shot = false;
		ShotTime[pEnt->EntIndex()] = 0.f;
	}

	if (Shot)
	{
		NextShotTime[pEnt->EntIndex()] = pEnt->GetSimTime() + pEnt->FireRate();

		if (simDelta >= pEnt->FireRate())
			BaimShot[pEnt->EntIndex()] = true;
	}

	if (BaimShot[pEnt->EntIndex()])
	{
		if (pEnt->GetSimTime() >= NextShotTime[pEnt->EntIndex()])
			BaimShot[pEnt->EntIndex()] = false;
	}



	std::vector<int> baim_hitboxes;


	baim_hitboxes.push_back(UpperChest);
	baim_hitboxes.push_back(Chest);
	baim_hitboxes.push_back(Pelvis);
	baim_hitboxes.push_back(Stomach);

	float Radius = 0;//
	Radius = (settings.RageBot.aimbot_bodypointscale / 100.f); //

	float Damage = 0;
	for (auto HitBoxID : baim_hitboxes)
	{
		Damage = Autowall->GetDamage(pEnt->GetHitboxPosition(HitBoxID, Ragebot->Matrix[pEnt->EntIndex()], &Radius));
	}

	if (LocalPlayer->GetVelocity().Length2D() > 125 && settings.RageBot.prefer_bodyaim[2])
		return true;

	if (BaimShot[pEnt->EntIndex()] && settings.RageBot.prefer_bodyaim[3])
		return true;

	if (!(pEnt->GetFlags() & FL_ONGROUND) && settings.RageBot.prefer_bodyaim[0])
		return true;

	if (settings.RageBot.prefer_bodyaim[1] && (pEnt->GetVelocity().Length2D() > 15 && pEnt->GetVelocity().Length2D() < 150))
		return true;

	if (pEnt->GetHealth() <= Damage && settings.RageBot.prefer_bodyaim[4])
		return true;

	if (pEnt->GetHealth() <= settings.RageBot.bodyaim_health)
		return true;

	if (MissedShots[pEnt->EntIndex()] > settings.RageBot.bodyaim_shots && settings.RageBot.bodyaim_shots > 0)
		return true;


	return false;
}

bool CRagebot::shouldIgnoreLimbs(CBaseEntity* pEnt) {
	if (settings.RageBot.aimbot_ignorelimbs && abs(pEnt->GetVelocity().Length2D()) > 100)
		return true;
	else
		return false;
}

Vector CRagebot::Hitscan(CBaseEntity* pEnt)
{
	
	float DamageArray[28];
	float tempDmg = 0.f;
	Vector tempHitbox = { 0,0,0 };
	static int HitboxForMuti[] = { 2,2,4,4,6,6 };
	float angToLocal = Math::CalcAngle(LocalPlayer->GetOrigin(), pEnt->GetOrigin()).y;

	Vector2D MutipointXY = { (sin(Math::GRD_TO_BOG(angToLocal))),(cos(Math::GRD_TO_BOG(angToLocal))) };
	Vector2D MutipointXY180 = { (sin(Math::GRD_TO_BOG(angToLocal + 180))) ,(cos(Math::GRD_TO_BOG(angToLocal + 180))) };
	Vector2D Mutipoint[] = { Vector2D(MutipointXY.x, MutipointXY.y), Vector2D(MutipointXY180.x, MutipointXY180.y) };

	float Velocity = abs(pEnt->GetVelocity().Length2D());

	if (Velocity > 29.f)
		Velocity = 30.f;

	std::vector<int> Scan;

	

	bool Baim = ShouldBaim(pEnt);

	bool PreferBaimIfLowVelcoity = (settings.RageBot.prefer_bodyaim_if_low_velocity && (Velocity > 15 && Velocity < 150));

	if (!Baim)
	{
		if (settings.RageBot.rage_hitscan_hitboxes[0])
			Scan.push_back(Head);
	}

	

	if (settings.RageBot.rage_hitscan_hitboxes[1])
	{
		Scan.push_back(Neck);
	}

	if (settings.RageBot.rage_hitscan_hitboxes[2])
	{
		
		Scan.push_back(Chest);

		Scan.push_back(LowerChest);
		Scan.push_back(UpperChest);
	}

	if (settings.RageBot.rage_hitscan_hitboxes[3])
	{
			Scan.push_back(Stomach);
			Scan.push_back(Pelvis);
	}

	if (settings.RageBot.rage_hitscan_hitboxes[4])
	{
		Scan.push_back(LeftUpperArm);
		Scan.push_back(LeftLowerArm);
	}

	if (settings.RageBot.rage_hitscan_hitboxes[5])
	{
		Scan.push_back(RightUpperArm);
		Scan.push_back(RightLowerArm);
	}

	if (settings.RageBot.rage_hitscan_hitboxes[6])
	{
		Scan.push_back(LeftShin);
		Scan.push_back(LeftThigh);
		Scan.push_back(LeftFoot);
	}

	if (settings.RageBot.rage_hitscan_hitboxes[7])
	{
		Scan.push_back(RightShin);
		Scan.push_back(RightThigh);
		Scan.push_back(RightFoot);
	}

	

	if (Baim && settings.RageBot.bodyaim_mode == 0) {
		if (settings.RageBot.rage_hitscan_hitboxes[0])
			Scan.push_back(Head);
	}


	Vector Hitbox;


	for (int hitbox : Scan)
	{
		
		if (hitbox < 19)
			Hitbox = pEnt->GetHitboxPosition(hitbox, Matrix[pEnt->EntIndex()]);


		else if (hitbox > 18 && hitbox < 25)
		{
			float Radius = 0;
			Hitbox = pEnt->GetHitboxPosition(HitboxForMuti[hitbox - 19], Matrix[pEnt->EntIndex()], &Radius);
			Radius *= (settings.RageBot.aimbot_bodypointscale / 100.f);
			Hitbox = Vector(Hitbox.x + (Radius * Mutipoint[((hitbox - 19) % 2)].x), Hitbox.y - (Radius * Mutipoint[((hitbox - 19) % 2)].y), Hitbox.z);
		}
		else if (hitbox > 24 && hitbox < 28)
		{
			float Radius = 0;
			Hitbox = pEnt->GetHitboxPosition(0, Matrix[pEnt->EntIndex()], &Radius);
			Radius *= (settings.RageBot.aimbot_headpointscale / 100.f);
			if (hitbox != 27)
				Hitbox = Vector(Hitbox.x + (Radius * Mutipoint[((hitbox - 25) % 2)].x), Hitbox.y - (Radius * Mutipoint[((hitbox - 25) % 2)].y), Hitbox.z);
			else
				Hitbox += Vector(0, 0, Radius);
		}

		

		
		float Damage = Autowall->GetDamage(Hitbox);

	
		//if (Baim && hitbox != 0 && hitbox != 25 && hitbox != 26 && hitbox != 27 && Damage >= (pEnt->GetHealth() + 10))
		//{
		//	DamageArray[hitbox] = 400;
		//	Baim = true;
		//}

		if (Damage > SynchronizeMinDamage())							
			 return Hitbox;

		else return Vector(0, 0, 0);

		AimbotHitbox[pEnt->EntIndex()][hitbox] = Hitbox;
	}

	PlayerRecords pPlayerEntityRecord = LagComp.PlayerRecord[pEnt->EntIndex()].at(0);

	
}


void CRagebot::OnCreateMove(CUserCmd* cmd)
{
	if (!LocalPlayer) return;
	
	Vector Aimpoint = { 0,0,0 };
	CBaseEntity* Target = nullptr;
	targetID = 0;

	for (int EntIndex = 0; EntIndex <= Interfaces::GlobalVars()->maxClients; ++EntIndex)
	{
		CBaseEntity* pPlayerEntity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(EntIndex);

		if (!pPlayerEntity || pPlayerEntity->IsDead() || pPlayerEntity->IsDormant())
		{
			LagComp.ClearRecords(EntIndex);
			continue;
		}

		LagComp.StoreRecord(pPlayerEntity, cmd);

		if (pPlayerEntity == LocalPlayer || pPlayerEntity->GetTeam() == LocalPlayer->GetTeam())
			continue;

		EnemyEyeAngs[EntIndex] = pPlayerEntity->GetEyeAngles();

		if (LagComp.PlayerRecord[EntIndex].size() == 0 || LocalPlayer->IsDead() || !settings.RageBot.aimbot_enabled)
			continue;

		Vector Hitbox = Hitscan(pPlayerEntity);

		if (Hitbox == Vector(0, 0, 0)) continue;
		Target = pPlayerEntity;
		targetID = EntIndex;
		Aimpoint = Hitbox;
	}
	if (Target)
	{
		
		if (LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_C4 || mLocal->m_pWeaponEntity->IsGrenade() || mLocal->m_pWeaponEntity->IsKnife()) return;

		float SimulationTime = 0.f;

		SimulationTime = LagComp.PlayerRecord[targetID].at(LagComp.PlayerRecord[targetID].size() - 1).SimTimes;

		if (mLocal->m_pWeaponEntity->IsKnife() || !LocalPlayer->GetActiveWeapon() || LocalPlayer->IsDead())
			return;


		static int MinimumVelocity = 0;

		if (settings.RageBot.autostop_mode == 1)
			MinimumVelocity = mLocal->m_pWeaponEntity->GetCSWpnData()->max_speed_alt * .34f;

		else if (settings.RageBot.autostop_mode == 2)
			MinimumVelocity = 0;

		bool shouldstop = settings.RageBot.stop_inbetween_shots ? true : mLocal->m_pWeaponEntity->CanShot();

		if (LocalPlayer->GetVelocity().Length() >= MinimumVelocity && shouldstop && (LocalPlayer->GetFlags() & FL_ONGROUND) && !(cmd->buttons & IN_RELOAD))
			Autostop(cmd);


		if ((LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_SCAR20 || LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_AWP || LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_G3SG1 || LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_SSG08 || LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_SG553 || LocalPlayer->GetBaseWeapon()->GetItemDefinitionIndex() == WEAPON_AUG))
			if (!LocalPlayer->GetIsScoped() && settings.RageBot.auto_scope)
				cmd->buttons |= IN_ATTACK2;


		Vector Angle = Math::CalcAngle(LocalPlayer->GetEyePosition(), Aimpoint);
		Angle -= (LocalPlayer->GetAimPunchAngle() * Interfaces::GetConVar()->FindVar("weapon_recoil_scale")->GetFloat());
		auto weapon = reinterpret_cast<CBaseWeapon*>(Interfaces::EntityList()->GetClientEntity(LocalPlayer->GetActiveWeaponIndex()));
		if (mLocal->m_pWeaponEntity->CanShot() && (accepted_inaccuracy(mLocal->m_pWeaponEntity) > SynchronizeHitchance())/*HitChance(pPlayerEntity, mLocal->m_pWeaponEntity, Angle, Hitbox, SynchronizeHitchance()) */ && mLocal->m_pWeaponEntity->GetWeaponAmmo() > 0)
		{
			if (!Backtrack[targetID] && !ShotBacktrack[targetID])
				Shot[targetID] = true;

			if (settings.RageBot.choke_shot)
				bSendPacket = false;
			else
				bSendPacket = true;

			Math::Clamp(Angle);

			cmd->viewangles = Angle;


			cmd->buttons |= IN_ATTACK;

			cmd->tick_count = TIME_TO_TICKS(SimulationTime + LagComp.LerpTime());

			if (settings.RageBot.choke_shot)
				bSendPacket = true;

		}
	}
}