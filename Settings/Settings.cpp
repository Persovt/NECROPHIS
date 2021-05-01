#include "Settings.h"
CSettings settings;





void SaveSettings(string szIniFile)
{
	CSX::Cvar::InitPath(szIniFile.c_str());

	CSX::Cvar::SaveCvar(AIMBOT_TEXT, "LegitBot Active", settings.LegitBot.aim_Active);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, "LegitBot Deathmath", settings.LegitBot.aim_Deathmatch);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, "LegitBot Draw Fov", settings.LegitBot.aim_DrawFov);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, "LegitBot checksmoke", settings.LegitBot.aim_CheckSmoke);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, "LegitBot cheackflash", settings.LegitBot.aim_CheckFlash);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, "LegitBot AntiJump", settings.LegitBot.aim_AntiJump);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, "LegitBot RcsType", settings.LegitBot.aim_RcsType);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, "LegitBot Delay", settings.LegitBot.aim_Delay);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, "LegitBot kill delay time", settings.LegitBot.aim_KillDelayTime);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, "LegitBot autopistol", settings.LegitBot.aim_AutoPistol);

	for (DWORD i = 0; i < 5; i++)
	{
		CSX::Cvar::SaveCvar(pWeaponData[i], "LegitBot Active", settings.LegitBot.weapon_aim_settings[i].aim_Active);
		CSX::Cvar::SaveCvar(pWeaponData[i], "LegitBot smoth", settings.LegitBot.weapon_aim_settings[i].aim_Smooth);
		CSX::Cvar::SaveCvar(pWeaponData[i], "LegitBot Fov", settings.LegitBot.weapon_aim_settings[i].aim_Fov);
		CSX::Cvar::SaveCvar(pWeaponData[i], "LegitBot fov type", settings.LegitBot.weapon_aim_settings[i].aim_FovType);
		CSX::Cvar::SaveCvar(pWeaponData[i], "LegitBot Besthit", settings.LegitBot.weapon_aim_settings[i].aim_BestHit);
		CSX::Cvar::SaveCvar(pWeaponData[i], "LegitBot Spot", settings.LegitBot.weapon_aim_settings[i].aim_Spot);
		CSX::Cvar::SaveCvar(pWeaponData[i], "LegitBot RCS type X Y", settings.LegitBot.weapon_aim_settings[i].aim_RCSType);
		CSX::Cvar::SaveCvar(pWeaponData[i], "LegitBot RCS", settings.LegitBot.weapon_aim_settings[i].aim_Rcs);
		CSX::Cvar::SaveCvar(pWeaponData[i], "LegitBot RCS X", settings.LegitBot.weapon_aim_settings[i].aim_Rcsx);
		CSX::Cvar::SaveCvar(pWeaponData[i], "LegitBot RCS Y", settings.LegitBot.weapon_aim_settings[i].aim_Rcsy);

		if (i == 3)
			CSX::Cvar::SaveCvar(pWeaponData[i], "LegitBot rcs clamp type", settings.LegitBot.weapon_aim_settings[i].aim_RcsClampType);
	}

	for (int i = 1; i < 3; i++)
	{
		char* vischname[3] = { "vis1","vis2","vis3" };
		CSX::Cvar::SaveCvar(vischname[i], "Visual esp_Health", settings.Visual.CHvisual[i].esp_Health);
		CSX::Cvar::SaveCvar(vischname[i], "Visual esp_Armor", settings.Visual.CHvisual[i].esp_Armor);
		CSX::Cvar::SaveCvar(vischname[i], "Visual esp_Style", settings.Visual.CHvisual[i].esp_Style);
		CSX::Cvar::SaveCvar(vischname[i], "Visual esp_Chams", settings.Visual.CHvisual[i].esp_Chams);
		CSX::Cvar::SaveCvar(vischname[i], "Visual esp_Name", settings.Visual.CHvisual[i].esp_Name);
		CSX::Cvar::SaveCvar(vischname[i], "Visual esp_Skeleton", settings.Visual.CHvisual[i].esp_Skeleton);
		CSX::Cvar::SaveCvar(vischname[i], "Visual esp_Scoped", settings.Visual.CHvisual[i].esp_Scoped);
		CSX::Cvar::SaveCvar(vischname[i], "Visual esp_Flashed", settings.Visual.CHvisual[i].esp_Flashed);
	}

	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual esp_Team", settings.Visual.esp_Team);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual esp_Enemy", settings.Visual.esp_Enemy);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual esp_me", settings.Visual.esp_me);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual esp_Visible", settings.Visual.esp_Visible);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual misc_NoFlash", settings.Misc.misc_NoFlash);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual noscope", settings.Visual.noscope);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual misc_NoSmoke", settings.Misc.misc_NoSmoke);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual DamageIndicator", settings.Visual.DamageIndicator);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual esp_GrenadePrediction", settings.Visual.esp_GrenadePrediction);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual esp_Defusing", settings.Visual.esp_Defusing);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual esp_BombPlanted", settings.Visual.esp_BombPlanted);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual esp_WorldWeapons", settings.Visual.esp_WorldWeapons);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual esp_WorldGrenade", settings.Visual.esp_WorldGrenade);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual thirdperson", settings.Visual.thirdperson);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual thirdperson_dist", settings.Visual.thirdperson_dist);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual misc_FovChanger", settings.Misc.misc_FovChanger);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual misc_FovView", settings.Misc.misc_FovView);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual misc_FovModelView", settings.Misc.misc_FovModelView);
	CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visual esp_Bomb", settings.Visual.esp_Bomb);



	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot aimbot_enabled", settings.RageBot.aimbot_enabled);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot autostop_mode", settings.RageBot.autostop_mode);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot stop_inbetween_shots", settings.RageBot.stop_inbetween_shots);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot auto_scope", settings.RageBot.auto_scope);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot aimbot_resolver", settings.RageBot.aimbot_resolver);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot bodyaim_mode", settings.RageBot.bodyaim_mode);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot bodyaim_shots", settings.RageBot.bodyaim_shots);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot bodyaim_health", settings.RageBot.bodyaim_health);
	for (int i; i <= 6; i++)
	{
		CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot rage_hitscan_hitboxes", settings.RageBot.rage_hitscan_hitboxes[i]);
		CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot prefer_bodyaim", settings.RageBot.prefer_bodyaim[i]);
	}
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot antiaim_enabled", settings.RageBot.antiaim_enabled);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot freestand", settings.RageBot.freestand);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot desync", settings.RageBot.desync);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot Pitch_stand", settings.RageBot.Pitch_stand);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot Yaw_stand", settings.RageBot.Yaw_stand);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot Pitch_move", settings.RageBot.Pitch_move);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot Yaw_move", settings.RageBot.Yaw_move);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot Pitch_air", settings.RageBot.Pitch_air);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot Yaw_air", settings.RageBot.Yaw_air);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot aimbot_headpointscale", settings.RageBot.aimbot_headpointscale);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot aimbot_bodypointscale", settings.RageBot.aimbot_bodypointscale);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot aimbot_ignorelimbs", settings.RageBot.aimbot_ignorelimbs);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot auto_hitchance", settings.RageBot.auto_hitchance);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot scout_hitchance", settings.RageBot.scout_hitchance);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot awp_hitchance", settings.RageBot.awp_hitchance);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot heavy_pistol_hitchance", settings.RageBot.heavy_pistol_hitchance);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot other_hitchance", settings.RageBot.other_hitchance);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot auto_mindamage", settings.RageBot.auto_mindamage);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot scout_mindamage", settings.RageBot.scout_mindamage);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot awp_mindamage", settings.RageBot.awp_mindamage);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot heavy_pistol_mindamage", settings.RageBot.heavy_pistol_mindamage);
	CSX::Cvar::SaveCvar(RAGEBOT_TEXT, "RageBot other_mindamage", settings.RageBot.other_mindamage);

}

