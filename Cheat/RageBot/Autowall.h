#pragma once
#include "../../Engine/Engine.h"


struct FireBulletData
{
	
	
	Vector          src;
	trace_t         enter_trace;
	Vector          direction;
	CTraceFilter   filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};

class CAutowall
{
public:

	float GetDamage(const Vector& point);
	bool CanHitFloatingPoint(const Vector &point, const Vector &source);
	bool CanWallbang(float & dmg);
	//	float get_damage_vec(C_BaseEntity* dst_entity, Vector point, int hitbox);
	bool CanHit(const Vector &point, float *damage_given);
};
extern CAutowall* Autowall;
