#pragma once
#include "../../Engine/Engine.h"






class CSkinChanger
{
public:
	void fsn_skinchanger(CBaseEntity* pLocalPlayer, ClientFrameStage_t stage);
};

extern CSkinChanger* SkinChanger;