void LoadSettings(string szIniFile)
{
	CSX::Cvar::InitPath(szIniFile.c_str());

	settings.LegitBot.aim_Active = CSX::Cvar::LoadCvar(AIMBOT_TEXT, "LegitBot Active", settings.LegitBot.aim_Active);
	settings.LegitBot.aim_DrawFov = CSX::Cvar::LoadCvar(AIMBOT_TEXT, "LegitBot Draw Fov", settings.LegitBot.aim_DrawFov);
	settings.LegitBot.aim_Deathmatch = CSX::Cvar::LoadCvar(AIMBOT_TEXT, "LegitBot Deathmath", settings.LegitBot.aim_Deathmatch);
	settings.LegitBot.aim_CheckSmoke = CSX::Cvar::LoadCvar(AIMBOT_TEXT, "LegitBot checksmoke", settings.LegitBot.aim_CheckSmoke);
	settings.LegitBot.aim_CheckFlash = CSX::Cvar::LoadCvar(AIMBOT_TEXT, "LegitBot cheackflash", settings.LegitBot.aim_CheckFlash);
	settings.LegitBot.aim_AntiJump = CSX::Cvar::LoadCvar(AIMBOT_TEXT, "LegitBot AntiJump", settings.LegitBot.aim_AntiJump);
	settings.LegitBot.aim_RcsType = CSX::Cvar::LoadCvar(AIMBOT_TEXT, "LegitBot RcsType", settings.LegitBot.aim_RcsType);
	settings.LegitBot.aim_Delay = CSX::Cvar::LoadCvar(AIMBOT_TEXT, "LegitBot Delay", settings.LegitBot.aim_Delay);
	settings.LegitBot.aim_KillDelay = CSX::Cvar::LoadCvar(AIMBOT_TEXT, "LegitBot kill delay", settings.LegitBot.aim_KillDelay);
	settings.LegitBot.aim_KillDelayTime = CSX::Cvar::LoadCvar(AIMBOT_TEXT, "LegitBot kill delay time", settings.LegitBot.aim_KillDelayTime);
	settings.LegitBot.aim_AutoPistol = CSX::Cvar::LoadCvar(AIMBOT_TEXT, "LegitBot autopistol", settings.LegitBot.aim_AutoPistol);
	for (DWORD i = 0; i < 5; i++)
	{
			settings.LegitBot.weapon_aim_settings[i].aim_Active = CSX::Cvar::LoadCvar(pWeaponData[i], "LegitBot_Active", 0);
			settings.LegitBot.weapon_aim_settings[i].aim_Smooth = CSX::Cvar::LoadCvar(pWeaponData[i], "LegitBot smoth", 50);
			settings.LegitBot.weapon_aim_settings[i].aim_Fov = CSX::Cvar::LoadCvar(pWeaponData[i], "LegitBot Fov", 35);
			settings.LegitBot.weapon_aim_settings[i].aim_FovType = CSX::Cvar::LoadCvar(pWeaponData[i], "LegitBot fov type", 1);
			settings.LegitBot.weapon_aim_settings[i].aim_BestHit = CSX::Cvar::LoadCvar(pWeaponData[i], "LegitBot Besthit", 0);
			settings.LegitBot.weapon_aim_settings[i].aim_Spot = CSX::Cvar::LoadCvar(pWeaponData[i], "LegitBot Spot", 5);
			settings.LegitBot.weapon_aim_settings[i].aim_RCSType = CSX::Cvar::LoadCvar(pWeaponData[i], "LegitBot RCS type X Y", 0);
			settings.LegitBot.weapon_aim_settings[i].aim_Rcs = CSX::Cvar::LoadCvar(pWeaponData[i], "LegitBot RCS", 50);
			settings.LegitBot.weapon_aim_settings[i].aim_Rcsx = CSX::Cvar::LoadCvar(pWeaponData[i], "LegitBot RCS X", 50);
			settings.LegitBot.weapon_aim_settings[i].aim_Rcsy = CSX::Cvar::LoadCvar(pWeaponData[i], "LegitBot RCS Y", 50);

		if (i == 3)
			settings.LegitBot.weapon_aim_settings[i].aim_RcsClampType = CSX::Cvar::LoadCvar(pWeaponData[i], "LegitBot rcs clamp type", 0);
	}


	for (int i = 1; i < 3; i++)
	{
		char* vischname[3] = { "vis1","vis2","vis3" };
		settings.Visual.CHvisual[i].esp_Health = CSX::Cvar::LoadCvar(vischname[i], "Visual esp_Health", settings.Visual.CHvisual[i].esp_Health);
		settings.Visual.CHvisual[i].esp_Armor = CSX::Cvar::LoadCvar(vischname[i], "Visual esp_Armor", settings.Visual.CHvisual[i].esp_Armor);
		settings.Visual.CHvisual[i].esp_Style = CSX::Cvar::LoadCvar(vischname[i], "Visual esp_Style", settings.Visual.CHvisual[i].esp_Style);
		settings.Visual.CHvisual[i].esp_Chams = CSX::Cvar::LoadCvar(vischname[i], "Visual esp_Chams", settings.Visual.CHvisual[i].esp_Chams);
		settings.Visual.CHvisual[i].esp_Name = CSX::Cvar::LoadCvar(vischname[i], "Visual esp_Name", settings.Visual.CHvisual[i].esp_Name);
		settings.Visual.CHvisual[i].esp_Skeleton = CSX::Cvar::LoadCvar(vischname[i], "Visual esp_Skeleton", settings.Visual.CHvisual[i].esp_Skeleton);
		settings.Visual.CHvisual[i].esp_Scoped = CSX::Cvar::LoadCvar(vischname[i], "Visual esp_Scoped", settings.Visual.CHvisual[i].esp_Scoped);
		settings.Visual.CHvisual[i].esp_Flashed = CSX::Cvar::LoadCvar(vischname[i], "Visual esp_Flashed", settings.Visual.CHvisual[i].esp_Flashed);
	}

	settings.Visual.esp_Team =  CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual esp_Team", settings.Visual.esp_Team);
	settings.Visual.esp_Enemy = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual esp_Enemy", settings.Visual.esp_Enemy);
	settings.Visual.esp_me = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual esp_me", settings.Visual.esp_me);
	settings.Visual.esp_Visible = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual esp_Visible", settings.Visual.esp_Visible);
	settings.Misc.misc_NoFlash = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual misc_NoFlash", settings.Misc.misc_NoFlash);
	settings.Visual.noscope = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual noscope", settings.Visual.noscope);
	settings.Misc.misc_NoSmoke = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual misc_NoSmoke", settings.Misc.misc_NoSmoke);
	settings.Visual.DamageIndicator = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual DamageIndicator", settings.Visual.DamageIndicator);
	settings.Visual.esp_GrenadePrediction = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual esp_GrenadePrediction", settings.Visual.esp_GrenadePrediction);
	settings.Visual.esp_Bomb = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual esp_Bomb", settings.Visual.esp_Bomb);
	settings.Visual.esp_Defusing = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual esp_Defusing", settings.Visual.esp_Defusing);
	settings.Visual.esp_BombPlanted = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual esp_BombPlanted", settings.Visual.esp_BombPlanted);
	settings.Visual.esp_WorldWeapons = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual esp_WorldWeapons", settings.Visual.esp_WorldWeapons);
	settings.Visual.esp_WorldGrenade = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual esp_WorldGrenade", settings.Visual.esp_WorldGrenade);
	settings.Visual.thirdperson = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual thirdperson", settings.Visual.thirdperson);
	settings.Visual.thirdperson_dist = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual thirdperson_dist", settings.Visual.thirdperson_dist);
	settings.Misc.misc_FovChanger = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual misc_FovChanger", settings.Misc.misc_FovChanger);
	settings.Misc.misc_FovView = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual misc_FovView", settings.Misc.misc_FovView);
	settings.Misc.misc_FovModelView = CSX::Cvar::LoadCvar(VISUAL_TEXT, "Visual misc_FovModelView", settings.Misc.misc_FovModelView);


	settings.RageBot.aimbot_enabled = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot aimbot_enabled", settings.RageBot.aimbot_enabled);
	settings.RageBot.autostop_mode = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot autostop_mode", settings.RageBot.autostop_mode);
	settings.RageBot.stop_inbetween_shots = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot stop_inbetween_shots", settings.RageBot.stop_inbetween_shots);
	settings.RageBot.auto_scope = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot auto_scope", settings.RageBot.auto_scope);
	settings.RageBot.aimbot_resolver = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot aimbot_resolver", settings.RageBot.aimbot_resolver);
	settings.RageBot.bodyaim_mode = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot bodyaim_mode", settings.RageBot.bodyaim_mode);
	settings.RageBot.bodyaim_shots = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot bodyaim_shots", settings.RageBot.bodyaim_shots);
	for (int i; i <= 6; i++)
	{
		settings.RageBot.rage_hitscan_hitboxes[i] = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "rage_hitscan_hitboxes", settings.RageBot.rage_hitscan_hitboxes[i]);
		settings.RageBot.prefer_bodyaim[i] = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot prefer_bodyaim", settings.RageBot.prefer_bodyaim[i]);
	}
	settings.RageBot.antiaim_enabled = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot antiaim_enabled", settings.RageBot.antiaim_enabled);
	settings.RageBot.freestand = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot freestand", settings.RageBot.freestand);
	settings.RageBot.desync = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot desync", settings.RageBot.desync);
	settings.RageBot.Pitch_stand = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot Pitch_stand", settings.RageBot.Pitch_stand);
	settings.RageBot.Yaw_stand = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot Yaw_stand", settings.RageBot.Yaw_stand);
	settings.RageBot.Pitch_move = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot Pitch_move", settings.RageBot.Pitch_move);
	settings.RageBot.Yaw_move = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot Yaw_move", settings.RageBot.Yaw_move);
	settings.RageBot.Pitch_air = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot Pitch_air", settings.RageBot.Pitch_air);
	settings.RageBot.Yaw_air = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot Yaw_air", settings.RageBot.Yaw_air);
	settings.RageBot.aimbot_headpointscale = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot aimbot_headpointscale", settings.RageBot.aimbot_headpointscale);
	settings.RageBot.aimbot_bodypointscale = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot aimbot_bodypointscale", settings.RageBot.aimbot_bodypointscale);
	settings.RageBot.aimbot_ignorelimbs = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot aimbot_ignorelimbs", settings.RageBot.aimbot_ignorelimbs);
	settings.RageBot.scout_hitchance = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot scout_hitchance", settings.RageBot.scout_hitchance);
	settings.RageBot.awp_hitchance = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot awp_hitchance", settings.RageBot.awp_hitchance);
	settings.RageBot.heavy_pistol_hitchance = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot heavy_pistol_hitchance", settings.RageBot.heavy_pistol_hitchance);
	settings.RageBot.other_hitchance = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot other_hitchance", settings.RageBot.other_hitchance);
	settings.RageBot.auto_mindamage = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot auto_mindamage", settings.RageBot.auto_mindamage);
	settings.RageBot.scout_mindamage = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot scout_mindamage", settings.RageBot.scout_mindamage);
	settings.RageBot.awp_mindamage = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot awp_mindamage", settings.RageBot.awp_mindamage);
	settings.RageBot.heavy_pistol_mindamage = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot heavy_pistol_mindamage", settings.RageBot.heavy_pistol_mindamage);
	settings.RageBot.other_mindamage = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot other_mindamage", settings.RageBot.other_mindamage);
	settings.RageBot.aimbot_enabled = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot aimbot_enabled", settings.RageBot.aimbot_enabled);
	settings.RageBot.aimbot_enabled = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot aimbot_enabled", settings.RageBot.aimbot_enabled);
	settings.RageBot.aimbot_enabled = CSX::Cvar::LoadCvar(RAGEBOT_TEXT, "RageBot aimbot_enabled", settings.RageBot.aimbot_enabled);
}