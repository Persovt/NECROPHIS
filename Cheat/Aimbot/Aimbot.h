#pragma once

#include "../../Engine/Engine.h"
#define MAXBACKTRACKTICKS ((int)Settings::Aimbot::aim_Backtracktickrate)
#define TICK_INTERVAL			(Interfaces::GlobalVars()->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TT_CT_BEST_HIT_SIZE_1 ( sizeof( tt_ct_best_hit_1 ) / sizeof( *tt_ct_best_hit_1 ) )
#define TT_CT_BEST_HIT_SIZE_2 ( sizeof( tt_ct_best_hit_2 ) / sizeof( *tt_ct_best_hit_2 ) )
#define TT_CT_BEST_HIT_SIZE_3 ( sizeof( tt_ct_best_hit_3 ) / sizeof( *tt_ct_best_hit_3 ) )
#define TT_CT_BEST_HIT_SIZE_4 ( sizeof( tt_ct_best_hit_4 ) / sizeof( *tt_ct_best_hit_4 ) )
class CTimer;

namespace Engine
{
	class CMe;
	class CPlayer;
}

using namespace Engine;

enum AIM_ATTACK_TYPE
{
	AA_SHOTGUN ,
	AA_SNIPER_PISTOL
};

class CAimbot
{
public:
	CAimbot();

	bool	IsEnable();
	int		GetPlayerFov();
	int		GetBestTarget();
	int		GetBestHitBox();
	void	OnRender();
	void	Aimbot(CUserCmd * cmd, CMe * pLocal, bool &bSendPacket);
	void	AimbotSet(CUserCmd * cmd, CMe * pLocal, bool &bSendPacket);
	
	void	AutoPistol(CUserCmd * cmd);
	
	void	OnCreateMove( CUserCmd* pCmd);
private:
	int		m_iBestPreTarget;
	int		m_iBestTarget;
	int		m_iBestHitbox;
	bool    m_pAim_KeyTrue;
	bool    m_pAim_KeyTrue2;
	ULONGLONG    m_pAim_KeyTrueTime;

	bool	m_bClamp;
	bool	m_bAttack;
	bool	m_bAimShot;
	bool	m_bTargetFov;
	bool	m_bTargetChange;
	bool	m_bTargetPsilentFov;
	bool	m_bAutoPistolEn;
	QAngle m_oldangle;
	CTimer*	m_pShotDelay;
	Vector	m_vAimBestHitbox;
	Vector  m_vAimBestHitboxScreen;
	Vector  m_vAimBestHitboxScreen2;
};
extern CAimbot*	Aimbot;