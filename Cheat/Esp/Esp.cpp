#include "Esp.h"
#include "../../Render/Render.h"
#include "../../SDK/beambullets.h"


std::vector<DamageIndicator_t> DamageIndicator;
std::vector<cbullet_tracer_info> logs;


SDK::PlayerInfo GetInfo(int Index)
{
	SDK::PlayerInfo info;
	Interfaces::Engine()->GetPlayerInfo(Index, &info);
	return info;
}

char* HitgroupToName(int hitgroup)
{
	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		return "head";
	case HITGROUP_LEFTLEG:
		return "leg";
	case HITGROUP_RIGHTLEG:
		return "leg";
	case HITGROUP_LEFTARM:
		return "arm";
	case HITGROUP_RIGHTARM:
		return "arm";
	case HITGROUP_STOMACH:
		return "chest";
	default:
		return "chest";
	}
}
//using namespace Client;


CEsp::CEsp()
{
	CT_HP_ColorM.SetColor(255, 64, 64);
	TT_HP_ColorM.SetColor(255, 64, 64);

	CT_AR_ColorM.SetColor(255, 64, 64);
	TT_AR_ColorM.SetColor(255, 64, 64);

	visible_flat = nullptr;
	visible_tex = nullptr;
	hidden_flat = nullptr;
	hidden_tex = nullptr;

	fExplodeC4Timer = 0.f;
	fC4Timer = 0.f;


}

void CEsp::DrawDamageIndicator()
{
	float CurrentTime = LocalPlayer->GetTickBase() * Interfaces::GlobalVars()->interval_per_tick;

	for (int i = 0; i < DamageIndicator.size(); i++)
	{
		if (DamageIndicator[i].flEraseTime < CurrentTime)
		{
			DamageIndicator.erase(DamageIndicator.begin() + i);
			continue;
		}

		if (!DamageIndicator[i].bInitialized)
		{
			DamageIndicator[i].Position = DamageIndicator[i].Player->GetBonePosition(HITBOX_HEAD);
			DamageIndicator[i].bInitialized = true;
		}

		if (CurrentTime - DamageIndicator[i].flLastUpdate > 0.0001f)
		{
			DamageIndicator[i].Position.z -= (0.1f * (CurrentTime - DamageIndicator[i].flEraseTime));
			DamageIndicator[i].flLastUpdate = CurrentTime;
		}

		Vector ScreenPosition;

	if (WorldToScreen(DamageIndicator[i].Position, ScreenPosition))
		Render->Text(ScreenPosition.x, ScreenPosition.y, false, true, Color(int(settings.Visual.DamagerColor[0] * 255.f),int(settings.Visual.DamagerColor[1] * 255.f),int(settings.Visual.DamagerColor[2] * 255.f)), std::to_string(DamageIndicator[i].iDamage).c_str());
		
	}
}


void CEsp::GrenadePrediction()
{
	if (IsLocalAlive())
	{
		const float TIMEALIVE = 5.f;
		const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;

		float fStep = 0.1f;
		float fGravity = 800.0f / 8.f;

		Vector vPos, vThrow, vThrow2;
		Vector vStart;

		int iCollisions = 0;

		QAngle vViewAngles;
		Interfaces::Engine()->GetViewAngles(vViewAngles);
		
		vThrow[0] = vViewAngles[0];
		vThrow[1] = vViewAngles[1];
		vThrow[2] = vViewAngles[2];

		if (vThrow[0] < 0)
			vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);
		else
			vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);

		float fVel = (90 - vThrow[0]) * 4;
		if (fVel > 500)
			fVel = 500;

		Math::AngleVectors(vThrow, &vThrow2,0 , 0);

		Vector vEye = LocalPlayer->GetEyePosition();
		vStart[0] = vEye[0] + vThrow2[0] * 16;
		vStart[1] = vEye[1] + vThrow2[1] * 16;
		vStart[2] = vEye[2] + vThrow2[2] * 16;

		vThrow2[0] = (vThrow2[0] * fVel) + LocalPlayer->GetVelocity()[0];
		vThrow2[1] = (vThrow2[1] * fVel) + LocalPlayer->GetVelocity()[1];
		vThrow2[2] = (vThrow2[2] * fVel) + LocalPlayer->GetVelocity()[2];

