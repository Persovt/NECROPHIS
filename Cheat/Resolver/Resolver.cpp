#include "Resolver.h"
#define TICKS_TO_TIME( t )		( Interfaces::GlobalVars()->interval_per_tick *( t ) )
//using namespace Client;
Vector CalcAngle(Vector src, Vector dst)
{
	Vector ret;
	VectorAngles(dst - src, ret);
	return ret;
}
float NormalizeYaw(float yaw)
{
	if (yaw > 180)
	{
		yaw -= (round(yaw / 360) * 360.f);
	}
	else if (yaw < -180)
	{
		yaw += (round(yaw / 360) * -360.f);
	}
	return yaw;
}
void CResolver::Override()
{
	int w, h, cx, cy;

	Interfaces::Engine()->GetScreenSize(w, h);

	cx = w / 2;
	cy = h / 2;

	Vector crosshair = Vector(cx, cy, 0);
	CBaseEntity* LocalPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
	CBaseEntity * nearest_player = nullptr;
	float bestFoV = 0;
	Vector bestHead2D;
	
	if (!(Settings::Ragebot::ResolverOverride  && GetAsyncKeyState(Settings::Ragebot::OverrideKey)))
	{
		Settings::Ragebot::sg = false; return;
	}
	for (int i = 1; i <= Interfaces::GlobalVars()->maxClients; i++) 
	{

		auto pEnt = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(i);

		if (!pEnt) continue;

		if (pEnt == LocalPlayer) continue;

		if (pEnt->IsDead()) continue;
		
		if (pEnt->IsDormant()) continue;

		if (pEnt->GetTeam() == LocalPlayer->GetTeam()) continue;

		Vector viewangles; Interfaces::Engine()->GetViewAngles(viewangles);
		auto at_target_yaw = CalcAngle(pEnt->GetOrigin(), LocalPlayer->GetOrigin()).y;

		auto delta = NormalizeYaw(viewangles.y - at_target_yaw);
		auto rightDelta = Vector(pEnt->GetEyeAngles().x, at_target_yaw + 180, pEnt->GetEyeAngles().z);
		auto leftDelta = Vector(pEnt->GetEyeAngles().x, at_target_yaw - 180, pEnt->GetEyeAngles().z);

		if (delta > 0)
			pEnt->SetEyeAngles(rightDelta);
		else
			pEnt->SetEyeAngles(leftDelta);
	}

	if (nearest_player) 
	{
		
		int minX = cx - (w / 10), maxX = cx + (w / 10);

		if (bestHead2D.x < minX || bestHead2D.x > maxX)
			return;

		int totalWidth = maxX - minX;

		int playerX = bestHead2D.x - minX;

		int yawCorrection = -(((playerX * 360) / totalWidth) - 180);

		float_t new_yaw = yawCorrection;

		ClampYaw(new_yaw);

		nearest_player->GetEyeAnglesPtr()->y += new_yaw;
	}
	Settings::Ragebot::sg = true;
}


float YawDistance(float firstangle, float secondangle)
{
	if (firstangle == secondangle)
		return 0.f;

	bool oppositeSides = false;

	if (firstangle > 0 && secondangle < 0)
		oppositeSides = true;
	else if (firstangle < 0 && secondangle > 0)
		oppositeSides = true;

	if (!oppositeSides)
		return fabs(firstangle - secondangle);

	bool past90 = false;

	if (firstangle > 90 && secondangle < -90)
	{
		firstangle -= (firstangle - 90);
		secondangle += (secondangle + 90);
	}
	else if (firstangle < -90 && secondangle > 90)
	{
		firstangle += (firstangle + 90);
		secondangle -= (secondangle - 90);
	}

	float oneTwo;

	oneTwo = fabs(firstangle - secondangle);

	return oneTwo;
}

