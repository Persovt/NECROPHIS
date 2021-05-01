#pragma once

#include "../../Engine/Engine.h"

namespace Engine
{
	class CPlayer;
}

using namespace Engine;

class CFakeLag
{
public:
	void Fakelag(CUserCmd * cmd);
	//[/swap_lines]
private:
	int ticks = 0;
	int ticksMax = 16;
	int choked;
};
extern CFakeLag* FakeLag;