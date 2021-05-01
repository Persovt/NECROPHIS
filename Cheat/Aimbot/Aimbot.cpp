#include "Aimbot.h"

//using namespace Client;

#pragma warning(disable:4244)
//[junk_enable /]

//[enc_string_enable /]
byte tt_ct_best_hit_1[14] =
{
	//All Spots
	HITBOX_HEAD ,
	HITBOX_NECK ,
	HITBOX_BODY ,
	HITBOX_THORAX ,
	HITBOX_CHEST ,
	HITBOX_RIGHT_THIGH ,
	HITBOX_LEFT_THIGH ,
	HITBOX_RIGHT_HAND ,
	HITBOX_LEFT_HAND ,
	HITBOX_RIGHT_UPPER_ARM ,
	HITBOX_RIGHT_FOREARM ,
	HITBOX_LEFT_UPPER_ARM ,
	HITBOX_LEFT_FOREARM
};

byte tt_ct_best_hit_2[13] =
{
	//No Headshot
	HITBOX_NECK ,
	HITBOX_BODY ,
	HITBOX_THORAX ,
	HITBOX_CHEST ,
	HITBOX_RIGHT_THIGH ,
	HITBOX_LEFT_THIGH ,
	HITBOX_RIGHT_HAND ,
	HITBOX_LEFT_HAND ,
	HITBOX_RIGHT_UPPER_ARM ,
	HITBOX_RIGHT_FOREARM ,
	HITBOX_LEFT_UPPER_ARM ,
	HITBOX_LEFT_FOREARM
};

byte tt_ct_best_hit_3[6] =
{
	//No Arms/Legs
	HITBOX_HEAD ,
	HITBOX_NECK ,
	HITBOX_BODY ,
	HITBOX_THORAX ,
	HITBOX_CHEST
};

byte tt_ct_best_hit_4[5] =
{
	//No Arms/Legs/Neck
	HITBOX_HEAD ,
	HITBOX_BODY ,
	HITBOX_THORAX ,
	HITBOX_CHEST
};

CAimbot::CAimbot()
{
	m_iBestPreTarget = -1;

	m_iBestTarget = -1;
	m_iBestHitbox = -1;

	m_bClamp = false;
	m_bAttack = false;
	m_bAimShot = false;
	m_bTargetFov = false;
	m_bTargetChange = false;
	m_bAutoPistolEn = false;

	m_pShotDelay = new CTimer();

	m_vAimBestHitboxScreen.Zero();
}



bool CAimbot::IsEnable()
{
	

		if (!settings.LegitBot.aim_Active)
			return false;

		if (settings.RageBot.aimbot_enabled)
			return false;

		if (!mLocal->WeaponAmmo || mLocal->bInReload)
			return false;


		if (mLocal->WeaponType > WEAPON_TYPE_SNIPER)
			return false;

		if (settings.LegitBot.onlyiszoom && !LocalPlayer->GetIsScoped() && (*mLocal->m_pWeaponEntity->GeteAttributableItem()->GetItemDefinitionIndex() == WEAPON_SCAR20 || *mLocal->m_pWeaponEntity->GeteAttributableItem()->GetItemDefinitionIndex() == WEAPON_AWP || *mLocal->m_pWeaponEntity->GeteAttributableItem()->GetItemDefinitionIndex() == WEAPON_G3SG1 || *mLocal->m_pWeaponEntity->GeteAttributableItem()->GetItemDefinitionIndex() == WEAPON_SSG08))
			return false;

		if (settings.LegitBot.aim_OnKey && !GetAsyncKeyState(settings.LegitBot.aim_Key))
			return false;
		

		if (settings.LegitBot.aim_CheckFlash && LocalPlayer->GetFlashDuration())
			return false;

		if (LineGoesThroughSmoke(mLocal->vEyeOrigin, m_vAimBestHitbox) && settings.LegitBot.aim_CheckSmoke)
			return false;
		

		return true;
	
}