static void nospread_resolve(CBaseEntity* player, int entID) //gay but very effective reversed aw air resolver
{
	CBaseEntity* local_player = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	if (!local_player)
		return;

	Vector local_position = local_player->GetOrigin() + local_player->GetViewOffset();

	float atTargetAngle = CalcAngle(local_player->GetHealth() <= 0 ? local_player->GetOrigin() : local_position, player->GetOrigin()).y;
	Vector velocityAngle;
	VectorAngles(player->GetVelocity(), velocityAngle);

	float primaryBaseAngle = player->GetLowerBodyYaw();
	float secondaryBaseAngle = velocityAngle.y;

	switch (local_player->GetShotsFired() % 15)
	{
	case 0:
		player->GetEyeAnglesPtr()->y = atTargetAngle + 180.f;
		break;
	case 1:
		player->GetEyeAnglesPtr()->y = velocityAngle.y + 180.f;
		break;
	case 2:
		player->GetEyeAnglesPtr()->y = primaryBaseAngle;
		break;
	case 3:
		player->GetEyeAnglesPtr()->y = primaryBaseAngle - 45.f;
		break;
	case 4:
		player->GetEyeAnglesPtr()->y = primaryBaseAngle + 90.f;
		break;
	case 5:
		player->GetEyeAnglesPtr()->y = primaryBaseAngle - 130.f;
		break;
	case 6:
		player->GetEyeAnglesPtr()->y = primaryBaseAngle - 180.f;
		break;
	case 7:
		player->GetEyeAnglesPtr()->y = secondaryBaseAngle;
		break;
	case 8:
		player->GetEyeAnglesPtr()->y = secondaryBaseAngle - 40.f;
		break;
	case 9:
		player->GetEyeAnglesPtr()->y = secondaryBaseAngle - 90.f;
		break;
	case 10:
		player->GetEyeAnglesPtr()->y = secondaryBaseAngle - 130.f;
		break;
	case 11:
		player->GetEyeAnglesPtr()->y = secondaryBaseAngle - 70.f;
		break;
	case 12:
		player->GetEyeAnglesPtr()->y = primaryBaseAngle + 45.f;
		break;
	case 13:
		player->GetEyeAnglesPtr()->y = primaryBaseAngle + 135.f;
		break;
	case 14:
		player->GetEyeAnglesPtr()->y = primaryBaseAngle - 90.f;
		break;
	}
}
bool find_layer(CBaseEntity* entity, int act, CAnimationLayer *set)
{
	for (int i = 0; i < 13; i++)
	{
		CAnimationLayer layer = entity->GetAnimOverlay(i);
		const int activity = entity->GetSequenceActivity(layer.m_nSequence);
		if (activity == act) {
			*set = layer;
			return true;
		}
	}
	return false;
}

