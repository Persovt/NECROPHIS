#include "Misc.h"


void CMisc::OnRender()
{
	OnRenderSpectatorList();

	if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
	{
		if (settings.Misc.misc_Punch == 3 && (Players->GetLocal()->WeaponIndex == WEAPON_AWP || Players->GetLocal()->WeaponIndex == WEAPON_SCAR20 || Players->GetLocal()->WeaponIndex == WEAPON_SSG08 || Players->GetLocal()->WeaponIndex == WEAPON_G3SG1))
		{
			Color AwpAimColor = Color(int(settings.Misc.misc_AwpAimColor[0] * 255.f),
				int(settings.Misc.misc_AwpAimColor[1] * 255.f),
				int(settings.Misc.misc_AwpAimColor[2] * 255.f));

			Render->DrawOutlineBox(iScreenWidth / 2 - 1, iScreenHeight / 2 - 1, 3, 3, AwpAimColor);
		}
		if (settings.Misc.misc_Punch == 1)
		{
			int punch_x = (int)CenterScreens.x + (int)Players->GetLocal()->vAimPunch.y * 10;
			int punch_y = (int)CenterScreens.y + (int)Players->GetLocal()->vAimPunch.x * 10;

			Render->DrawFillBox(punch_x - 1, punch_y - 1, 3, 3, Color::Green());
		}
		if (settings.Misc.misc_Punch == 2)
		{

			if (!LocalPlayer)
				return;

			Vector ViewAngles;
			Interfaces::Engine()->GetViewAngles(ViewAngles);
			ViewAngles += (LocalPlayer->GetAimPunchAngle()) * 2.f;

			Vector fowardVec;
			Math::AngleVectors(ViewAngles, &fowardVec, 0, 0);
			fowardVec *= 10000;

			Vector start = LocalPlayer->GetEyePosition();
			Vector end = start + fowardVec, endScreen;

			if (WorldToScreen(end, endScreen) && IsLocalAlive())
			{
				Render->DrawLine(endScreen.x + 7, endScreen.y + 0.5, endScreen.x - 8, endScreen.y + 0.5, Color::Red());
				Render->DrawLine(endScreen.x + 0, endScreen.y + 7, endScreen.x + 0, endScreen.y - 8, Color::Red());
			}

		}
	}
}

void CMisc::OnCreateMove(CUserCmd* pCmd)
{

	bool in_water = Players->GetLocal()->m_pEntity->m_nWaterLevel() >= 2;
	bool on_ladder = Players->GetLocal()->m_pEntity->movetype() == MOVETYPE_LADDER;
	bool noclip = Players->GetLocal()->m_pEntity->movetype() == MOVETYPE_NOCLIP;
	bool spectate = Players->GetLocal()->m_pEntity->movetype() == MOVETYPE_OBSERVER;

	ConVar* lefthandknife = Interfaces::GetConVar()->FindVar("cl_righthand");
	
	
	if (settings.Misc.misc_Duck)
		pCmd->buttons |= IN_BULLRUSH;

		if (settings.Misc.misc_Bhop)
		{
			
				static bool bLastJumped = false;
				static bool bShouldFake = false;

				if (!LocalPlayer)
					return;

				if (LocalPlayer->GetMoveType() == MOVETYPE_LADDER || LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP)
					return;

				if (!bLastJumped && bShouldFake)
				{
					bShouldFake = false;
					pCmd->buttons |= IN_JUMP;
				}
				else if (pCmd->buttons & IN_JUMP)
				{
					if (Players->GetLocal()->iFlags & FL_ONGROUND)
					{
						bLastJumped = true;
						bShouldFake = true;
					}
					else
					{
						pCmd->buttons &= ~IN_JUMP;
						bLastJumped = false;
					}
				}
				else
				{
					bLastJumped = false;
					bShouldFake = false;
				}
			

			if (settings.Misc.misc_AutoStrafe && !(Players->GetLocal()->iFlags & FL_ONGROUND) && !on_ladder && !noclip && !spectate && !in_water)
			{
				static float float1;
				static float float2;

				if (!(Players->GetLocal()->iFlags & FL_ONGROUND))
				{
					float2 = CSX::Utils::RandomIntRange(-440, -450);
					float1 = CSX::Utils::RandomIntRange(440, 450);
				}

				if (pCmd->mousedx < 0)
				{
					pCmd->sidemove = float2;
				}
				else if (pCmd->mousedx > 0)
				{
					pCmd->sidemove = float1;
				}
			}
	}

	if (settings.Misc.misc_antiafk)
	{
		static bool Jitter;
		Jitter = !Jitter;
		if (Jitter)
			pCmd->sidemove += 450;
		else
			pCmd->sidemove = -450;

		if (!Jitter)
			pCmd->forwardmove = -450;
		else
			pCmd->forwardmove = +450;

		pCmd->buttons += IN_MOVELEFT;
	}


}



