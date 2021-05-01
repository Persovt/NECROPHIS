#pragma once
#include <DirectXMath.h>
#include "../../Engine/Engine.h"

using namespace Engine;
template< class T, class Y >
T Clamp(T const& Val, Y const& minVal, Y const& maxVal)
{
	if (Val < minVal)
	{
		return minVal;
	}
	else if (Val > maxVal)
	{
		return maxVal;
	}
	else
	{
		return Val;
	}
}
namespace Engine
{
	class CMe;
	class CPlayer;
}

class CAntiAim
{
public:
	CAntiAim();
	void OnCreateMove(CUserCmd* cmd);
	void fix_movement(CUserCmd* cmd);
	void do_antiaim(CUserCmd* cmd);
	void do_antiaimdesync(CUserCmd* cmd);
	void do_antiaimfake(CUserCmd* cmd);
	void Fakewalk(CUserCmd* cmd);
	void FakeDuck(CUserCmd * cmd);
	void do_freestanding(CUserCmd* cmd);

private:
	CMe*		m_pLocal;
	CUserCmd*	cmd;
};
extern CAntiAim*  AntiAim;