void CResolver::Resolver(CBaseEntity* entity)
{
	CBaseEntity* local_player = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	if (!entity)
		return;

	if (!local_player)
		return;

	bool is_local_player = entity == local_player;
	bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

	if (is_local_player)
		return;

	if (is_teammate)
		return;

	if (entity->GetHealth() <= 0)
		return;

	if (local_player->GetHealth() <= 0)
		return;

	//--- Variable Declaration ---//;
	auto &info = player_info[entity->GetIndex()];

	//--- Variable Defenitions/Checks ---//
	float fl_lby = entity->GetLowerBodyYaw();

	info.lby = Vector(entity->GetEyeAngles().x, entity->GetLowerBodyYaw(), 0.f);
	info.inverse = Vector(entity->GetEyeAngles().x, entity->GetLowerBodyYaw() + 180.f, 0.f);
	info.last_lby = Vector(entity->GetEyeAngles().x, info.last_moving_lby, 0.f);
	info.inverse_left = Vector(entity->GetEyeAngles().x, entity->GetLowerBodyYaw() + 115.f, 0.f);
	info.inverse_right = Vector(entity->GetEyeAngles().x, entity->GetLowerBodyYaw() - 115.f, 0.f);

	info.back = Vector(entity->GetEyeAngles().x, CalcAngle(entity->GetOrigin(), local_player->GetOrigin()).y + 180.f, 0.f);
	info.right = Vector(entity->GetEyeAngles().x, CalcAngle(entity->GetOrigin(), local_player->GetOrigin()).y + 70.f, 0.f);
	info.left = Vector(entity->GetEyeAngles().x, CalcAngle(entity->GetOrigin(), local_player->GetOrigin()).y - 70.f, 0.f);

	info.backtrack = Vector(entity->GetEyeAngles().x, lby_to_back[entity->GetIndex()], 0.f);

	shots_missed[entity->GetIndex()] = shots_fired[entity->GetIndex()] - shots_hit[entity->GetIndex()];
	info.is_moving = entity->GetVelocity().Length2D() > 0.1 && entity->GetFlags() & FL_ONGROUND && !info.could_be_slowmo;
	info.is_jumping = !entity->GetFlags() & FL_ONGROUND;
	info.could_be_slowmo = entity->GetVelocity().Length2D() > 6 && entity->GetVelocity().Length2D() < 36 && !info.is_crouching;
	info.is_crouching = entity->GetFlags() & FL_DUCKING;
	update_time[entity->GetIndex()] = info.next_lby_update_time;

	static float old_simtime[65];
	if (entity->GetSimTime() != old_simtime[entity->GetIndex()])
	{
		using_fake_angles[entity->GetIndex()] = entity->GetSimTime() - old_simtime[entity->GetIndex()] == Interfaces::GlobalVars()->interval_per_tick; entity->GetSimTime() - old_simtime[entity->GetIndex()] >= TICKS_TO_TIME(2);
		old_simtime[entity->GetIndex()] = entity->GetSimTime();
	}


	/*============*/
	
	/*===============*/
	//--- Actual Angle Resolving ---//
	if (!using_fake_angles[entity->GetIndex()])
	{
		if (backtrack_tick[entity->GetIndex()])
		{
			resolve_type[entity->GetIndex()] = 7;
			entity->SetEyeAngles(info.backtrack);
		}
		else if (info.stored_lby != entity->GetLowerBodyYaw()) // || entity->GetSimTime() > info.next_lby_update_time) lby prediction
		{
			entity->GetSimTime() > info.next_lby_update_time;
			entity->SetEyeAngles(info.lby);
			info.next_lby_update_time = entity->GetSimTime() + 1.1;
			info.stored_lby = entity->GetLowerBodyYaw();
			resolve_type[entity->GetIndex()] = 3;
		}
		else if (info.is_jumping)
		{
			nospread_resolve(entity, entity->GetIndex());
		}
		else if (info.is_moving) //while moving
		{
			entity->SetEyeAngles(info.lby);
			info.last_moving_lby = entity->GetLowerBodyYaw();
			info.stored_missed = shots_missed[entity->GetIndex()];
			resolve_type[entity->GetIndex()] = 1;
		}
		else
		{
			if (shots_missed[entity->GetIndex()] > info.stored_missed) //if we have missed 1 shot since we have stopped moving
			{
				resolve_type[entity->GetIndex()] = 4;
				switch (shots_missed[entity->GetIndex()] % 4)
				{
				case 0: entity->SetEyeAngles(info.inverse); break;
				case 1: entity->SetEyeAngles(info.left); break;
				case 2: entity->SetEyeAngles(info.back); break;
				case 3: entity->SetEyeAngles(info.right); break;
				}
			}
			else //first thing we shoot when they stop
			{
				entity->SetEyeAngles(info.last_lby);
				resolve_type[entity->GetIndex()] = 5;
			}
		}
	}


	SDK::CAnimationLayer layer = entity->GetAnimOverlay(0);
	if (entity->GetSimTime() != info.stored_simtime)
	{
		info.stored_simtime = entity->GetSimTime();
		info.prev_layer = info.backup_layer;
		SDK::CAnimationLayer dummy;
		info.backup_layer = find_layer(entity, 979, &dummy) ? dummy : layer;
	}

	SDK::CAnimationLayer prev = info.prev_layer;
	auto server_time = local_player->GetTickBase() * Interfaces::GlobalVars()->interval_per_tick; //i have a global dedicated to curtime but am using this because lemon is gay

	if (info.is_moving && !info.could_be_slowmo)
	{
		entity->SetEyeAngles(info.lby);
		info.last_moving_lby = entity->GetLowerBodyYaw();
		info.stored_missed = shots_missed[entity->GetIndex()];
		info.last_move_time = server_time;
		info.reset_state = true;
		resolve_type[entity->GetIndex()] = 1;
	}
	else
	{
		if (info.stored_lby != entity->GetLowerBodyYaw())
		{
			entity->SetEyeAngles(info.lby);
			info.stored_lby = entity->GetLowerBodyYaw();
			info.next_lby_update_time = entity->GetSimTime() + 1.1;
			resolve_type[entity->GetIndex()] = 7;
		}
		else if (server_time - info.last_move_time < 0.1 && info.reset_state)
		{
			info.pre_anim_lby = entity->GetLowerBodyYaw();
			info.reset_state = false;
			info.breaking_lby = false;
			
		}
		auto previous_is_valid = entity->GetSequenceActivity(prev.m_nSequence) == 979;

		if (info.unresolved_yaw.size() < 2 || info.lby_deltas.size() < 2)
			return;

		auto average_unresolved_yaw = 0;
		for (auto val : info.unresolved_yaw)
			average_unresolved_yaw += val;
		average_unresolved_yaw /= info.unresolved_yaw.size();

		auto average_lby_delta = 0;
		for (auto val : info.lby_deltas)
			average_lby_delta += val;
		average_lby_delta /= info.lby_deltas.size();

		int deltaxd = average_unresolved_yaw - entity->GetLowerBodyYaw();
		auto current_lby_delta = abs((((deltaxd + 180) % 360 + 360) % 360 - 180));

		int update_delta = info.pre_anim_lby - entity->GetLowerBodyYaw();
		auto lby_update_delta = abs((((update_delta + 180) % 360 + 360) % 360 - 180));

		if (find_layer(entity, 979, &layer)
			&& previous_is_valid
			&& (layer.m_flCycle != prev.m_flCycle
				|| layer.m_flWeight == 1.f
				|| server_time - info.last_move_time < 1.4
				&& !info.breaking_lby
				&& layer.m_flCycle >= 0.01
				&& lby_update_delta > 75))
		{
			if (server_time - info.last_move_time < 1.4)
			{
				info.breaking_lby = true;
				//std::cout << "breaking lby" << std::endl;
			}
			entity->SetEyeAngles(info.inverse);
			resolve_type[entity->GetIndex()] = 6;
		}
		else
		{
			if (info.breaking_lby)
			{
				if (current_lby_delta > 130 && average_lby_delta > 130) {
					entity->SetEyeAngles(info.lby);
					resolve_type[entity->GetIndex()] = 7;
				}
				else {
					if (info.next_lby_update_time < entity->GetSimTime())
					{
						entity->SetEyeAngles(info.lby);
						info.next_lby_update_time = entity->GetSimTime() + 1.1;
						resolve_type[entity->GetIndex()] = 3;
					}
					else if (info.is_moving)
					{
						resolve_type[entity->GetIndex()] = 5;
						switch (shots_missed[entity->GetIndex()] % 2)
						{
						case 0: entity->SetEyeAngles(info.last_lby); break;
						case 1: entity->SetEyeAngles(info.inverse); break;
						}
					}
					else
					{
						if (shots_missed[entity->GetIndex()] > info.stored_missed)
						{
							resolve_type[entity->GetIndex()] = 4;
							switch (shots_missed[entity->GetIndex()] % 3)
							{
							case 0: entity->SetEyeAngles(info.inverse); break; //180
							case 1: entity->SetEyeAngles(info.inverse_left); break; //115
							case 2: entity->SetEyeAngles(info.inverse_right); break; //-115
							}
						}
						//else
						//{
						//	resolve_type[entity->GetIndex()] = 2;
						//	entity->SetEyeAngles(info.last_lby);
						//}
					}
				}
			}
			else
			{
				entity->SetEyeAngles(info.lby);
				resolve_type[entity->GetIndex()] = 7;
			}
		}
	}
	if (info.stored_lby != fl_lby)
	{
		entity->SetEyeAngles(info.lby);
		info.stored_lby = fl_lby;
		info.next_lby_update_time = entity->GetSimTime() + 1.1;
		resolve_type[entity->GetIndex()] = 1;
	}
	else if (info.next_lby_update_time < entity->GetSimTime())
	{
		entity->SetEyeAngles(info.lby);
		info.next_lby_update_time = entity->GetSimTime() + 1.1;
		resolve_type[entity->GetIndex()] = 3;
	}
	else if (info.is_moving && !info.could_be_slowmo)
	{
		entity->SetEyeAngles(info.lby);
		info.last_moving_lby = fl_lby;
		info.stored_missed = shots_missed[entity->GetIndex()];
		Interfaces::GlobalVars()->curtime;
		resolve_type[entity->GetIndex()] = 1;
	}
	else
	{
		if (info.breaking_lby)
		{
			if (info.is_moving && !info.is_crouching)
			{
				resolve_type[entity->GetIndex()] = 5;
				switch (shots_missed[entity->GetIndex()] % 2)
				{
				case 0: entity->SetEyeAngles(info.last_lby); break;
				case 1: entity->SetEyeAngles(info.inverse); break;
				}
			}
			else {
				if (shots_missed[entity->GetIndex()] > info.stored_missed)
				{
					resolve_type[entity->GetIndex()] = 4;
					switch (shots_missed[entity->GetIndex()] % 4)
					{
					case 0: entity->SetEyeAngles(info.inverse); break;
					case 1: entity->SetEyeAngles(info.right); break;
					case 2: entity->SetEyeAngles(info.left); break;
					case 3: entity->SetEyeAngles(info.back); break;
					}
				}
				else
				{
					resolve_type[entity->GetIndex()] = 2;
					entity->SetEyeAngles(info.last_lby);
				}
			}
		}
		else
		{
			entity->SetEyeAngles(info.lby);
			resolve_type[entity->GetIndex()] = 1;
		}
	}



}


CResolver* Resolver = new CResolver;