#define IsGrenade Players->GetLocal()->WeaponIndex == WEAPON_SMOKEGRENADE | Players->GetLocal()->WeaponIndex == WEAPON_HEGRENADE | Players->GetLocal()->WeaponIndex == WEAPON_INCGRENADE | Players->GetLocal()->WeaponIndex == WEAPON_MOLOTOV | Players->GetLocal()->WeaponIndex == WEAPON_DECOY | Players->GetLocal()->WeaponIndex == WEAPON_FLASHBANG

		if (!LocalPlayer->IsDead() && IsGrenade)
		{
			for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
			{
				vPos = vStart + vThrow2 * fStep;

				Ray_t ray;
				trace_t tr;
				CTraceFilter loc;
				//loc.pSkip = pLocal;

				ray.Init(vStart, vPos);
				Interfaces::EngineTrace()->TraceRay(ray, MASK_SOLID, &loc, &tr);
				Color color = Color(0, 255, 0);
				if (tr.DidHit())
				{
					float anglez = DotProduct(Vector(0, 0, 1), tr.plane.normal);
					float invanglez = DotProduct(Vector(0, 0, -1), tr.plane.normal);
					float angley = DotProduct(Vector(0, 1, 0), tr.plane.normal);
					float invangley = DotProduct(Vector(0, -1, 0), tr.plane.normal);
					float anglex = DotProduct(Vector(1, 0, 0), tr.plane.normal);
					float invanglex = DotProduct(Vector(3 - 1, 0, 0), tr.plane.normal);
					float scale = tr.endpos.DistTo(LocalPlayer->GetOrigin()) / 60;

					Color color = Color(0, 0, 255);
					if (anglez > 0.5)
					{
						tr.endpos.z += 1;
						Vector startPos = tr.endpos + Vector(-scale, 0, 0);
						Vector endPos = tr.endpos + Vector(scale, 0, 0);
						Vector outStart, outEnd;
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							Render->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

						startPos = tr.endpos + Vector(0, -scale, 0);
						endPos = tr.endpos + Vector(0, scale, 0);
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							Render->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
					}
					else if (invanglez > 0.5)
					{
						tr.endpos.z += 1;
						Vector startPos = tr.endpos + Vector(-scale, 0, 0);
						Vector endPos = tr.endpos + Vector(scale, 0, 0);
						Vector outStart, outEnd;
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							Render->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

						startPos = tr.endpos + Vector(0, -scale, 0);
						endPos = tr.endpos + Vector(0, scale, 0);
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							Render->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
					}
					else if (angley > 0.5)
					{
						tr.endpos.y += 1;
						Vector startPos = tr.endpos + Vector(0, 0, -scale);
						Vector endPos = tr.endpos + Vector(0, 0, scale);
						Vector outStart, outEnd;
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							Render->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

						startPos = tr.endpos + Vector(-scale, 0, 0);
						endPos = tr.endpos + Vector(scale, 0, 0);
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							Render->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
					}
					else if (invangley > 0.5)
					{
						tr.endpos.y += 1;
						Vector startPos = tr.endpos + Vector(0, 0, -scale);
						Vector endPos = tr.endpos + Vector(0, 0, scale);
						Vector outStart, outEnd;
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							Render->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

						startPos = tr.endpos + Vector(-scale, 0, 0);
						endPos = tr.endpos + Vector(scale, 0, 0);
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							Render->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
					}
					else if (anglex > 0.5)
					{
						tr.endpos.x += 1;
						Vector startPos = tr.endpos + Vector(0, -scale, 0);
						Vector endPos = tr.endpos + Vector(0, scale, 0);
						Vector outStart, outEnd;
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							Render->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

						startPos = tr.endpos + Vector(0, 0, -scale);
						endPos = tr.endpos + Vector(0, 0, scale);
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							Render->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
					}
					else if (invanglex > 0.5)
					{
						tr.endpos.x += 1;
						Vector startPos = tr.endpos + Vector(0, -scale, 0);
						Vector endPos = tr.endpos + Vector(0, scale, 0);
						Vector outStart, outEnd;
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							Render->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

						startPos = tr.endpos + Vector(0, 0, -scale);
						endPos = tr.endpos + Vector(0, 0, scale);
						if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
							Render->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
					}

					vThrow2 = tr.plane.normal * -2.0f * DotProduct(vThrow2, tr.plane.normal) + vThrow2;
					vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;

					iCollisions++;
					if (iCollisions > 2)
						break;

					vPos = vStart + vThrow2 * tr.fraction * fStep;
					fTime += (fStep * (1 - tr.fraction));
				}

				Vector vOutStart, vOutEnd;

				if (WorldToScreen(vStart, vOutStart) && WorldToScreen(vPos, vOutEnd))
					
					Render->DrawLine((int)vOutStart.x, (int)vOutStart.y, (int)vOutEnd.x, (int)vOutEnd.y, Color::Blue());

				vStart = vPos;
				vThrow2.z -= fGravity * tr.fraction * fStep;
			}
		}
	}
}


