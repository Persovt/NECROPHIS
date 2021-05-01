#pragma once
#include "../Engine/Engine.h"

extern int iScreenWidth;
extern int iScreenHeight;
extern Vector         AimbotHitbox[65][27];
extern Vector         EnemyEyeAngs[65];
extern Vector2D		CenterScreens;
extern bool           Shot[65];
extern int TargetIndex;
extern bool backtrack_tick[65];
extern int shots_fired[65];
extern int shots_hit[65];
extern bool           Hit[65];
extern int            MissedShots[65];
extern float update_time[65];
extern float lby_to_back[65];
extern int resolve_type[65];
extern bool using_fake_angles[65];
extern int	iWeaponID;
extern int changvis;
extern bool bSendPacket;
extern Vector RealAngles;
extern bool auth;
extern int pWeaponItemIndexData[34];
extern char* pWeaponData[34];
extern CBaseEntity* LocalPlayer;
extern CMe* mLocal;
