#pragma once

#include "../../Engine/Engine.h"

class CMisc
{
public:

	void OnRender();
	void OnCreateMove( CUserCmd* pCmd );
	void OnDrawModelExecute();
	
	void OnPlaySound(const char* pszSoundName);
	void OnOverrideView(int& OverrideFov);
	void OnGetViewModelFOV( int& ViewModelfov);
	vector<int> GetObservervators( int playerId );
	void OnRenderSpectatorList();


};
extern CMisc* Misc;