Color GetPlayerColor(CPlayer* pPlayer)
{
	if (pPlayer->bVisible)
	{		
			if (changvis == 1) return Color(int(settings.Visual.esp_Color_VTeam[0] * 255.f), int(settings.Visual.esp_Color_VTeam[1] * 255.f), int(settings.Visual.esp_Color_VTeam[2] * 255.f));
			else if (changvis == 2)	return  Color(int(settings.Visual.esp_Color_VEnemy[0] * 255.f), int(settings.Visual.esp_Color_VEnemy[1] * 255.f), int(settings.Visual.esp_Color_VEnemy[2] * 255.f));
	}
	else 
	{
		if (changvis == 1)	return Color(int(settings.Visual.esp_Color_Team[0] * 255.f), int(settings.Visual.esp_Color_Team[1] * 255.f), int(settings.Visual.esp_Color_Team[2] * 255.f));
		else if (changvis == 2) return Color(int(settings.Visual.esp_Color_Enemy[0] * 255.f), int(settings.Visual.esp_Color_Enemy[1] * 255.f), int(settings.Visual.esp_Color_Enemy[2] * 255.f));
	}
	return Color::White();
}


bool CEsp::CheckPlayerTeam(CPlayer* pPlayer, CBaseEntity* pBaseEntity)
{	
	if (settings.Visual.esp_me && (pBaseEntity == LocalPlayer))	{changvis = 3;return true;}
	else if (settings.Visual.esp_Enemy && pPlayer->Team != Players->GetLocal()->Team && !(pBaseEntity == LocalPlayer)){changvis = 2;return true;}
	else if (settings.Visual.esp_Team && pPlayer->Team == Players->GetLocal()->Team){changvis = 1;return true;}
	//else if (!settings.Visual.esp_Visible && !pPlayer->bVisible) return false;

	return false;
}

void CEsp::NightMode()
{
	static bool OldNightmode;

	if (OldNightmode != settings.Visual.esp_NightMode)
	{
		ConVar* r_DrawSpecificStaticProp;
		if (!r_DrawSpecificStaticProp)
			r_DrawSpecificStaticProp = Interfaces::GetConVar()->FindVar("r_DrawSpecificStaticProp");

		r_DrawSpecificStaticProp->SetValue(0);

		static auto sv_skyname = Interfaces::GetConVar()->FindVar("sv_skyname");

		for (MaterialHandle_t i = Interfaces::MaterialSystem()->FirstMaterial(); i != Interfaces::MaterialSystem()->InvalidMaterial(); i = Interfaces::MaterialSystem()->NextMaterial(i))
		{
			IMaterial *pMaterial = Interfaces::MaterialSystem()->GetMaterial(i);
			if (!pMaterial)
				continue;

			if (settings.Visual.esp_NightMode)
			{
				if (strstr(pMaterial->GetTextureGroupName(), "Model")) {
					pMaterial->ColorModulate(0.60, 0.60, 0.60);
				}

				if (strstr(pMaterial->GetTextureGroupName(), "World"))
				{
					sv_skyname->SetValue("sky_csgo_night02");
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT, false);
					pMaterial->ColorModulate(0.1, 0.1, 0.1);
				}

				if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
				{
					pMaterial->ColorModulate(0.3, 0.3, 0.3);
				}

			}
			else
			{
				if (strstr(pMaterial->GetTextureGroupName(), "Model")) {
					pMaterial->ColorModulate(1, 1, 1);
				}
				if ((strstr(pMaterial->GetTextureGroupName(), "World")) || strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
				{
					sv_skyname->SetValue("sky_csgo_night02");
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT, false);
					pMaterial->ColorModulate(1, 1, 1);
				}
			}
		}
		OldNightmode = settings.Visual.esp_NightMode;
	}
}

