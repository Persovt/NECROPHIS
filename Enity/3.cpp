#include "Entity.h"
#include "../SDK/RecvData.h"
namespace Engine
{
	//[junk_enable /]
	char* CBaseEntity::GetPlayerName()
	{
		if (IsPlayer())
		{
			static PlayerInfo Info;

			if (Interfaces::Engine()->GetPlayerInfo(EntIndex(), &Info))
				return Info.m_szPlayerName;
		}

		return "";
	}
	void CBaseEntity::SetEyeAngles(Vector angles)
	{
		*reinterpret_cast<Vector*>(uintptr_t(this) + Offset::Entity::m_angEyeAngles) = angles;
	}
	Vector CBaseEntity::GetEyePos()
	{
		return *(Vector*)((DWORD)this + Offset::Entity::m_vecOrigin + Offset::Entity::m_vecViewOffset);
	}
	bool CBaseEntity::IsPlayer()
	{
		typedef bool(__thiscall* IsPlayerFn)(void*);
		return GetMethod<IsPlayerFn>(this, 155)(this);
	}

	bool CBaseEntity::IsValid()
	{
		return (!IsDead() && GetHealth() > 0  && !IsDormant() );
	}


	bool CBaseEntity::IsDead()
	{
		BYTE LifeState = *(PBYTE)((DWORD)this + Offset::Entity::m_lifeState);
		return (LifeState != LIFE_ALIVE);
	}

