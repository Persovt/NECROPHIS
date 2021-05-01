#include "Radar.h"

//using namespace Client;

//[junk_enable /]
void CRadar::RadarRange(float* x, float* y, float range)
{
	if (fabs((*x)) > range || fabs((*y)) > range)
	{
		if ((*y) > (*x))
		{
			if ((*y) > -(*x))
			{
				(*x) = range * (*x) / (*y);
				(*y) = range;
			}
			else
			{
				(*y) = -range * (*y) / (*x);
				(*x) = -range;
			}
		}
		else
		{
			if ((*y) > -(*x))
			{
				(*y) = range * (*y) / (*x);
				(*x) = range;
			}
			else
			{
				(*x) = -range * (*x) / (*y);
				(*y) = -range;
			}
		}
	}
}

void CRadar::CalcRadarPoint(Vector vOrigin, int& screenx, int& screeny)
{
	Vector vAngle;

	Interfaces::Engine()->GetViewAngles(vAngle);

	float dx = vOrigin.x - Players->GetLocal()->vEyeOrigin.x;
	float dy = vOrigin.y - Players->GetLocal()->vEyeOrigin.y;

	float fYaw = float(vAngle.y * (M_PI / 180.0));

	float fsin_yaw = sin(fYaw);
	float fminus_cos_yaw = -cos(fYaw);

	float x = dy * fminus_cos_yaw + dx * fsin_yaw;
	float y = dx * fminus_cos_yaw - dy * fsin_yaw;

	float range = (float)settings.Radar.rad_Range;

	RadarRange(&x, &y, range);

	ImVec2 DrawPos = ImGui::GetCursorScreenPos();
	ImVec2 DrawSize = ImGui::GetContentRegionAvail();

	int rad_x = (int)DrawPos.x;
	int rad_y = (int)DrawPos.y;

	float r_siz_x = DrawSize.x;
	float r_siz_y = DrawSize.y;

	int x_max = (int)r_siz_x + rad_x - 5;
	int y_max = (int)r_siz_y + rad_y - 5;

	screenx = rad_x + ((int)r_siz_x / 2 + int(x / range * r_siz_x));
	screeny = rad_y + ((int)r_siz_y / 2 + int(y / range * r_siz_y));

	if (screenx > x_max)
		screenx = x_max;

	if (screenx < rad_x)
		screenx = rad_x;

	if (screeny > y_max)
		screeny = y_max;

	if (screeny < rad_y)
		screeny = rad_y;
}

void CRadar::OnRenderPlayer()
{
	float Alpha = (float)settings.Radar.rad_Alpha / 255.f;

	for (BYTE PlayerIndex = 0; PlayerIndex < Players->GetSize(); PlayerIndex++)
	{
		CPlayer* pPlayer = Players->GetPlayer(PlayerIndex);

		if (pPlayer && pPlayer->m_pEntity && pPlayer->bUpdate)
		{

			if (settings.Radar.rad_InGame && GetKeyState(settings.Radar.rad_InGameKey))
			{
				if (*pPlayer->m_pEntity->IsSpotted() == false)
				{
					*pPlayer->m_pEntity->IsSpotted() = true;
				}
			}

			if (!settings.Radar.rad_Active)
				continue;

			if (!settings.Radar.rad_Team && pPlayer->Team == Players->GetLocal()->Team)
				continue;

			if (!settings.Radar.rad_Enemy && pPlayer->Team != Players->GetLocal()->Team)
				continue;

			int screenx = 0;
			int screeny = 0;

			float Color_R = 0.f;
			float Color_G = 0.f;
			float Color_B = 0.f;

			if (pPlayer->Team == TEAM_CT)
			{
				Color_R = settings.Radar.rad_Color_CT[0];
				Color_G = settings.Radar.rad_Color_CT[1];
				Color_B = settings.Radar.rad_Color_CT[2];

				if (pPlayer->bVisible)
				{
					Color_R = settings.Radar.rad_Color_VCT[0];
					Color_G = settings.Radar.rad_Color_VCT[1];
					Color_B = settings.Radar.rad_Color_VCT[2];
				}
			}
			else if (pPlayer->Team == TEAM_TT)
			{
				Color_R = settings.Radar.rad_Color_TT[0];
				Color_G = settings.Radar.rad_Color_TT[1];
				Color_B = settings.Radar.rad_Color_TT[2];

				if (pPlayer->bVisible)
				{
					Color_R = settings.Radar.rad_Color_VTT[0];
					Color_G = settings.Radar.rad_Color_VTT[1];
					Color_B = settings.Radar.rad_Color_VTT[2];
				}
			}

			CalcRadarPoint(pPlayer->vOrigin, screenx, screeny);

			ImDrawList* Draw = ImGui::GetWindowDrawList();

			Draw->AddRectFilled(ImVec2((float)screenx, (float)screeny),
				ImVec2((float)screenx + 5, (float)screeny + 5),
				ImColor(Color_R, Color_G, Color_B, Alpha));
		}
	}
}

void CRadar::OnRender()
{
	if (settings.Radar.rad_Active && IsLocalAlive())
	{
		float prevAlpha = ImGui::GetStyle().Alpha;

		float Alpha = (float)settings.Radar.rad_Alpha / 255.f;

		ImGui::GetStyle().Alpha = Alpha;

		if (ImGui::Begin(RADAR_TEXT, &settings.Radar.rad_Active, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
		{
			ImDrawList* Draw = ImGui::GetWindowDrawList();

			ImVec2 DrawPos = ImGui::GetCursorScreenPos();
			ImVec2 DrawSize = ImGui::GetContentRegionAvail();

			Draw->AddLine(ImVec2(DrawPos.x + DrawSize.x / 2.f, DrawPos.y),ImVec2(DrawPos.x + DrawSize.x / 2.f, DrawPos.y + DrawSize.y),ImColor(1.f, 1.f, 1.f, Alpha));

			Draw->AddLine(ImVec2(DrawPos.x, DrawPos.y + DrawSize.y / 2.f),ImVec2(DrawPos.x + DrawSize.x, DrawPos.y + DrawSize.y / 2.f),ImColor(1.f, 1.f, 1.f, Alpha));

			OnRenderPlayer();

			ImGui::End();
		}

		ImGui::GetStyle().Alpha = prevAlpha;
	}
	if (settings.Radar.rad_InGame && GetKeyState(settings.Radar.rad_InGameKey))
	{
		if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
			OnRenderPlayer();
		
	}
}

CRadar*	Radar = new CRadar;