void CEsp::DrawHitmarker()
{
		if (settings.hitmarkerAlpha < 0.f)
			settings.hitmarkerAlpha = 0.f;
		else if (settings.hitmarkerAlpha > 0.f)
			settings.hitmarkerAlpha -= 0.01f;

		int W, H;
		Interfaces::Engine()->GetScreenSize(W, H);

		float r = settings.Misc.misc_HitMarkerColor[0] * 255.f;
		float g = settings.Misc.misc_HitMarkerColor[1] * 255.f;
		float b = settings.Misc.misc_HitMarkerColor[2] * 255.f;

		if (settings.hitmarkerAlpha > 0.f)
		{
			Render->DrawLine(W / 2 - 15, H / 2 - 15, W / 2 - 5, H / 2 - 5, Color(r, g, b, (settings.hitmarkerAlpha * 255.f)));
			Render->DrawLine(W / 2 - 15, H / 2 + 15, W / 2 - 5, H / 2 + 5, Color(r, g, b, (settings.hitmarkerAlpha * 255.f)));
			Render->DrawLine(W / 2 + 15, H / 2 - 15, W / 2 + 5, H / 2 - 5, Color(r, g, b, (settings.hitmarkerAlpha * 255.f)));
			Render->DrawLine(W / 2 + 15, H / 2 + 15, W / 2 + 5, H / 2 + 5, Color(r, g, b, (settings.hitmarkerAlpha * 255.f)));
		}
}

void CEsp::ThirdPerson()
{
	if (!LocalPlayer)
		return;

	if (settings.Visual.thirdperson_tp && !LocalPlayer->IsDead() )
	{
		Interfaces::g_pInput->m_vecCameraOffset.z = settings.Visual.thirdperson_dist;
		Interfaces::g_pInput->m_fCameraInThirdPerson = true;
	}
	else
	{
		Interfaces::g_pInput->m_fCameraInThirdPerson = false;
	}

	
}
void DrawBorderLines()
{
	if (!LocalPlayer) return;
	if (!LocalPlayer->GetActiveWeapon()) return;

	int screen_x;
	int screen_y;
	int center_x;
	int center_y;
	Interfaces::Engine()->GetScreenSize(screen_x, screen_y);
	Interfaces::Engine()->GetScreenSize(center_x, center_y);
	center_x /= 2; center_y /= 2;

	if (LocalPlayer->GetIsScoped())
	{
		Render->DrawLine(0, center_y, screen_x, center_y, Color(0, 0, 0, 255));
		Render->DrawLine(center_x, 0, center_x, screen_y, Color(0, 0, 0, 255));
		
	}
}

void CEsp::OnRender()
{
	if (settings.Visual.noscope)
		DrawBorderLines();

	if (settings.Visual.DamageIndicator)
		DrawDamageIndicator();

	if (settings.Visual.esp_GrenadePrediction)
		GrenadePrediction();

	if (settings.Misc.misc_HitMarker)
		DrawHitmarker();

	for (int EntIndex = 0; EntIndex < Interfaces::EntityList()->GetHighestEntityIndex(); EntIndex++)
	{
		CPlayer* pPlayer = Players->GetPlayer(EntIndex);
		CBaseEntity* pPlayerEntity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(EntIndex);

		if (!pPlayerEntity) continue;
		if (!pPlayer) continue;
		if (pPlayer->bUpdate && CheckPlayerTeam(pPlayer, pPlayerEntity))
		{
			if (settings.Visual.CHvisual[changvis].esp_Skeleton)
				DrawPlayerSkeleton(pPlayerEntity);

			DrawPlayerEsp(pPlayer);
		}

		Vector vEntScreen;

		if (WorldToScreen(pPlayerEntity->GetRenderOrigin(), vEntScreen) )
		{

			if (settings.Visual.esp_BombPlanted && (pPlayerEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CPlantedC4))
			{
				Render->Text((int)vEntScreen.x, (int)vEntScreen.y + 15, true, true, Color::Yellow(), "%0.2f", Esp->fC4Timer);

				Render->DrawOutlineBox((int)vEntScreen.x - 0, (int)vEntScreen.y - 0, 10, 10, Color::Purple());
				Render->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::Red(), "[C4 PLANTED]");
			}

			if (settings.Visual.esp_Bomb && (pPlayerEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CC4))
			{
				static float rainbow;
				rainbow += 0.005f;
				if (rainbow > 1.f) rainbow = 0.f;
				Render->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::FromHSB(rainbow, 1.f, 1.f),
					"[C4]");
			}

		}
	}
}

