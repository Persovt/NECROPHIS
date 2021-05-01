#pragma once

#include "../../Engine/Engine.h"
#include "../../SDK/Sounds.h"


#include <algorithm>

#include <Windows.h>
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib") 
#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#ifndef RAD2DEG
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#endif

#ifndef DEG2RAD
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif
namespace Engine
{
	class CBaseEntity;
	class CPlayer;
}

using namespace Engine;
inline float CSGO_Armor(float flDamage, int ArmorValue)
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (ArmorValue > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;

		if (flArmor > static_cast<float>(ArmorValue)) {
			flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}
	return flDamage;
}
#define CHECK_VALID( _v)    0

FORCEINLINE vec_t DotProduct(const Vector& a, const Vector& b)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	return(a.x*b.x + a.y*b.y + a.z*b.z);
}
struct DamageIndicator_t
{
	int iDamage;
	bool bInitialized;
	float flEraseTime;
	float flLastUpdate;
	CBaseEntity * Player;
	Vector Position;
};
class CEsp 
{
public:
	CEsp();
	
	Color CT_HP_ColorM;
	Color TT_HP_ColorM;
	void GrenadePrediction();
	Color CT_AR_ColorM;
	Color TT_AR_ColorM;
	void ThirdPerson();
	IMaterial* visible_flat;
	IMaterial* visible_tex;
	IMaterial* hidden_flat;
	IMaterial* hidden_tex;
	float fExplodeC4Timer;
	float fC4Timer;

	
	void DrawDamageIndicator();
	bool CheckPlayerTeam(CPlayer* pPlayer, CBaseEntity* pBaseEntity);
	void DrawHitmarker();
	void renderBeams();
	void HitmarkerEvents(IGameEvent * event);
	void OnRender();
	void OnCreateMove(CUserCmd* pCmd);
	void OnReset();
	void OnEvents(IGameEvent* pEvent);
	void OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
	void NightMode();
private:
	
	void DrawPlayerEsp(CPlayer* pPlayer);
	void DrawPlayerSkeleton(CBaseEntity* Entity);

};
extern CEsp* Esp;