int CAimbot::GetPlayerFov()
{
	int iFov = 0;
	int iFovVal = 0;

	
		if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_FovType == 0)
		{
			iFovVal = settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Fov;
		}
		else if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_FovType == 1)
		{
			if (iFovVal < settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Fov)
			{
				int fov = settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Fov / 5;
				iFovVal = (mLocal->iShotsFired * fov);
			}
		}

	int base_fov = pow(iFovVal + FIX_MIN_FOV_HEAD, 2) * 90;
	iFov = (int)(base_fov / (FOV_BASE_DISTANCE * mLocal->iFov));

	return iFov;
}

int CAimbot::GetBestTarget()
{
	float ScreenDistanceBase = 1000.f;

	int m_lBestTarget = -1;

	for (BYTE PlayerIndex = 0; PlayerIndex < Players->GetSize(); PlayerIndex++)
	{
		CPlayer* pPlayer = Players->GetPlayer(PlayerIndex);

		if (pPlayer && pPlayer->m_pEntity && pPlayer->bUpdate)
		{
			if (!settings.LegitBot.aim_Deathmatch && pPlayer->Team == mLocal->Team)
				continue;

			if (!pPlayer->bVisible)
				continue;

			Vector vHitbox;
			Vector vHitboxScreen;

			if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_BestHit == 1)				
				vHitbox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[GetBestHitBox()]);	

			else if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_BestHit == 0)
				vHitbox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Spot]);
			

			if (WorldToScreen(vHitbox, vHitboxScreen))
			{
				Vector2D vPlayerScr = Vector2D(vHitboxScreen.x, vHitboxScreen.y);

				float fDistanceScreen = Math::DistanceScreen(CenterScreens, vPlayerScr);

				if (fDistanceScreen < ScreenDistanceBase)
				{
					ScreenDistanceBase = fDistanceScreen;
					m_lBestTarget = PlayerIndex;
				}
			}
		}
	}

	return m_lBestTarget;
}

int CAimbot::GetBestHitBox()
{
	float ScreenDistanceBase = 1000.f;

	int m_lBestHitbox = -1;

	if (mLocal->WeaponType == WEAPON_TYPE_SHOTGUN)
	{
		if (m_iBestHitbox && m_bAttack && !m_bTargetChange)
			m_lBestHitbox = m_iBestHitbox;
	}

	CPlayer* pPlayer = Players->GetPlayer(m_iBestTarget);

	if (pPlayer && pPlayer->m_pEntity && pPlayer->bUpdate)
	{
		if (m_lBestHitbox == -1)
		{
			if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_BestHit == 1)
			{
				
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_1; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = Math::DistanceScreen(CenterScreens, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_1[bHitbox];
						}
					}
				
			}
			else if(settings.LegitBot.weapon_aim_settings[iWeaponID].aim_BestHit == 0)	    		
				m_lBestHitbox = tt_ct_best_hit_1[settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Spot];			
			
		}

		Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(m_lBestHitbox);

		

		if (vHitBox.IsValid() && WorldToScreen(vHitBox, m_vAimBestHitboxScreen))
		{
			m_vAimBestHitbox = vHitBox;
			return m_lBestHitbox;
		}
		else
		{
			m_vAimBestHitbox.Zero();
			m_vAimBestHitboxScreen.Zero();
			return -1;
		}
	}

	return m_lBestHitbox;
}

void CAimbot::OnRender()
{
		if (m_vAimBestHitboxScreen.x <= 0.f || m_vAimBestHitboxScreen.y <= 0.f)
			return;



		CPlayer* pTargetPlayer = Players->GetPlayer(m_iBestTarget);

		if (!pTargetPlayer)
			return;

		
		if (settings.LegitBot.aim_DrawFov)
			Render->DrawCircle(Vector2D(CenterScreens.x, CenterScreens.y), 150, GetPlayerFov(), Color::White());
	
}