	bool CBaseEntity::IsVisible(CBaseEntity* pLocalEntity)
	{
		if (!pLocalEntity->IsValid())
			return false;

		Vector vSrcOrigin = pLocalEntity->GetEyePosition();

		if (vSrcOrigin.IsZero() || !vSrcOrigin.IsValid())
			return false;

		BYTE bHitBoxCheckVisible[6] = {
			HITBOX_HEAD,
			HITBOX_BODY,
			HITBOX_RIGHT_FOOT,
			HITBOX_LEFT_FOOT,
			HITBOX_RIGHT_HAND,
			HITBOX_LEFT_HAND,
		};

		CTraceFilter filter;

		filter.pSkip = pLocalEntity;

		for (int nHit = 0; nHit < 6; nHit++)
		{
			Vector vHitBox = GetHitboxPosition(bHitBoxCheckVisible[nHit]);

			if (vHitBox.IsZero() || !vHitBox.IsValid())
				continue;

			trace_t tr;
			Ray_t ray;

			ray.Init(vSrcOrigin, vHitBox);

			Interfaces::EngineTrace()->TraceRay(ray, PlayerVisibleMask, &filter, &tr);

			if (tr.m_pEnt == (IClientEntity*)this && !tr.allsolid)
				return true;
		}

		return false;
	}
	int CBaseEntity::GetIndex()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x64);
	}
	int CBaseEntity::GetMoveType()
	{
		if (this != NULL && this != nullptr && (DWORD)this != 0xE)
		{
			return *(int*)((DWORD)this + (DWORD)0x25C);
		}
	}
	bool CBaseEntity::HasHelmet()
	{
		return *(bool*)((DWORD)this + Offset::Entity::m_bHasHelmet);
	}

	bool CBaseEntity::HasDefuser()
	{
		return *(bool*)((DWORD)this + Offset::Entity::m_bHasDefuser);
	}

	int CBaseEntity::IsDefusing()
	{
		return *(bool*)((DWORD)this + (DWORD)Offset::Entity::m_bIsDefusing);
	}

	bool* CBaseEntity::IsSpotted()
	{
		return (bool*)((DWORD)this + Offset::Entity::m_bSpotted);
	}

	float CBaseEntity::GetFlashDuration() {
		return *(float*)((DWORD)this + Offset::Entity::m_flFlashDuration);
	}

	int CBaseEntity::IsFlashed()
	{
		return GetFlashDuration() > 0 ? true : false;
	}

	int	CBaseEntity::GetFovStart()
	{
		return *(PINT)((DWORD)this + Offset::Entity::m_iFOVStart);
	}

	int	CBaseEntity::GetFlags()
	{
		return *(PINT)((DWORD)this + Offset::Entity::m_fFlags);
	}

	int CBaseEntity::GetHealth()
	{
		return *(PINT)((DWORD)this + Offset::Entity::m_iHealth);
	}

	int	CBaseEntity::GetArmor()
	{
		return *(PINT)((DWORD)this + Offset::Entity::m_ArmorValue);
	}

	int	CBaseEntity::GetTeam()
	{
		return *(PINT)((DWORD)this + Offset::Entity::m_iTeamNum);
	}

	short& CBaseWeapon::GetItemDefinitionIndex()
	{
		return *(short*)((DWORD)this + Offset::Entity::m_iItemDefinitionIndex);
	}

	float CBaseEntity::GetLowerBodyYaw()
	{
		return *(float*)((DWORD)this + Offset::Entity::m_flLowerBodyYawTarget);
	}

	float CBaseEntity::GetSimTime()
	{
		return *(float*)((DWORD)this + Offset::Entity::m_flSimulationTime);
	}

	int CBaseEntity::GetShotsFired()
	{
		return *(PINT)((DWORD)this + (DWORD)Offset::Entity::m_iShotsFired);
	}

	int CBaseEntity::GetIsScoped()
	{
		return *(bool*)((DWORD)this + (DWORD)Offset::Entity::m_bIsScoped);
	}

	int	CBaseEntity::GetTickBase()
	{
		return *(PINT)((DWORD)this + (DWORD)Offset::Entity::m_nTickBase);
	}

	float CBaseEntity::m_hGroundEntity()
	{
		return *(float*)((DWORD)this + (DWORD)Offset::Entity::m_hGroundEntity);
	}
	int	CBaseEntity::movetype()
	{
		return *(PINT)((DWORD)this + (DWORD)Offset::Entity::movetype);
	}

	float CBaseEntity::m_nWaterLevel()
	{
		return *(float*)((DWORD)this + (DWORD)Offset::Entity::m_nWaterLevel);
	}


	ObserverMode_t CBaseEntity::GetObserverMode()
	{
		return *(ObserverMode_t*)((DWORD)this + (DWORD)Offset::Entity::m_iObserverMode);
	}

	PVOID CBaseEntity::GetObserverTarget()
	{
		return (PVOID)*(PDWORD)((DWORD)this + (DWORD)Offset::Entity::m_hObserverTarget);
	}

	PVOID CBaseEntity::GetActiveWeapon()
	{
		return (PVOID)((DWORD)this + (DWORD)Offset::Entity::m_hActiveWeapon);
	}

	CBaseWeapon* CBaseEntity::GetBaseWeapon()
	{
		return (CBaseWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle((PVOID)*(PDWORD)GetActiveWeapon());
	}

	UINT* CBaseEntity::GetWeapons()
	{
		// DT_BasePlayer -> m_hMyWeapons
		return (UINT*)((DWORD)this + Offset::Entity::m_hMyWeapons);
	}

	UINT* CBaseEntity::GetWearables()
	{
		return (UINT*)((DWORD)this + Offset::Entity::m_hMyWearables);
	}

	CBaseViewModel* CBaseEntity::GetViewModel()
	{
		// DT_BasePlayer -> m_hViewModel
		return (CBaseViewModel*)Interfaces::EntityList()->GetClientEntityFromHandle((PVOID)*(PDWORD)((DWORD)this + Offset::Entity::m_hViewModel));
	}

	Vector CBaseEntity::GetOrigin()
	{
		return *(Vector*)((DWORD)this + Offset::Entity::m_vecOrigin);
	}

	Vector* CBaseEntity::GetVAngles()
	{
		return (Vector*)((uintptr_t)this + Offset::Entity::deadflag + 0x4);
	}

	Vector CBaseEntity::GetAimPunchAngle()
	{
		return *(Vector*)((DWORD)this + Offset::Entity::m_aimPunchAngle);
	}

	Vector CBaseEntity::GetViewPunchAngle()
	{
		return *(Vector*)((DWORD)this + Offset::Entity::m_viewPunchAngle);
	}

	Vector CBaseEntity::GetVelocity()
	{
		return *(Vector*)((DWORD)this + Offset::Entity::m_vecVelocity);
	}

	Vector CBaseEntity::GetViewOffset()
	{
		return *(Vector*)((DWORD)this + Offset::Entity::m_vecViewOffset);
	}

	Vector CBaseEntity::GetEyePosition()
	{
		return GetRenderOrigin() + GetViewOffset();
	}

	int CBaseEntity::GetSequenceActivity(int sequence)
	{
		const auto model = GetModel();
		if (!model)
			return -1;

		const auto hdr = Interfaces::ModelInfo()->GetStudioModel(model);
		if (!hdr)
			return -1;

		static auto offset = (DWORD)CSX::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83");
		static auto GetSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, SDK::studiohdr_t*, int)>(offset);

		return GetSequenceActivity(this, hdr, sequence);
	}

	QAngle CBaseEntity::GetEyeAngles()
	{
		return *reinterpret_cast<QAngle*>((DWORD)this + Offset::Entity::m_angEyeAngles);
	}
	CAnimationLayer& CBaseEntity::GetAnimOverlay(int Index)
	{
		return (*(CAnimationLayer**)((DWORD)this + Offset::Entity::animlayer))[Index];
	}
	Vector CBaseEntity::GetBonePosition(int nBone)
	{
		Vector vRet;
		matrix3x4_t MatrixArray[MAXSTUDIOBONES];

		if (!SetupBones(MatrixArray, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, Interfaces::GlobalVars()->curtime))
			return vRet;

		matrix3x4_t HitboxMatrix = MatrixArray[nBone];

		vRet = Vector(HitboxMatrix[0][3], HitboxMatrix[1][3], HitboxMatrix[2][3]);

		return vRet;
	}
	mstudiobone_t* CBaseEntity::GetBone(int nBone)
	{
		mstudiobone_t* pBoneBox = nullptr;

		studiohdr_t* pStudioModel = GetStudioModel();

		if (!pStudioModel)
			return pBoneBox;

		mstudiobone_t* pBone = pStudioModel->pBone(nBone);

		if (!pBone)
			return nullptr;

		return pBone;
	}
	bool CBaseWeapon::IsKnife()
	{
		if (!this)
			return false;

		switch (this->GetItemDefinitionIndex())
		{
		case WEAPON_KNIFE:
		case WEAPON_KNIFE_T:
		case WEAPON_KNIFE_GUT:
		case WEAPON_KNIFE_FLIP:
		case WEAPON_KNIFE_BAYONET:
		case WEAPON_KNIFE_M9_BAYONET:
		case WEAPON_KNIFE_KARAMBIT:
		case WEAPON_KNIFE_TACTICAL:
		case WEAPON_KNIFE_BUTTERFLY:
		case WEAPON_KNIFE_SURVIVAL_BOWIE:
		case WEAPON_KNIFE_FALCHION:
		case WEAPON_KNIFE_PUSH:
			return true;
		default:
			return false;
		}
	}


	bool SetupBones(VMatrix *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		__asm
		{
			mov edi, this
			lea ecx, dword ptr ds : [edi + 0x4]
			mov edx, dword ptr ds : [ecx]
			push currentTime
			push boneMask
			push nMaxBones
			push pBoneToWorldOut
			call dword ptr ds : [edx + 0x34]
		}
	}

	int CBaseEntity::GetHitboxSet_()
	{
		return *(int*)((DWORD)this + Offset::Entity::m_nHitboxSet);
	}

	Vector CBaseEntity::GetHitboxPosition(int nHitbox)
	{
		matrix3x4_t MatrixArray[MAXSTUDIOBONES];
		Vector vRet, vMin, vMax;

		vRet = Vector(0, 0, 0);

		mstudiobbox_t* pHitboxBox = GetHitBox(nHitbox);

		if (!pHitboxBox || !IsValid())
			return vRet;

		if (!SetupBones(MatrixArray, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0/*Interfaces::GlobalVars()->curtime*/))
			return vRet;

		if (!pHitboxBox->m_Bone || !pHitboxBox->m_vBbmin.IsValid() || !pHitboxBox->m_vBbmax.IsValid())
			return vRet;

		VectorTransform(pHitboxBox->m_vBbmin, MatrixArray[pHitboxBox->m_Bone], vMin);
		VectorTransform(pHitboxBox->m_vBbmax, MatrixArray[pHitboxBox->m_Bone], vMax);

		vRet = (vMin + vMax) * 0.5f;

		return vRet;
	}

	int	CBaseViewModel::GetModelIndex()
	{
		// DT_BaseViewModel -> m_nModelIndex
		return *(int*)((DWORD)this + Offset::Entity::m_nModelIndex);
	}
	
	int& CBaseWeapon::GetWeaponID()
	{
		return *(int*)((DWORD)this + Offset::Entity::m_iWeaponID);
	}
	bool CBaseWeapon::IsAK47()
	{
		if (!this)
			return false;

		switch (this->GetItemDefinitionIndex())
		{
		case WEAPON_AK47:
			return true;
		default:
			return false;
		}
	}

	bool CBaseWeapon::IsAWP()
	{
		if (!this)
			return false;

		switch (this->GetItemDefinitionIndex())
		{
		case WEAPON_AWP:
			return true;
		default:
			return false;
		}
	}

	bool CBaseWeapon::IsGlock()
	{
		if (!this)
			return false;

		switch (this->GetItemDefinitionIndex())
		{
		case WEAPON_GLOCK:
			return true;
		default:
			return false;
		}
	}

	bool CBaseWeapon::IsM4A4()
	{
		if (!this)
			return false;

		switch (this->GetItemDefinitionIndex())
		{
		case WEAPON_M4A4:
			return true;
		default:
			return false;
		}
	}

	bool CBaseWeapon::IsM4A1S()
	{
		if (!this)
			return false;

		switch (this->GetItemDefinitionIndex())
		{
		case WEAPON_M4A1_SILENCER:
			return true;
		default:
			return false;
		}
	}

	bool CBaseWeapon::IsGun()
	{
		if (!this)
			return false;

		int id = this->GetWeaponID();
		//If your aimbot is broken, this is the reason. Just an FYI.

		switch (id)
		{
		case WEAPON_DEAGLE:
		case WEAPON_ELITE:
		case WEAPON_FIVESEVEN:
		case WEAPON_GLOCK:
		case WEAPON_AK47:
		case WEAPON_AUG:
		case WEAPON_AWP:
		case WEAPON_FAMAS:
		case WEAPON_G3SG1:
		case WEAPON_GALILAR:
		case WEAPON_M249:
		case WEAPON_M4A4:
		case WEAPON_MAC10:
		case WEAPON_P90:
		case WEAPON_UMP45:
		case WEAPON_XM1014:
		case WEAPON_BIZON:
		case WEAPON_MAG7:
		case WEAPON_NEGEV:
		case WEAPON_SAWEDOFF:
		case WEAPON_TEC9:
			return true;
		case WEAPON_TASER:
			return false;
		case WEAPON_HKP2000:
		case WEAPON_MP7:
		case WEAPON_MP9:
		case WEAPON_NOVA:
		case WEAPON_P250:
		case WEAPON_SCAR20:
		case WEAPON_SG553:
		case WEAPON_SSG08:
			return true;
		case WEAPON_KNIFE:
		case WEAPON_FLASHBANG:
		case WEAPON_HEGRENADE:
		case WEAPON_SMOKEGRENADE:
		case WEAPON_MOLOTOV:
		case WEAPON_DECOY:
		case WEAPON_INCGRENADE:
		case WEAPON_C4:
		case WEAPON_KNIFE_T:
			return false;
		case WEAPON_M4A1_SILENCER:
		case WEAPON_USP_SILENCER:
		case WEAPON_CZ75A:
		case WEAPON_REVOLVER:
			return true;
		default:
			return false;
		}
	}
	

	void CBaseViewModel::SetModelIndex(int nModelIndex)
	{
		VirtualFn(void)(PVOID, int);
		GetMethod< OriginalFn >(this, 75)(this, nModelIndex);
	}

	void CBaseViewModel::SetWeaponModel(const char* Filename, IClientEntity* Weapon)
	{
		typedef void(__thiscall* SetWeaponModelFn)(void*, const char*, IClientEntity*);
		return GetMethod<SetWeaponModelFn>(this, 242)(this, Filename, Weapon);
	}

	DWORD CBaseViewModel::GetOwner()
	{
		// DT_BaseViewModel -> m_hOwner
		return *(PDWORD)((DWORD)this + Offset::Entity::m_hOwner);
	}




	Vector* CBaseEntity::GetEyeAnglesPtr()
	{
		return reinterpret_cast<Vector*>((DWORD)this + Offset::Entity::m_angEyeAngles);
	}

	DWORD CBaseViewModel::GetWeapon()
	{
		// DT_BaseViewModel -> m_hWeapon
		return *(PDWORD)((DWORD)this + Offset::Entity::m_hWeapon);
	}

	void CBaseEntity::SetAngle2(Vector wantedang) {
		typedef void(__thiscall* oSetAngle)(void*, const Vector &);
		static oSetAngle _SetAngle = (oSetAngle)((uintptr_t)CSX::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		_SetAngle(this, wantedang);
	}
	int CBaseEntity::DrawModel2(int flags, uint8_t alpha)
	{
		VirtualFn(void)(PVOID, int);
		void* pRenderable = (void*)(this + 0x4);

		using fn = int(__thiscall*)(void*, int, uint8_t);
		return GetMethod<fn>(pRenderable, 9)(pRenderable, flags, alpha);
	
	}
	void CBaseEntity::SetAbsOrigin(Vector ArgOrigin)
	{
		using Fn = void(__thiscall*)(CBaseEntity*, const Vector &origin);
		static Fn func;

		if (!func)
			func = (Fn)(CSX::Memory::FindPatternV2("client_panorama.dll", "\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8\x00\x00"));

		func(this, ArgOrigin);
	}

	model_t* CBaseEntity::GetModel2()
	{
		return *(model_t**)((DWORD)this + 0x6C);
	}

	CBaseWeapon* CBaseEntity::GetWeapon()
	{
		return (CBaseWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle((PVOID)*(PDWORD)GetActiveWeapon());;

	}

	
	CCSGOAnimState* CBaseEntity::GetAnimState()
	{
		return *reinterpret_cast<CCSGOAnimState**>(uintptr_t(this) + Offset::Entity::animstate);
	}
	
	bool CBaseWeapon::IsGrenade()
	{
		if (!this)
			return false;

		switch (this->GetItemDefinitionIndex())
		{
		case WEAPON_SMOKEGRENADE:
		case WEAPON_HEGRENADE:
		case WEAPON_INCGRENADE:
		case WEAPON_FLASHBANG:
		case WEAPON_MOLOTOV:
		case WEAPON_DECOY:
			return true;
		default:
			return false;
		}
	}

	bool CBaseEntity::SetupBones2(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		__asm
		{
			mov edi, this
			lea ecx, dword ptr ds : [edi + 0x4]
			mov edx, dword ptr ds : [ecx]
			push currentTime
			push boneMask
			push nMaxBones
			push pBoneToWorldOut
			call dword ptr ds : [edx + 0x34]
		}
	}
	int CBaseEntity::GetBoneByName(const char* boneName)
	{
		studiohdr_t* pStudioModel = Interfaces::ModelInfo()->GetStudioModel(this->GetModel2());
		if (!pStudioModel)
			return -1;

		matrix3x4_t pBoneToWorldOut[128];
		if (!this->SetupBones(pBoneToWorldOut, 128, 256, 0))
			return -1;

		for (int i = 0; i < pStudioModel->numbones; i++)
		{
			mstudiobone_t *pBone = pStudioModel->pBone(i);
			if (!pBone)
				continue;

			if (pBone->pszName() && strcmp(pBone->pszName(), boneName) == 0)
				return i;
		}

		return -1;
	}
	float CBaseEntity::getmaxdesync()
	{

		auto animstate = uintptr_t(this->GetAnimState());

		float rate = 180;
		float duckammount = *(float *)(animstate + 0xA4);
		float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

		float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

		float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
		float unk2 = unk1 + 1.1f;
		float unk3;

		if (duckammount > 0) {

			unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

		}

		unk3 = *(float *)(animstate + 0x334) * unk2;

		return unk3;

	} 
	float& CBaseEntity::m_flAbsRotation() {
		return *(float*)((uintptr_t)this + 0x80);
	}
	float CBaseEntity::get_max_desync_delta(CBaseEntity *local) {

		uintptr_t animstate = uintptr_t(local->GetAnimState());


		float duckammount = *(float *)(animstate + 0xA4);
		float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

		float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

		float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
		float unk2 = unk1 + 1.1f;
		float unk3;

		if (duckammount > 0) {

			unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

		}

		unk3 = *(float *)(animstate + 0x334) * unk2;

		return unk3;

	}
	
	float CBaseEntity::GetNextAttack()
	{
		return *reinterpret_cast<float*>(uint32_t(this) + Offset::Entity::m_flNextAttack);
	}
	int CBaseEntity::GetActiveWeaponIndex()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + Offset::Entity::m_hActiveWeapon) & 0xFFF;
	}
}