void CMisc::OnPlaySound(const char* pszSoundName)
{
	
}
void CMisc::OnDrawModelExecute()
{


	if (settings.Misc.misc_NoFlash )
	{
		SDK::IMaterial* Flash = Interfaces::MaterialSystem()->FindMaterial("effects\\flashbang", "ClientEffect textures");
		SDK::IMaterial* FlashWhite = Interfaces::MaterialSystem()->FindMaterial("effects\\flashbang_white", "ClientEffect textures");
		Flash->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
		FlashWhite->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
	}

	

	if (settings.Misc.misc_NoSmoke )
	{
		static auto smoke_count = *reinterpret_cast<uint32_t **>(CSX::Memory::FindPattern("client_panorama.dll", "A3 ? ? ? ? 57 8B CB") + 1);

		static std::vector<const char*> smoke_materials = {
			"effects/overlaysmoke",
			"particle/beam_smoke_01",
			"particle/particle_smokegrenade",
			"particle/particle_smokegrenade1",
			"particle/particle_smokegrenade2",
			"particle/particle_smokegrenade3",
			"particle/particle_smokegrenade_sc",
			"particle/smoke1/smoke1",
			"particle/smoke1/smoke1_ash",
			"particle/smoke1/smoke1_nearcull",
			"particle/smoke1/smoke1_nearcull2",
			"particle/smoke1/smoke1_snow",
			"particle/smokesprites_0001",
			"particle/smokestack",
			"particle/vistasmokev1/vistasmokev1",
			"particle/vistasmokev1/vistasmokev1_emods",
			"particle/vistasmokev1/vistasmokev1_emods_impactdust",
			"particle/vistasmokev1/vistasmokev1_fire",
			"particle/vistasmokev1/vistasmokev1_nearcull",
			"particle/vistasmokev1/vistasmokev1_nearcull_fog",
			"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
			"particle/vistasmokev1/vistasmokev1_smokegrenade",
			"particle/vistasmokev1/vistasmokev4_emods_nocull",
			"particle/vistasmokev1/vistasmokev4_nearcull",
			"particle/vistasmokev1/vistasmokev4_nocull"
		};

		for (auto material_name : smoke_materials) {

			IMaterial * material = Interfaces::MaterialSystem()->FindMaterial(material_name, "Other textures");

			material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, "smokegrenade_thrown.mdl");
		}


		*(int*)smoke_count = 0;
	}
	
}






vector<int> CMisc::GetObservervators(int playerId)
{
	vector<int> SpectatorList;

	CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(playerId);

	if (!pPlayer)
		return SpectatorList;

	if (pPlayer->IsDead())
	{
		CBaseEntity* pObserverTarget = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pPlayer->GetObserverTarget());

		if (!pObserverTarget)
			return SpectatorList;

		pPlayer = pObserverTarget;
	}

	for (int PlayerIndex = 0; PlayerIndex < Players->GetSize(); PlayerIndex++)
	{
		CBaseEntity* pCheckPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(PlayerIndex);

		if (!pCheckPlayer)
			continue;

		if (pCheckPlayer->IsDormant() || !pCheckPlayer->IsDead())
			continue;

		CBaseEntity* pObserverTarget = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pCheckPlayer->GetObserverTarget());

		if (!pObserverTarget)
			continue;

		if (pPlayer != pObserverTarget)
			continue;

		SpectatorList.push_back(PlayerIndex);
	}

	return SpectatorList;
}

void CMisc::OnRenderSpectatorList()
{
	if (settings.Misc.misc_Spectators)
	{
		int specs = 0;
		int modes = 0;
		std::string spect = "";
		std::string mode = "";
		int DrawIndex = 1;

		for (int playerId : GetObservervators(Interfaces::Engine()->GetLocalPlayer()))
		{
			if (playerId == Interfaces::Engine()->GetLocalPlayer())
				continue;

			CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(playerId);

			if (!pPlayer)
				continue;

			PlayerInfo pInfo;
			Interfaces::Engine()->GetPlayerInfo(playerId, &pInfo);

			if (pInfo.m_bIsFakePlayer)
				continue;

			spect += pInfo.m_szPlayerName;
			spect += "\n";
			specs++;

			
		}
		bool misc_Spectators = true;

		if (ImGui::Begin("Spectator List", &settings.Misc.misc_Spectators, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar))
		{
			if (specs > 0) spect += "\n";
			ImVec2 size = ImGui::CalcTextSize(spect.c_str());
			ImGui::SetWindowSize(ImVec2(200, 25 + size.y));
			Gui->MainFont1();
			
			ImGui::Text(spect.c_str());

			DrawIndex++;
			ImGui::End();
		}
	}
}
CMisc* Misc = new CMisc;