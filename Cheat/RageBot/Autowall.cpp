#include "Autowall.h"
#include <algorithm>


CAutowall* Autowall = new CAutowall;


Vector aim;


float GetHitgroupDamageMult(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_HEAD:
		return 4.f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	}

	return 1.0f;
}

void ScaleDamage(int hitgroup, CBaseEntity *enemy, float weapon_armor_ratio, float &current_damage)
{
	current_damage *= GetHitgroupDamageMult(hitgroup);

	if (enemy->GetArmor() > 0.0f && hitgroup < HITGROUP_LEFTLEG)
	{
		if (hitgroup == HITGROUP_HEAD && !enemy->HasHelmet())
			return;

		float armorscaled = (weapon_armor_ratio * 0.5f) * current_damage;
		if ((current_damage - armorscaled) * 0.5f > enemy->GetArmor())
			armorscaled = current_damage - (enemy->GetArmor() * 2.0f);
		current_damage = armorscaled;
	}
}


void TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, CBaseEntity* ignore, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	CTraceFilter filter;
	filter.pSkip = ignore;

	Interfaces::EngineTrace()->TraceRay(ray, mask, &filter, ptr);
}
typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
TraceToExitFn TraceToExits = (TraceToExitFn)CSX::Memory::FindPattern(("client_panorama.dll"), ("55 8B EC 83 EC 30 F3 0F 10 75"));
bool TraceToExit(Vector& end, trace_t& tr, float x, float y, float z, float x2, float y2, float z2, trace_t* trace)
{
	if (!TraceToExits)
	{
		return false;
	}
	_asm
	{
		push trace
		push z2
		push y2
		push x2
		push z
		push y
		push x
		mov edx, tr
		mov ecx, end
		call TraceToExits
		add esp, 0x1C
	}
}
bool HandleBulletPenetration(CWeaponInfo *wpn_data, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = Interfaces::Physprops()->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = *(float*)((DWORD)enter_surface_data + 88);

	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= pow((wpn_data->range_modifier), (data.trace_length * 0.002));

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;
	trace_t trace_exit;
	if (!TraceToExit(dummy, data.enter_trace, data.enter_trace.endpos.x, data.enter_trace.endpos.y, data.enter_trace.endpos.z, data.direction.x, data.direction.y, data.direction.z, &trace_exit))
		return false;

	surfacedata_t *exit_surface_data = Interfaces::Physprops()->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = *(float*)((DWORD)exit_surface_data + 88);
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->penetration) * 1.25f);
	float thickness = (trace_exit.endpos - data.enter_trace.endpos).Length();

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;

	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}

bool SimulateFireBullet(CBaseWeapon* pWeapon, FireBulletData &data)
{
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	CWeaponInfo* weaponData = mLocal->m_pWeaponEntity->GetCSWpnData();

	if (weaponData == NULL)
		return false;
	CBaseEntity* m_pEnt = (CBaseEntity*)data.enter_trace.m_pEnt;
	data.current_damage = (float)weaponData->damage;

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = weaponData->range - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		TraceLine(data.src, end, 0x4600400B, LocalPlayer, &data.enter_trace);
		Math::ClipTraceToPlayers(data.src, end + data.direction * 40.f, 0x4600400B, &data.filter, &data.enter_trace);

		if (data.enter_trace.fraction == 1.0f)
			break;

		if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0)&& (LocalPlayer->GetTeam() != m_pEnt->GetTeam()))
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= pow(weaponData->range_modifier, data.trace_length * 0.002);
			ScaleDamage(data.enter_trace.hitgroup, m_pEnt, weaponData->armor_ratio, data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(weaponData, data))
			break;
	}

	return false;
}

float CAutowall::GetDamage(const Vector& point)
{
	FireBulletData data;
	data.src = LocalPlayer->GetEyePosition();
	data.filter.pSkip = LocalPlayer;

	QAngle angles = Math::CalcAngle(data.src, point);
	Math::AngleVectors(angles, &data.direction,0,0);
//	Vector dataNormalized;

//	data.direction.Normalized();

	CBaseWeapon* pWeap = mLocal->m_pWeaponEntity;
	if (SimulateFireBullet(pWeap, data))
		return data.current_damage;
	

	return 0;
}





/*
bool CAutowall::CanHitFloatingPoint(const Vector &point, const Vector &source)
{

	aim = point;

	if (!LocalPlayer)
		return false;

	FireBulletData data;

	Vector angles = Math::CalcAngle(data.src, point);
	Math::AngleVectors(angles, &data.direction,0,0);
	Vector VectorNormalize(data.direction);

	

	if (!mLocal->m_pWeaponEntity)
		return false;

	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	CWeaponInfo *weaponData = mLocal->m_pWeaponEntity->GetCSWpnData();

	if (!weaponData)
		return false;

	data.current_damage = (float)weaponData->damage;

	data.trace_length_remaining = weaponData->range - data.trace_length;

	Vector end = data.src + data.direction * data.trace_length_remaining;

	TraceLine(data.src, end, mask_shot | contents_hitbox, LocalPlayer, &data.enter_trace);

	if (VectortoVectorVisible(data.src, point))
	{
		return true;
	}
	static bool extra_check = true;
	if (HandleBulletPenetration(weaponData, data, extra_check))
	{
		return true;
	}

	return false;
}*/
void CalcAngle34(Vector src, Vector dst, Vector &angles)
{
	Vector delta = src - dst;
	double hyp = delta.Length2D(); //delta.Length
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	angles[2] = 0.00;

	if (delta.x >= 0.0)
		angles.y += 180.0f;
}

/*
bool CAutowall::CanWallbang(float &dmg)
{
	
	if (!LocalPlayer)
		return false;

	if (LocalPlayer->IsDead())
		return false;


	FireBulletData data;
	//	data.filter = trace;
	data.filter.pSkip = LocalPlayer;

	Vector EyeAng;
	Interfaces::Engine()->GetViewAngles(EyeAng);

	Vector dst, forward;

	Math::AngleVectors(EyeAng, &forward,0,0);
	dst = data.src + (forward * 8196.f);

	Vector angles;
	CalcAngle34(data.src, dst, angles);
	Math::AngleVectors(angles, &data.direction,0 ,0);
	Vector VectorNormalize(data.direction);

	

	if (!mLocal->m_pWeaponEntity)
		return false;

	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	CWeaponInfo* weaponData = mLocal->m_pWeaponEntity->GetCSWpnData();

	if (!weaponData)
		return false;

	data.current_damage = (float)weaponData->damage;

	data.trace_length_remaining = weaponData->range - data.trace_length;

	Vector end = data.src + data.direction * data.trace_length_remaining;

	TraceLine(data.src, end, mask_shot | contents_grate, LocalPlayer, &data.enter_trace);

	if (data.enter_trace.fraction == 1.0f)
		return false;

	if (HandleBulletPenetration(weaponData, data, false))
	{
		dmg = data.current_damage;
		return true;
	}

	return false;
}*/