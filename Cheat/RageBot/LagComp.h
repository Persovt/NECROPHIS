#pragma once
#include "../../Engine/Engine.h"
#include <deque>


#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / Interfaces::GlobalVars()->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( Interfaces::GlobalVars()->interval_per_tick *( t ) )

struct PlayerRecords
{
	matrix3x4_t Matrix[128];
	float Velocity;
	float SimTimes;
	bool Shot;
	Vector m_vecAbsOrigin;
};

class CLagComp
{
public:
	int ShotTick[65];
	std::deque<PlayerRecords> PlayerRecord[65] = {  };
	void StoreRecord(CBaseEntity* pEnt, CUserCmd* cmd);
	void ClearRecords(int i);
	float LerpTime();
	bool ValidTick(int tick, CUserCmd* cmd);

	template<class T, class U>
	T clamp(T in, U low, U high);
private:
};
extern CLagComp LagComp;