#include "Offset.h"

namespace Engine
{
	
	namespace Offset
	{
		namespace Entity
		{
			
			DWORD m_hMyWeapons = 0;
			DWORD m_hMyWearables = 0;
			DWORD m_hViewModel = 0;
			DWORD m_nTickBase = 0;
			DWORD m_iObserverMode = 0;
			DWORD m_hObserverTarget = 0;

			DWORD m_vecOrigin = 0;
			DWORD deadflag = 0;

			DWORD m_lifeState = 0;
			DWORD m_flLowerBodyYawTarget = 0;
			DWORD m_nHitboxSet = 0;
			DWORD m_bHasHelmet = 0;
			DWORD m_bHasDefuser = 0;
			DWORD m_bIsScoped = 0;

			DWORD m_iFOVStart = 0;
			DWORD m_fFlags = 0;

			DWORD m_iHealth = 0;
			DWORD m_ArmorValue = 0;
			DWORD m_iTeamNum = 0;
			DWORD m_iShotsFired = 0;
			DWORD m_flSimulationTime = 0;
			DWORD m_flFlashDuration = 0;

			DWORD m_aimPunchAngle = 0;
			DWORD m_viewPunchAngle = 0;
			DWORD m_dwBoneMatrix = 0;
			DWORD m_vecVelocity = 0;
			DWORD m_vecViewOffset = 0;
			DWORD m_iWeaponID = 0;
			DWORD m_angEyeAngles = 0;
			DWORD animlayer = 0;
			DWORD m_hActiveWeapon = 0;
			DWORD m_flNextAttack = 0;
			DWORD m_iClip1 = 0;
			DWORD m_flNextPrimaryAttack = 0;
			DWORD m_bCanReload = 0;
			DWORD m_fLastShotTime = 0;
			int16 m_iItemDefinitionIndex = 0;
			DWORD m_iItemIDHigh = 0;
			DWORD m_iAccountID = 0;
			DWORD m_iEntityQuality = 0;
			DWORD m_OriginalOwnerXuidLow = 0;
			DWORD m_OriginalOwnerXuidHigh = 0;
			DWORD m_nFallbackPaintKit = 0;
			DWORD m_flFallbackWear = 0;
			DWORD m_nFallbackSeed = 0;
			DWORD m_nFallbackStatTrak = 0;
			DWORD m_flFlashBangTime = 0;
			DWORD m_nModelIndex = 0;
			DWORD m_hOwner = 0;
			DWORD animstate = 0;
			DWORD m_hWeapon = 0;
			DWORD m_bSpotted = 0;
			DWORD m_bIsDefusing = 0;
			DWORD m_bClientSideAnimation = 0;
			DWORD m_zoomLevel = 0;
			DWORD m_hGroundEntity = 0;
			DWORD m_nWaterLevel = 0;
			DWORD movetype = 0;
			
		}

		bool Initialize()
		{
			
			Entity::m_dwBoneMatrix = 0x26A8;
			Entity::animstate = 0x3900;
			Entity::m_hMyWeapons = g_NetVar.GetOffset(BASE_PLAYER, BASE_PLAYER_WEAPONS) / 2;
			Entity::m_hMyWearables = g_NetVar.GetOffset(BASE_PLAYER, BASE_PLAYER_WERABLES);
			Entity::m_hViewModel = g_NetVar.GetOffset(BASE_PLAYER, BASE_PLAYER_VIEWMODEL);
			Entity::m_nTickBase = g_NetVar.GetOffset(BASE_PLAYER, BASE_PLAYER_TICKBASE);
			Entity::m_iObserverMode = g_NetVar.GetOffset(BASE_PLAYER, BASE_PLAYER_OBSMODE);
			Entity::m_hObserverTarget = g_NetVar.GetOffset(BASE_PLAYER, BASE_PLAYER_OBSERVER);
			//Entity::m_vecOrigin = g_NetVar.GetOffset(BASE_ENTITY, CS_PLAYER_ORIGIN);
			
			Entity::m_vecOrigin = g_NetVar.GetOffset(BASE_PLAYER, CS_PLAYER_ORIGIN);
			Entity::deadflag = g_NetVar.GetOffset(BASE_PLAYER, CS_PLAYER_DEADFLAG);

			Entity::m_lifeState = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_LIFESTATE);
			Entity::m_flLowerBodyYawTarget = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_LBYTARGET);


			Entity::m_fLastShotTime = g_NetVar.GetOffset("DT_WeaponCSBase", "m_fLastShotTime");

