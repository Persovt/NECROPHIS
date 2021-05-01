#include "Weapon.h"

namespace Engine
{
	
	int CBaseWeapon::GetWeaponId()
	{
		typedef int(__thiscall* GetWeaponIDFn)(void*);
		return GetMethod<GetWeaponIDFn>(this, 458)(this);
	}

	int	CBaseWeapon::GetWeaponType()
	{
		typedef int(__thiscall* GetWeaponTypeFn)(void*);
		return GetMethod<GetWeaponTypeFn>(this, 453)(this);
	}
	float CBaseWeapon::GetSpread()
	{
		typedef int(__thiscall* GetSpread)(void*);
		return GetMethod<GetSpread>(this, 449)(this);
	}
	
	int CBaseWeapon::GetWeaponAmmo()
	{
		return *(PINT)((DWORD)this + Offset::Entity::m_iClip1);
	}

	int	CBaseWeapon::GetZoomLevel()
	{
		return *(PINT)((DWORD)this + Offset::Entity::m_zoomLevel);
	}

	bool CBaseWeapon::GetWeaponReload()
	{
		return *(bool*)((DWORD)this + Offset::Entity::m_bCanReload);
	}

	float CBaseWeapon::GetNextPrimaryAttack()
	{
		return *(float*)((DWORD)this + Offset::Entity::m_flNextPrimaryAttack);
	}

	CWeaponInfo* CBaseWeapon::GetWeaponInfo()
	{
		typedef CWeaponInfo*(__thiscall* GetWeaponDataFn)(void*);
		return GetMethod< GetWeaponDataFn >(this, 456)(this);
	}

	CBaseAttributableItem* CBaseWeapon::GeteAttributableItem()
	{
		return (CBaseAttributableItem*)(DWORD)this;
	}
	
	float CBaseWeapon::GetInaccuracy()
	{
		typedef int(__thiscall* GetInaccuracy)(void*);
		return GetMethod<GetInaccuracy>(this, 479)(this);
	}

	template<class T>
	static T* FindHudElement(const char* name)
	{
		static auto pThis = *reinterpret_cast<DWORD**>(CSX::Memory::FindPattern("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08" /*"B9 ? ? ? ? E8 ? ? ? ? 85 C0 0F 84 ? ? ? ? 8D 58"*/) + 1);

		static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(CSX::Memory::FindPattern("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
		return (T*)find_hud_element(pThis, name);

	}

	struct hud_weapons_t {
		std::int32_t* get_weapon_count() {
			return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
		}
	};

	void CBaseWeapon::ForceFullUpdate()
	{
		static auto dwrd = CSX::Memory::FindPattern("client_panorama.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC");
		static auto dw_FullUpdate = CSX::Memory::FindPattern("engine.dll", "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");

		if (dw_FullUpdate) {
			static auto clearHudWeaponIcon = dwrd;

			if (clearHudWeaponIcon) {
				static auto clear_hud_weapon_icon_fn =
					reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(clearHudWeaponIcon);
				auto element = FindHudElement<std::uintptr_t*>(("CCSGO_HudWeaponSelection"));

				if (element && clear_hud_weapon_icon_fn) {
					auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0/*0x9c*/);
					if (hud_weapons == nullptr)
						return;

					if (!*hud_weapons->get_weapon_count())
						return;

					for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
						i = clear_hud_weapon_icon_fn(hud_weapons, i);

					typedef void(*CL_FullUpdate_t) (void);
					CL_FullUpdate_t cl_fullupdate = (CL_FullUpdate_t)(dw_FullUpdate);
					cl_fullupdate();
				}

			}
		}

	}

	CWeaponInfo* CBaseWeapon::GetCSWpnData()
	{
		//static auto fnGetWpnData= reinterpret_cast<CWeaponInfo * (__thiscall*)(void*)>(CSX::Memory::FindPattern("client_panorama.dll",  "55 8B EC 81 EC ? ? ? ? 53 8B D9 56 57 8D 8B ? ? ? ? 85 C9 75 04 33 FF EB 2F"));
		//return fnGetWpnData(this);
		typedef CWeaponInfo*(__thiscall* OriginalFn)(void*);
		return GetMethod<OriginalFn>(this, 457)(this);

	}
	float GetCurtime()
	{
		
		if (!LocalPlayer)
			return -1;

		return static_cast<float>(LocalPlayer->GetTickBase()) * Interfaces::GlobalVars()->interval_per_tick;
	}
	bool CBaseWeapon::CanShot()
	{
		if (!LocalPlayer) return false;
		if (LocalPlayer->GetHealth() <= 0) return false;
		if (GetWeaponAmmo() == 0) return false;
		return (GetNextPrimaryAttack() < GetCurtime()) && (LocalPlayer->GetNextAttack() < GetCurtime());
	}

	const char* CBaseWeapon::GetName()
	{
		typedef const char* (__thiscall* GetNameFn)(void*);
		return GetMethod<GetNameFn>(this, 377)(this);
	}
	
	const char*	CBaseWeapon::GetPrintName()
	{
		typedef const char* (__thiscall* GetPrintNameFn)(void*);
		return GetMethod<GetPrintNameFn>(this, 379)(this);
	}

	short* CBaseAttributableItem::GetItemDefinitionIndex()
	{
		return (short*)((DWORD)this + Offset::Entity::m_iItemDefinitionIndex);
	}

	int* CBaseAttributableItem::GetItemIDHigh()
	{
		return (int*)((DWORD)this + Offset::Entity::m_iItemIDHigh);
	}

	int* CBaseAttributableItem::GetAccountID()
	{
		return (int*)((DWORD)this + Offset::Entity::m_iAccountID);
	}

	int* CBaseAttributableItem::GetEntityQuality()
	{
		return (int*)((DWORD)this + Offset::Entity::m_iEntityQuality);
	}

	int* CBaseAttributableItem::GetOriginalOwnerXuidLow()
	{
		return (int*)((DWORD)this + Offset::Entity::m_OriginalOwnerXuidLow);
	}

	int* CBaseAttributableItem::GetOriginalOwnerXuidHigh()
	{
		return (int*)((DWORD)this + Offset::Entity::m_OriginalOwnerXuidHigh);
	}

	int* CBaseAttributableItem::GetFallbackPaintKit()
	{
		return (int*)((DWORD)this + Offset::Entity::m_nFallbackPaintKit);
	}

	float* CBaseAttributableItem::GetFallbackWear()
	{
		return (float*)((DWORD)this + Offset::Entity::m_flFallbackWear);
	}

	int* CBaseAttributableItem::GetFallbackSeed()
	{
		return (int*)((DWORD)this + Offset::Entity::m_nFallbackSeed);
	}

	int* CBaseAttributableItem::GetFallbackStatTrak()
	{
		return (int*)((DWORD)this + Offset::Entity::m_nFallbackStatTrak);
	}

	CBaseViewModel* CBaseAttributableItem::GetViewModel()
	{
		return (CBaseViewModel*)(DWORD)this;
	}
}