void MsgFunc_ServerRankRevealAll()
{
	using tServerRankRevealAllFn = bool(__cdecl*)(int*);
	static tServerRankRevealAllFn ServerRankRevealAll = 0;

	if (!ServerRankRevealAll)
	{
		ServerRankRevealAll = (tServerRankRevealAllFn)(
			CSX::Memory::FindPattern(CLIENT_DLL,
				"55 8B EC 8B 0D ? ? ? ? 85 C9 75 28 A1 ? ? ? ? 68 ? ? ? ? 8B 08 8B 01 FF 50 04 85 C0 74 0B 8B C8 E8 ? ? ? ? 8B C8 EB 02 33 C9 89 0D ? ? ? ? 8B 45 08", 0));
	}

	if (ServerRankRevealAll)
	{
		int fArray[3] = { 0,0,0 };
		ServerRankRevealAll(fArray);
	}
}

void CEsp::OnCreateMove(CUserCmd* pCmd)
{
	

	if (settings.Visual.esp_Rank && pCmd->buttons & IN_SCORE)
		MsgFunc_ServerRankRevealAll();

}

void CEsp::OnReset()
{
	if (settings.Visual.esp_BombTimer)
	{
		bC4Timer = false;
		iC4Timer = settings.Visual.esp_BombTimer;
	}
}

void CEsp::OnEvents(IGameEvent* pEvent)
{
	
	if (Esp && settings.Visual.esp_BombTimer)
	{
		if (!strcmp(pEvent->GetName(), "bomb_defused") || !strcmp(pEvent->GetName(), "bomb_exploded"))
		{
			bC4Timer = false;
		}
		else if (!strcmp(pEvent->GetName(), "bomb_planted"))
		{
			bC4Timer = true;
		}
	}

	if (settings.Visual.DamageIndicator)
	{
		if (!LocalPlayer)
			return;

		if (strcmp(pEvent->GetName(), "player_hurt") == 0)
		{
			CBaseEntity* hurt = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(pEvent->GetInt("userid")));
			CBaseEntity* attacker = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetPlayerForUserID(pEvent->GetInt("attacker")));
		
			if (hurt != LocalPlayer && attacker == LocalPlayer)
			{
				DamageIndicator_t DmgIndicator;
				DmgIndicator.iDamage = pEvent->GetInt("dmg_health");
				DmgIndicator.Player = hurt;
				DmgIndicator.flEraseTime = LocalPlayer->GetTickBase() * Interfaces::GlobalVars()->interval_per_tick + 3.f;
				DmgIndicator.bInitialized = false;

				DamageIndicator.push_back(DmgIndicator);
			}
		}
	}

	
}