			Entity::m_bHasHelmet = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_HASHELMET);
			Entity::m_bHasDefuser = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_HASHDEFUSER);
			Entity::m_bIsScoped = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_ISSCOPED);
			Entity::m_bIsDefusing = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_ISDEFUSING);
			Entity::m_flFlashDuration = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_ISFLASHED);

			Entity::m_iFOVStart = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_FOVSTART);
			Entity::m_fFlags = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_FLAGS);

			Entity::m_iHealth = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_HEALTH);
			Entity::m_ArmorValue = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_ARMOR);
			Entity::m_iTeamNum = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_TEAMNUM);
			Entity::m_iShotsFired = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_SHOTFIRED);

			Entity::m_aimPunchAngle = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_AIMPUNCHANG);
			Entity::m_viewPunchAngle = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_VIEWPUNCHANG);

			Entity::m_vecVelocity = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_VECVELOCITY);
			Entity::m_vecViewOffset = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_VECVIEW);
			Entity::m_angEyeAngles = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_EYEANGLES);

			Entity::m_hActiveWeapon = g_NetVar.GetOffset(CS_PLAYER, CS_PLAYER_ACTIVEWEAPON);
			Entity::m_bClientSideAnimation = g_NetVar.GetOffset(BASE_ANIMATING, SIDE_ANIMATING);
			Entity::m_iClip1 = g_NetVar.GetOffset(BASE_COMBAT_WEAPON, BASE_COMBAT_WEAPON_CLIP);
			Entity::m_flNextPrimaryAttack = g_NetVar.GetOffset(BASE_COMBAT_WEAPON, BASE_COMBAT_WEAPON_PRIM);
			Entity::m_bCanReload = Entity::m_flNextPrimaryAttack + 0x6D;
			Entity::m_flNextAttack = g_NetVar.GetOffset(("DT_BaseCombatCharacter"), ("m_flNextAttack"));
			Entity::m_iItemDefinitionIndex = g_NetVar.GetOffset(BASE_ATTRIBUTABLE_ITEM, BASE_ATTRIBUTABLE_ITEM_INDEX);
			Entity::m_iItemIDHigh = g_NetVar.GetOffset(BASE_ATTRIBUTABLE_ITEM, BASE_ATTRIBUTABLE_ITEM_ID_HIGH);
			Entity::m_iAccountID = g_NetVar.GetOffset(BASE_ATTRIBUTABLE_ITEM, BASE_ATTRIBUTABLE_ITEM_ACCOUNT_ID);
			Entity::m_iEntityQuality = g_NetVar.GetOffset(BASE_ATTRIBUTABLE_ITEM, BASE_ATTRIBUTABLE_ITEM_ENTITY_QAL);
			Entity::m_OriginalOwnerXuidLow = g_NetVar.GetOffset(BASE_ATTRIBUTABLE_ITEM, BASE_ATTRIBUTABLE_ITEM_OWNER_LOW);
			Entity::m_OriginalOwnerXuidHigh = g_NetVar.GetOffset(BASE_ATTRIBUTABLE_ITEM, BASE_ATTRIBUTABLE_ITEM_OWNER_HIGH);
			Entity::m_nFallbackPaintKit = g_NetVar.GetOffset(BASE_ATTRIBUTABLE_ITEM, BASE_ATTRIBUTABLE_ITEM_PAINT_KIT);
			Entity::m_flFallbackWear = g_NetVar.GetOffset(BASE_ATTRIBUTABLE_ITEM, BASE_ATTRIBUTABLE_ITEM_FALL_WEAR);
			Entity::m_nFallbackSeed = g_NetVar.GetOffset(BASE_ATTRIBUTABLE_ITEM, BASE_ATTRIBUTABLE_ITEM_FALL_SEED);
			Entity::m_nFallbackStatTrak = g_NetVar.GetOffset(BASE_ATTRIBUTABLE_ITEM, BASE_ATTRIBUTABLE_ITEM_FALL_STAT);

			Entity::m_nModelIndex = g_NetVar.GetOffset(BASE_ENTITY, BASE_ENTITY_MODEL_INDEX);
			Entity::m_hOwner = g_NetVar.GetOffset(BASE_VIEW_MODEL, BASE_VIEW_MODEL_OWNER);
			Entity::m_hWeapon = g_NetVar.GetOffset(BASE_VIEW_MODEL, BASE_VIEW_MODEL_WEAPON);
			Entity::m_bSpotted = g_NetVar.GetOffset(BASE_ENTITY, BASE_ENTITY_SPOTTED);

			Entity::m_zoomLevel = g_NetVar.GetOffset(BASE_WEAPON_AWP, BASE_WEAPON_AWP_ZOOMLEVEL);
			Entity::m_flSimulationTime = g_NetVar.GetOffset(BASE_ENTITY, SimTime);
			Entity::m_hGroundEntity = g_NetVar.GetOffset(BASE_ENTITY, BASE_ENTITY_ON_GROUND);
			Entity::m_nWaterLevel = g_NetVar.GetOffset(BASE_ENTITY, BASE_ENTITY_WATER_LEVEL);
			Entity::movetype = g_NetVar.GetOffset(BASE_ENTITY, MOVE_TYPE);
			Entity::animlayer = 0x2980;
			return true;
		}

		float* FindW2Matrix()
		{
			return (float*)(reinterpret_cast<DWORD>(&Interfaces::Engine()->WorldToScreenMatrix()) + 0x40);
		}
	}
}




































































































































































































































































































































