void CAimbot::Aimbot(CUserCmd* cmd, CMe* pLocal, bool &bSendPacket)
{
	m_bAimShot = false;
	m_bTargetFov = false;


	m_bAttack = (cmd->buttons & IN_ATTACK);

	if (!Players->GetPlayer(m_iBestTarget)->bUpdate)
	{
		m_iBestPreTarget = -1;
		m_iBestTarget = -1;
		m_iBestHitbox = -1;
	}

	m_iBestPreTarget = m_iBestTarget;
	m_iBestTarget = GetBestTarget();

	
	if (settings.LegitBot.aim_AutoPistol && mLocal->WeaponType == WEAPON_TYPE_PISTOL && !m_bAutoPistolEn && m_bAttack)
		AutoPistol(cmd);

	if (m_iBestTarget == -1)
		return;

	if (m_iBestPreTarget != m_iBestTarget && m_bAttack)
		m_bTargetChange = true;
	else
		m_bTargetChange = false;
	

	m_iBestHitbox = GetBestHitBox();

	if (m_iBestHitbox == -1)
		return;

	CPlayer* pPreTargetPlayer = Players->GetPlayer(m_iBestPreTarget);
	CPlayer* pTargetPlayer = Players->GetPlayer(m_iBestTarget);

	int iPlayerFov = GetPlayerFov();

	if (CheckPlayerFov(m_vAimBestHitboxScreen, iPlayerFov))
		m_bTargetFov = true;

	static bool bLockDelay = false;
	static bool bLockAttack = false;

	if (!m_bAttack)
	{
		m_bClamp = true;
		bLockDelay = false;
		bLockAttack = false;
		m_pShotDelay->reset();
	}

	if (settings.LegitBot.aim_AutoPistol && mLocal->WeaponType == WEAPON_TYPE_PISTOL)
	{
		if (m_bTargetFov && !m_bAttack)
			m_bAutoPistolEn = true;
	
		else if (!m_bTargetFov )		
			m_bAutoPistolEn = false;

	}


		if (mLocal->iShotsFired == 1 && bLockDelay && !settings.LegitBot.aim_AutoPistol && mLocal->WeaponType == WEAPON_TYPE_PISTOL)
			return;
	

	AIM_ATTACK_TYPE AttackType = AA_SHOTGUN;

	if (mLocal->WeaponType == WEAPON_TYPE_SNIPER || mLocal->WeaponType == WEAPON_TYPE_PISTOL)
		AttackType = AA_SNIPER_PISTOL;

	static bool bAimAttack = false;
	static bool bAimCheckAttack = false;

	if (AttackType == AA_SHOTGUN)
	{
		if (m_bAttack && m_bTargetFov)
			bAimCheckAttack = true;
		
		else
			bAimCheckAttack = false;
		
	}
	else if (AttackType == AA_SNIPER_PISTOL)
	{
		if (!settings.LegitBot.aim_AutoPistol)
		{
			if (m_bTargetFov && !m_bAttack)
				bAimAttack = true;
			
			else if (!m_bTargetFov )		
				bAimAttack = false;


			if (m_bAttack && bAimAttack && m_bTargetFov)
				bAimCheckAttack = true;
			
			else
				bAimCheckAttack = false;
			
		}
		else
		{
			if (m_bAttack && m_bTargetFov)			
				bAimCheckAttack = true;	
			else
			{
				bAimCheckAttack = false;
				m_bClamp = true;
			}
		}
	}

	if (mLocal->WeaponType == WEAPON_TYPE_SNIPER || mLocal->WeaponType == WEAPON_TYPE_PISTOL)
		settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RcsClampType = 2;

	if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RcsClampType >= 1 && mLocal->WeaponType == WEAPON_TYPE_SHOTGUN)
	{
		if (!bLockAttack)
		{
			if (m_bTargetFov && !m_bAttack)	
				bAimAttack = true;
			
			else if (!m_bTargetFov) 	
				bAimAttack = false;
		}

		if (m_bAttack && bAimAttack && m_bTargetFov && !bLockAttack)
		{
			bAimCheckAttack = true;
			bLockAttack = true;
		}
		else if (m_bAttack && m_bTargetFov && bLockAttack)
		{
			bAimCheckAttack = true;
			bLockAttack = true;
		}
		else
			bAimCheckAttack = false;
	}

	if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RcsClampType >= 2)
	{
		if (m_bClamp && m_bTargetChange)
			m_bClamp = false;
	}

	if (!(pTargetPlayer->m_pEntity->GetFlags() & FL_ONGROUND) && settings.LegitBot.aim_AntiJump)
		return;

	if (bAimCheckAttack)
	{
		int AimDelay = settings.LegitBot.aim_Delay;

		if (AimDelay)
		{
			float fDistanceAimScreen = Math::DistanceScreen(CenterScreens, Vector2D(m_vAimBestHitboxScreen.x, m_vAimBestHitboxScreen.y));


			if (fDistanceAimScreen <= 3.5f && !bLockDelay || m_pShotDelay->delay(AimDelay))
				m_bAimShot = true;
				
				bLockDelay = true;
		}
		else
			m_bAimShot = true;
		


		if (m_bClamp)
		{
			AimbotSet(cmd, pLocal, bSendPacket);
			
		}

		if (mLocal->WeaponType == WEAPON_TYPE_SHOTGUN || !settings.LegitBot.aim_AutoPistol)
		{
			if (m_bAimShot)		
				cmd->buttons |= IN_ATTACK;		
			else			
				cmd->buttons &= ~IN_ATTACK;
			
		}

		if (mLocal->WeaponType == WEAPON_TYPE_PISTOL && settings.LegitBot.aim_AutoPistol && m_bAutoPistolEn)
		{
			if (m_bAimShot)
			{
				cmd->buttons |= IN_ATTACK;
				m_bAutoPistolEn = false;
			}
			else
				cmd->buttons &= ~IN_ATTACK;
		}
	}
}