void CEsp::OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{

	if (!Players || Interfaces::Engine()->IsTakingScreenshot() || !Interfaces::Engine()->IsConnected() || !pInfo.pModel)
		return;

	SDK::IMaterial *material = nullptr;

	material = Interfaces::MaterialSystem()->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);

	if (!material)
		return;

	static bool InitalizeMaterial = false;

	if (!InitalizeMaterial)
	{
		
		visible_tex = CreateMaterial(false, false);

		hidden_tex = CreateMaterial(true, true);
		InitalizeMaterial = true;
		return;
	}

	string strModelName = Interfaces::ModelInfo()->GetModelName(pInfo.pModel);

	if (strModelName.size() <= 1)
		return;
	
	if (Players->GetLocal() && strModelName.find("models/player") != string::npos)
	{
		CBaseEntity* pBaseEntity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(pInfo.entity_index);

		if (!pBaseEntity)
			return;

		if (pBaseEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CCSPlayer)
		{
			CPlayer* pPlayer = Players->GetPlayer(pInfo.entity_index);
			if (pPlayer && pPlayer->bUpdate)
			{
				Color VisibleColor;
				Color HideColor;


				if (!settings.Visual.CHvisual[changvis].esp_Chams)
					return;



				if (changvis == 1)
				{
					HideColor = Color(int(settings.Visual.chams_Color_Team[0] * 255.f),
						int(settings.Visual.chams_Color_Team[1] * 255.f),
						int(settings.Visual.chams_Color_Team[2] * 255.f));

					VisibleColor = Color(int(settings.Visual.chams_Color_VTeam[0] * 255.f),
						int(settings.Visual.chams_Color_VTeam[1] * 255.f),
						int(settings.Visual.chams_Color_VTeam[2] * 255.f));
				}
				else if (changvis == 2 || changvis == 3)
				{
					HideColor = Color(int(settings.Visual.chams_Color_Enemy[0] * 255.f),
						int(settings.Visual.chams_Color_Enemy[1] * 255.f),
						int(settings.Visual.chams_Color_Enemy[2] * 255.f));

					VisibleColor = Color(int(settings.Visual.chams_Color_VEnemy[0] * 255.f),
						int(settings.Visual.chams_Color_VEnemy[1] * 255.f),
						int(settings.Visual.chams_Color_VEnemy[2] * 255.f));
				}



				if (CheckPlayerTeam(pPlayer, pBaseEntity))
				{
					if (settings.Visual.esp_Visible)
					{
						if (settings.Visual.CHvisual[changvis].esp_Chams == 1)
						{
							ForceMaterial(HideColor, material);
							material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						}
						else if (settings.Visual.CHvisual[changvis].esp_Chams == 2)
						{

							ForceMaterial(HideColor, hidden_tex);
							hidden_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						}
					}
					else
					{
						if (settings.Visual.CHvisual[changvis].esp_Chams == 1)
						{
							ForceMaterial(VisibleColor, material);
							material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						}
						else if (settings.Visual.CHvisual[changvis].esp_Chams == 2)
						{
							ForceMaterial(VisibleColor, visible_tex);
							visible_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						}
					}

					Interfaces::ModelRender()->DrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

					if (settings.Visual.CHvisual[changvis].esp_Chams == 1)
					{
						ForceMaterial(VisibleColor, material);
						material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					}
					else if (settings.Visual.CHvisual[changvis].esp_Chams == 2)
					{
						ForceMaterial(VisibleColor, visible_tex);
						visible_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					}

				}
			}
		}
		
	}
}

