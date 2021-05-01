#pragma once

#include "../../Engine/Engine.h"
#include <deque>


#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / Interfaces::GlobalVars()->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( Interfaces::GlobalVars()->interval_per_tick *( t ) )

namespace Engine
{
	class CBaseWeapon;
	class CBaseEntity;
}

class CRagebot
{
public:
	matrix3x4_t Matrix[65][128];
	float FovToPlayer(Vector ViewOffSet, Vector View, CBaseEntity * pEntity, int aHitBox);
	void OnCreateMove(CUserCmd* cmd);
	float bestEntDmg;
	void Autostop(CUserCmd* cmd);
	int targetID = 0;
	
private:
	float LerpTime();
	bool shouldIgnoreLimbs(CBaseEntity* pEnt);
	Vector Hitscan(CBaseEntity* pEnt);
	bool HitChance(CBaseEntity* pEnt, CBaseWeapon* pWeapon, Vector Angle, Vector Point,  int chance);
	float SynchronizeHitchance();
	float SynchronizeMinDamage();
	bool Backtrack[65];
	bool ShotBacktrack[65];

	template<class T, class U>
	T clamp(T in, U low, U high);
};
extern CRagebot* Ragebot;