void CAimbot::AimbotSet(CUserCmd * cmd, CMe * pLocal, bool &bSendPacket)
{
	if (!m_vAimBestHitbox.IsValid())
		return;

	Vector vAimAngle, vSmoothAimAngle;

	Math::VectorAngles2(m_vAimBestHitbox - mLocal->vEyeOrigin, vAimAngle);

	bool bEnableRcs = false;
	
	

	
		if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RCSType == 0)
		{
			float MulVar = settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Rcs * 0.02f;
			if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RcsTypes == 1)
			{
				vAimAngle -= mLocal->vAimPunch * MulVar;
			}
			else if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RcsTypes == 2)
			{
				vAimAngle -= (mLocal->vPunch + mLocal->vAimPunch) * MulVar;
			}
			else if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RcsTypes == 3)
			{
				vAimAngle -= (mLocal->vPunch + (mLocal->vAimPunch * 2 * 0.5f)) * MulVar;
			}
		}
		else if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RCSType == 1)
		{
			if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RcsTypes2 == 1)
			{
				vAimAngle -= mLocal->vAimPunch * Vector(settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Rcsx * 0.0221521f, settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Rcsy * 0.0221521f, 1);
			}
			else if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RcsTypes2 == 2)
			{
				vAimAngle -= (mLocal->vPunch + mLocal->vAimPunch) * Vector(settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Rcsx * 0.0221521f, settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Rcsy * 0.0221521f, 1);
			}
			else if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RcsTypes2 == 3)
			{
				vAimAngle -= (mLocal->vPunch + (mLocal->vAimPunch * 2 * 0.5f)) * Vector(settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Rcsx * 0.0221521f, settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Rcsy * 0.0221521f, 1);
			}
		}

	

	float fSmooth = settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Smooth;
	
	if(settings.LegitBot.Humanize)
		fSmooth += Math::RandomFloat(-1, 4);

	Math::NormalizeAngle(vAimAngle);


	Math::SmoothAngles(cmd->viewangles, vAimAngle, vSmoothAimAngle, fSmooth);

	cmd->viewangles += vSmoothAimAngle;
	
}

void CAimbot::AutoPistol(CUserCmd* cmd)
{
	float NextAttack = mLocal->m_pWeaponEntity->GetNextPrimaryAttack();
	float Tick = mLocal->m_pEntity->GetTickBase() * Interfaces::GlobalVars()->interval_per_tick;

	if (NextAttack < Tick)
		return;

	if (*mLocal->m_pWeaponEntity->GeteAttributableItem()->GetItemDefinitionIndex() == WEAPON_REVOLVER)
	{
		cmd->buttons &= ~IN_ATTACK2;
	}
	else
	{
		cmd->buttons &= ~IN_ATTACK;
	}
}



void CAimbot::OnCreateMove(CUserCmd * cmd)
{


	if (!IsEnable() || !mLocal)
		return;

	Aimbot(cmd, mLocal, bSendPacket);
	//Interfaces::Engine()->SetViewAngles(cmd->viewangles);
	
	
}

CAimbot*	Aimbot = new CAimbot;
