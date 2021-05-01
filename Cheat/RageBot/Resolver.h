#pragma once
#include "../../Engine/Engine.h"
#include <deque>

namespace Engine
{
	class CBaseEntity;
}
class CResolver
{
public:
	bool UseFreestandAngle[65];
	float FreestandAngle[65];

	float pitchHit[65];

	void OnCreateMove();
	void Yaw(CBaseEntity * ent);
	void FrameStage(ClientFrameStage_t stage);
private:
	void AnimationFix(CBaseEntity* pEnt);
};
extern CResolver* Resolver;