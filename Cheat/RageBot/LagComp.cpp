#include "LagComp.h"
#include "Ragebot.h"

#define FLOW_OUTGOING	0
#define FLOW_INCOMING	1
CLagComp LagComp;

float CLagComp::LerpTime() 
{
	int updaterate = Interfaces::GetConVar()->FindVar("cl_updaterate")->GetInt();
	ConVar* minupdate = Interfaces::GetConVar()->FindVar("sv_minupdaterate");
	ConVar* maxupdate = Interfaces::GetConVar()->FindVar("sv_maxupdaterate");

	if (minupdate && maxupdate)
		updaterate = maxupdate->GetInt();

	float ratio = Interfaces::GetConVar()->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = Interfaces::GetConVar()->FindVar("cl_interp")->GetFloat();
	ConVar* cmin = Interfaces::GetConVar()->FindVar("sv_client_min_interp_ratio");
	ConVar* cmax = Interfaces::GetConVar()->FindVar("sv_client_max_interp_ratio");

	if (cmin && cmax && cmin->GetFloat() != 1)
		ratio = clamp(ratio, cmin->GetFloat(), cmax->GetFloat());

	return max(lerp, (ratio / updaterate));
}

bool CLagComp::ValidTick(int tick, CUserCmd* cmd) // gucci i think cant remember
{
	
	auto nci = Interfaces::Engine()->GetNetChannelInfo();

	if (!nci)
		return false;

	auto PredictedCmdArrivalTick = cmd->tick_count + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING));
	auto Correct = clamp(LerpTime() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(PredictedCmdArrivalTick + TIME_TO_TICKS(LerpTime()) - (tick + TIME_TO_TICKS(LerpTime())));

	return (abs(Correct) <= 0.2f);
}

void CLagComp::StoreRecord(CBaseEntity* pEnt, CUserCmd* cmd) // best lag comp in the world
{
	PlayerRecords Setup;

	static float ShotTime[65];
	static float OldSimtime[65];
	
	
	if (pEnt != LocalPlayer)
		pEnt->FixSetupBones(Ragebot->Matrix[pEnt->EntIndex()]);

	if (PlayerRecord[pEnt->EntIndex()].size() == 0)
	{
		Setup.Velocity = abs(pEnt->GetVelocity().Length2D());

		Setup.SimTimes = pEnt->GetSimTime();

		memcpy(Setup.Matrix, Ragebot->Matrix[pEnt->EntIndex()], (sizeof(matrix3x4_t) * 128));

		Setup.Shot = false;

		PlayerRecord[pEnt->EntIndex()].push_back(Setup);
	}

	if (OldSimtime[pEnt->EntIndex()] != pEnt->GetSimTime())
	{
		Setup.Velocity = abs(pEnt->GetVelocity().Length2D());

		Setup.SimTimes = pEnt->GetSimTime();

		Setup.m_vecAbsOrigin = pEnt->GetAbsOrigin();

		if (pEnt == LocalPlayer)
			pEnt->FixSetupBones(Ragebot->Matrix[pEnt->EntIndex()]);

		memcpy(Setup.Matrix, Ragebot->Matrix[pEnt->EntIndex()], (sizeof(matrix3x4_t) * 128));

		if (pEnt->GetActiveWeapon())
		{
			if (ShotTime[pEnt->EntIndex()] != pEnt->GetLastShotTime())
			{
				Setup.Shot = true;
				ShotTime[pEnt->EntIndex()] = pEnt->GetLastShotTime();
			}
			else
				Setup.Shot = false;
		}
		else
		{
			Setup.Shot = false;
			ShotTime[pEnt->EntIndex()] = 0.f;
		}

		PlayerRecord[pEnt->EntIndex()].push_back(Setup);

		OldSimtime[pEnt->EntIndex()] = pEnt->GetSimTime();
	}


	ShotTick[pEnt->EntIndex()] = -1;

	if (PlayerRecord[pEnt->EntIndex()].size() > 0)
		for (int tick = 0; tick < PlayerRecord[pEnt->EntIndex()].size(); tick++)
			if (!ValidTick(TIME_TO_TICKS(PlayerRecord[pEnt->EntIndex()].at(tick).SimTimes), cmd))
				PlayerRecord[pEnt->EntIndex()].erase(PlayerRecord[pEnt->EntIndex()].begin() + tick);
			else if (PlayerRecord[pEnt->EntIndex()].at(tick).Shot)
				ShotTick[pEnt->EntIndex()] = tick; // gets the newest shot tick
}

void CLagComp::ClearRecords(int i)
{
	if (PlayerRecord[i].size() > 0)
	{
		for (int tick = 0; tick < PlayerRecord[i].size(); tick++)
		{
			PlayerRecord[i].erase(PlayerRecord[i].begin() + tick);
		}
	}
}

template<class T, class U>
T CLagComp::clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}