void CEsp::DrawPlayerEsp(CPlayer* pPlayer)
{
	bool bOriginScreen = (pPlayer->vOriginScreen.x > 0 && pPlayer->vOriginScreen.y > 0);
	bool bHitBoxScreen = (pPlayer->vHitboxHeadScreen.x > 0 && pPlayer->vHitboxHeadScreen.y > 0);

	if (!bOriginScreen && !bHitBoxScreen)
		return;

	Vector vLineOrigin;

	int Height = (int)pPlayer->vOriginScreen.y - (int)pPlayer->vHitboxHeadScreen.y;

	if (Height < 18)
		Height = 18;

	int Width = Height / 2;

	int x = (int)pPlayer->vHitboxHeadScreen.x - Width / 2;
	int y = (int)pPlayer->vHitboxHeadScreen.y;

	vLineOrigin = pPlayer->vHitboxHeadScreen;
	vLineOrigin.y += Height;


	Color EspColor = GetPlayerColor(pPlayer);


	if (settings.Visual.CHvisual[changvis].esp_Style == 1)  //Box
		Render->DrawBox(x, y, Width, Height, EspColor);

	else if (settings.Visual.CHvisual[changvis].esp_Style == 2)  //CoalBox
		Render->DrawCoalBox(x, y, Width, Height, EspColor);

	else if (settings.Visual.CHvisual[changvis].esp_Style == 3)
		Render->GenuineBox(x, y, Width, Height, EspColor);

	if (settings.Visual.CHvisual[changvis].esp_Name)
		Render->Text((int)vLineOrigin.x, (int)pPlayer->vHitboxHeadScreen.y - 13, true, true, EspColor, pPlayer->Name.c_str());


	int iHpAmY = 1;

	if (settings.Visual.CHvisual[changvis].esp_Health)
	{
		int iHealth = pPlayer->iHealth;

		if (iHealth)
		{
			if (settings.Visual.CHvisual[changvis].esp_Health == 1) //Number
			{
				Render->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, Color::LawnGreen(), to_string(iHealth).c_str());
				iHpAmY += 10;
			}
			else if (settings.Visual.CHvisual[changvis].esp_Health == 2) //Bottom
			{
				Render->DrawHorBar(x, (int)vLineOrigin.y + iHpAmY, Width, 5, iHealth, Color::LawnGreen(), Color::Red());
				iHpAmY += 6;
			}
			else if (settings.Visual.CHvisual[changvis].esp_Health == 3) //Left
			{
				Render->DrawVerBar(x - 6, (int)pPlayer->vHitboxHeadScreen.y, 5, Height, iHealth, Color::LawnGreen(), Color::Red());
			}
			else if (settings.Visual.CHvisual[changvis].esp_Health == 4) //edgy
			{
				float flBoxes = std::ceil(pPlayer->iHealth / 10.f);
				float flX = x - 6;
				float flY = y - 1;
				float flHeight = Height / 10.f;
				float flMultiplier = 12 / 360.f;
				flMultiplier *= flBoxes - 1;
				Color ColHealth = Color::FromHSB(flMultiplier, 1, 1);

				Render->DrawBox(flX, flY, 4, Height + 2, Color(80, 80, 80, 125));
				Render->DrawOutlineBox(flX, flY, 4, Height + 2, Color::Black());
				Render->DrawBox(flX + 1, flY, 2, flHeight * flBoxes + 1, ColHealth);

				for (int i = 0; i < 10; i++)
				{
					Render->DrawLine(flX, flY + i * flHeight, flX + 4, flY + i * flHeight, Color::Black());
				}
			}
		}
	}

	if (settings.Visual.CHvisual[changvis].esp_Armor) //None
	{
		int iArmor = pPlayer->iArmor;

		if (iArmor)
		{
			if (settings.Visual.CHvisual[changvis].esp_Armor == 1) //Number
			{
				Render->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, EspColor, to_string(iArmor).c_str());
				iHpAmY += 10;
			}
			if (settings.Visual.CHvisual[changvis].esp_Armor == 2) //Bottom
			{
				Render->DrawHorBar(x, (int)vLineOrigin.y + iHpAmY, Width, 5, iArmor, Color::White(), Color::DarkBlue());
				iHpAmY += 6;
			}
			else if (settings.Visual.CHvisual[changvis].esp_Armor == 3) //Right
			{
				Render->DrawVerBar(x + Width + 1, (int)pPlayer->vHitboxHeadScreen.y, 5, Height, iArmor, Color::Cyan(), Color::LightSkyBlue());
			}
			else if (settings.Visual.CHvisual[changvis].esp_Armor == 4) //Edgy
			{
				float flBoxes = std::ceil(pPlayer->iArmor / 10.f);
				float flX = x + Width + 1;
				float flY = y - 1;
				float flHeight = Height / 10.f;
				float flMultiplier = 12 / 360.f;
				flMultiplier *= flBoxes - 1;
				Color ColArmor = Color::White();

				Render->DrawBox(flX, flY, 4, Height + 2, Color::White());
				Render->DrawOutlineBox(flX, flY, 4, Height + 2, Color::Black());
				Render->DrawBox(flX + 1, flY, 2, flHeight * flBoxes + 1, ColArmor);

				for (int i = 0; i < 10; i++)
				{
					Render->DrawLine(flX, flY + i * flHeight, flX + 4, flY + i * flHeight, Color::Black());
				}
			}
		}
	}


	if (pPlayer->m_pEntity->IsDefusing() && settings.Visual.esp_Defusing)
		Render->Text((int)vLineOrigin.x, (int)vLineOrigin.y + 15, true, true, EspColor, ("Defusing"));

	if (pPlayer->m_pEntity->GetIsScoped() && settings.Visual.CHvisual[changvis].esp_Scoped)
		Render->Text((int)vLineOrigin.x, (int)vLineOrigin.y + 15, true, true, EspColor, ("Scoped"));

	if (pPlayer->m_pEntity->IsFlashed() && settings.Visual.CHvisual[changvis].esp_Flashed)
		Render->Text((int)vLineOrigin.x, (int)vLineOrigin.y + 15, true, true, EspColor, ("Flashed"));

}

void CEsp::DrawPlayerSkeleton(CBaseEntity* Entity)
{
	if (!Entity->GetModel()) return;
	studiohdr_t* pStudioHdr = Interfaces::ModelInfo()->GetStudioModel(Entity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->pBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = Entity->GetBonePosition(j);
			vParent = Entity->GetBonePosition(pBone->parent);

			if (WorldToScreen(vParent, sParent) && WorldToScreen(vChild, sChild)) 
				Render->DrawLine(sParent.x, sParent.y, sChild.x, sChild.y, Color(255, 255, 255, 200));		
		}
	}
}




CEsp* Esp = new CEsp;