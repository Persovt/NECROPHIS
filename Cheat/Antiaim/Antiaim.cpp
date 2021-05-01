#include "Antiaim.h"
#pragma warning(disable:4244)

//using namespace Client;

CAntiAim::CAntiAim()
{
	CMe* m_pLocal;
}
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
#define M_PHI 1.618033988749895
#define Square( x ) ( x * x )
#define FastSqrt(x)	(sqrt)(x)
#define PI 3.14159265358979323846f

enum MovingAAType {
	Standing = 0,
	Moving,
	Air
};








int ReturnMovementType() {

	if (LocalPlayer->GetFlags() & FL_ONGROUND)
	{
		if (LocalPlayer->GetVelocity().Length2D() < 32)
			return MovingAAType::Standing;
	
		else if (LocalPlayer->GetVelocity().Length2D() > 32)
			return MovingAAType::Moving;
	}
	else
		return MovingAAType::Air;	
}
void VectorSubtract1(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}
void NormalizeNum(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}
float fov_player(Vector ViewOffSet, Vector View, CBaseEntity* entity, int hitbox)
{

	// Anything past 180 degrees is just going to wrap around
	CONST FLOAT MaxDegrees = 180.0f;

	// Get local angles
	Vector Angles = View;

	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	//Vector Origin(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	Math::AngleVectors(Angles, &Forward,0,0);

	Vector AimPos = LocalPlayer->GetBonePosition(8);

	VectorSubtract1(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	NormalizeNum(Delta, Delta);
	//Math::NormalizeAngle(Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / PI));
}

int closest_to_crosshair()
{
	int index = -1;
	float lowest_fov = INT_MAX;


	if (!LocalPlayer)
		return -1;

	Vector local_position = LocalPlayer->GetOrigin() + LocalPlayer->GetViewOffset();

	Vector angles;
	Interfaces::Engine()->GetViewAngles(angles);

	for (int i = 1; i <= Interfaces::GlobalVars()->maxClients; i++)
	{
		CBaseEntity* entity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(i);
		

		if (!entity || entity->IsDead() || entity->GetTeam() == LocalPlayer->GetTeam() || entity->IsDormant() || entity == LocalPlayer)
			continue;

		float fov = fov_player(local_position, angles, entity, 0);

		if (fov < lowest_fov)
		{
			lowest_fov = fov;
			index = i;
		}
	}

	return index;
}

void freestanding(CUserCmd* pCmd)
{
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	Vector besthead;
	
	auto leyepos = LocalPlayer->GetOrigin() + LocalPlayer->GetViewOffset();
	auto headpos = LocalPlayer->GetBonePosition(8);
	auto origin = LocalPlayer->GetOrigin();

	auto checkWallThickness = [&](CBaseEntity* pPlayer, Vector newhead) -> float
	{
		Vector endpos1, endpos2;
		Vector eyepos = pPlayer->GetOrigin() + pPlayer->GetViewOffset();

		Ray_t ray;
		ray.Init(newhead, eyepos);

		CTraceFilter filter(pPlayer);

		trace_t trace1, trace2;
		Interfaces::EngineTrace()->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

		if (trace1.DidHit())
			endpos1 = trace1.endpos;
		else
			return 0.f;

		ray.Init(eyepos, newhead);
		Interfaces::EngineTrace()->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

		if (trace2.DidHit())
			endpos2 = trace2.endpos;

		float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) + 3.f;
		return endpos1.DistTo(endpos2) + add / 3;
	};

	int index = closest_to_crosshair();

	CBaseEntity* entity;

	if (index != -1)
		entity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(index);
		

	float step = (2 * PI) / 17.f; // One PI = half a circle ( for stacker cause low iq :sunglasses: ), 28

	float radius = fabs(Vector(headpos - origin).Length2D());

	if (index == -1)
	{
		no_active = true;
	}
	else
	{
		for (float rotation = 0; rotation < (PI * 2.0); rotation += step)
		{
			Vector newhead(radius * cos(rotation) + leyepos.x, radius * sin(rotation) + leyepos.y, leyepos.z);

			float totalthickness = 0.f;

			no_active = false;

			totalthickness += checkWallThickness(entity, newhead);

			if (totalthickness > highestthickness)
			{
				highestthickness = totalthickness;
				bestrotation = rotation;
				besthead = newhead;
			}
		}
	}

	if (no_active)
		pCmd->viewangles.y += 180.f;
	else
		pCmd->viewangles.y = RAD2DEG(bestrotation);
}



float freestand() {
	enum {
		back,
		right,
		left
	};
	
	Vector view_angles;
	Interfaces::Engine()->GetViewAngles(view_angles);

	static constexpr int damage_tolerance = 30;

	std::vector< CBaseEntity * > enemies;

	auto get_target = [&]() -> CBaseEntity * {
		CBaseEntity * target = nullptr;
		float best_fov = 360.f;

		for (int id = 1; id <= Interfaces::GlobalVars()->maxClients; id++) {
			
			CBaseEntity* e = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(id);

			if (!e || e->IsDead() )
				continue;

			float fov = Math::GetFov(view_angles, Math::calculate_angle(LocalPlayer->GetEyePos(), e->GetOrigin()));

			if (fov < best_fov) {
				target = e;
				best_fov = fov;
			}

			enemies.push_back(e);
		}

		return target;
	};

	CBaseEntity * e = get_target();

	if (!e)
		return view_angles.y + 180;

	

	auto rotate_and_extend = [](Vector position, float yaw, float distance) -> Vector {
		Vector direction;
		Math::AngleVectors(Vector(0, yaw, 0), &direction,0,0);

		return position + (direction * distance);
	};

	Vector
		head_position = LocalPlayer->GetEyePos(),
		at_target = Math::calculate_angle(LocalPlayer->GetOrigin(), e->GetOrigin());

	float angles[3] = {
		at_target.y + 180.f,
		at_target.y - 70.f,
		at_target.y + 70.f
	};

	Vector head_positions[3] = {
		rotate_and_extend(head_position, at_target.y + 180.f, 17.f),
		rotate_and_extend(head_position, at_target.y - 70.f, 17.f),
		rotate_and_extend(head_position, at_target.y + 70.f, 17.f)
	};

	int damages[3] = {
		Autowall->GetDamage(head_positions[back]),
		Autowall->GetDamage(head_positions[right]),
		Autowall->GetDamage(head_positions[left])
	};

	if (damages[right] > damage_tolerance && damages[left] > damage_tolerance)
		return angles[back];

	if (at_target.x > 15.f)
		return angles[back];

	if (damages[right] == damages[left]) {
		auto trace_to_end = [](Vector start, Vector end) -> Vector {
			trace_t trace;
			CTraceFilterWorldOnly filter;
			Ray_t ray;

			ray.Init(start, end);
			Interfaces::EngineTrace()->TraceRay(ray, MASK_ALL, &filter, &trace);

			return trace.endpos;
		};

		Vector
			trace_right_endpos = trace_to_end(head_position, head_positions[right]),
			trace_left_endpos = trace_to_end(head_position, head_positions[left]);

		Ray_t ray;
		trace_t trace;
		CTraceFilterWorldOnly filter;

		ray.Init(trace_right_endpos, e->GetEyePos());
		Interfaces::EngineTrace()->TraceRay(ray, MASK_ALL, &filter, &trace);
		float distance_1 = (trace.startpos - trace.endpos).Length();

		ray.Init(trace_left_endpos, e->GetEyePos());
		Interfaces::EngineTrace()->TraceRay(ray, MASK_ALL, &filter, &trace);
		float distance_2 = (trace.startpos - trace.endpos).Length();

		if (fabs(distance_1 - distance_2) > 15.f)
			return (distance_1 < distance_2) ? angles[right] : angles[left];
		else
			return angles[back];
	}
	else
		return (damages[right] < damages[left]) ? angles[right] : angles[left];
}


float randnum(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}



void CAntiAim::do_antiaim(CUserCmd* cmd)
{

	Math::AngleNormalize(cmd->viewangles);
	
	/*----------------------------ANTIAIM----------------------------------------*/

		/*-----------------------PITCH------------------------------------*/

	switch (ReturnMovementType()) {
	case MovingAAType::Standing: {
		switch (settings.RageBot.Pitch_stand)
		{
		case 0: break;
		case 1: cmd->viewangles.x = 0.f; break; // AA_NONE
		case 2: cmd->viewangles.x = 89.f; break; // AA_DOWN
		case 3: cmd->viewangles.x = -89.f; break; // AA_UP
		}
	}break;
	case MovingAAType::Moving: {
		switch (settings.RageBot.Pitch_move)
		{
		case 0: break;
		case 1: cmd->viewangles.x = 0.f; break; // AA_NONE
		case 2: cmd->viewangles.x = 89.f; break; // AA_DOWN
		case 3: cmd->viewangles.x = -89.f; break; // AA_UP
		}
	}break;
	case MovingAAType::Air: {
		switch (settings.RageBot.Pitch_air)
		{
		case 0: break;
		case 1: cmd->viewangles.x = 0.f; break; // AA_NONE
		case 2: cmd->viewangles.x = 89.f; break; // AA_DOWN
		case 3: cmd->viewangles.x = -89.f; break; // AA_UP
		}
	}break;
	}
	/*-----------------------PITCH------------------------------------*/

	static int jitterangle = 0;
	int maxJitter;
	int random;
	float temp;
	static bool bFlip;
	static float RealAng = 0;
	static float RealPit = 0;
  /*-----------------------REAL------------------------------------*/
	switch (ReturnMovementType()) {
	case MovingAAType::Standing: {
		switch (settings.RageBot.Yaw_stand)
		{
		case 0:
			break;

		

		case 1:
			cmd->viewangles.y = 180.f;
			break;

		case 2:
			cmd->viewangles.y = 90.f;
			break;

		case 3:
			cmd->viewangles.y = -90.f;
			break;

		case 4:
			cmd->viewangles.y = 0;
			break;

		
		case 5:
			cmd->viewangles.y += fmodf(Interfaces::GlobalVars()->curtime * 360.0 / M_PHI, 360.0);
			break;

		case 6:
		{

			if (jitterangle <= 1)
			{
				cmd->viewangles.y += 90;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				cmd->viewangles.y -= 90;
			}

			int re = rand() % 4 + 1;


			if (jitterangle <= 1)
			{
				if (re == 4)
					cmd->viewangles.y += 180;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				if (re == 4)
					cmd->viewangles.y -= 180;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}
		}break;

	

		}
	}break;


	case MovingAAType::Moving: {
		switch (settings.RageBot.Yaw_move)
		{
		case 0:
			break;

		case 1:
			cmd->viewangles.y = 180.f;
			break;

		case 2:
			cmd->viewangles.y = 90.f;
			break;

		case 3:
			cmd->viewangles.y = -90.f;
			break;

		case 4:
			cmd->viewangles.y = 0;
			break;

		case 5:
			cmd->viewangles.y += fmodf(Interfaces::GlobalVars()->curtime * 360.0 / M_PHI, 360.0);
			break;

		case 6:
		{

			if (jitterangle <= 1)
			{
				cmd->viewangles.y += 90;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				cmd->viewangles.y -= 90;
			}

			int re = rand() % 4 + 1;


			if (jitterangle <= 1)
			{
				if (re == 4)
					cmd->viewangles.y += 180;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				if (re == 4)
					cmd->viewangles.y -= 180;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}

		}break;

		

		}
	}break;



	case MovingAAType::Air: {
		switch (settings.RageBot.Yaw_air)
		{
		case 0:
			break;



		case 1:
			cmd->viewangles.y = 180.f;
			break;

		case 2:
			cmd->viewangles.y = 90.f;
			break;

		case 3:
			cmd->viewangles.y = -90.f;
			break;

		case 4:
			cmd->viewangles.y = 0;
			break;


		case 5:
			cmd->viewangles.y += fmodf(Interfaces::GlobalVars()->curtime * 360.0 / M_PHI, 360.0);
			break;

		case 6:
		{

			if (jitterangle <= 1)
			{
				cmd->viewangles.y += 90;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				cmd->viewangles.y -= 90;
			}

			int re = rand() % 4 + 1;


			if (jitterangle <= 1)
			{
				if (re == 4)
					cmd->viewangles.y += 180;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				if (re == 4)
					cmd->viewangles.y -= 180;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}
		}break;
		
		}
	 }
	}
	/*-----------------------REAL------------------------------------*/

	
	

}


float get_curtime(CUserCmd* ucmd) {
	
	if (!LocalPlayer)
		return 0;

	int g_tick = 0;
	CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = (float)LocalPlayer->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * Interfaces::GlobalVars()->interval_per_tick;
	return curtime;
}

bool next_lby_update(const float yaw_to_break, CUserCmd* cmd)
{
	if (!LocalPlayer)
		return false;

	static float next_lby_update_time = 0;
	float curtime = get_curtime(cmd);

	auto animstate = LocalPlayer->GetAnimState();
	if (!animstate)
		return false;

	if (!(LocalPlayer->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->speed_2d > 0.1)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}

void CAntiAim::do_antiaimdesync(CUserCmd* cmd)
{
	/*--------------DESYNC-----------------------*/
	
	


	switch (settings.RageBot.desync)
	{
	case 0: break;
	case 1:
	{
		float desync = LocalPlayer->getmaxdesync();
		float balance = 1.0f;

		int type = 2;
		if (type == 2)
			balance = -1.0f;

		if (Interfaces::GlobalVars()->curtime <= next_lby_update(cmd->viewangles.y, cmd))
		{

			auto net_channel = Interfaces::Engine()->GetNetChannel();

			if (!net_channel)
				return;

			if (net_channel->m_nChokedPackets >= 2) {
				cmd->viewangles.y = Math::NormalizeYaw(cmd->viewangles.y);
				return;
			}

			if (type == 1)
				cmd->viewangles.y -= 100.0f;
			else
				cmd->viewangles.y += (balance * 120.0f);
		}
		else if (type != 1) {
			cmd->viewangles.y -= (desync + 30.0f) * balance;
		}

	}break;
	case 2:
	{
		int m_iJitter;
		int jitter_side = 1;

		cmd->viewangles.y += 180.0f;

		float desync = LocalPlayer->getmaxdesync();
		float lby_delta = 180.0f - desync + 10.0f;
		float desync_length = 180.0f - lby_delta * 0.5f;
		float jitter = 90.0f * jitter_side;


		if (jitter_side == 1)
			cmd->viewangles.y += desync_length;
		else if (jitter_side == -1)
			cmd->viewangles.y -= desync_length;


		int v19 = 0;
		if (Interfaces::GlobalVars()->curtime < Interfaces::GlobalVars()->curtime <= next_lby_update(cmd->viewangles.y, cmd)) {
			v19 = m_iJitter;
		}
		else {
			m_iJitter = 0;
		}

		int v20 = v19 - 1;
		if (v20) {
			if (v20 == 1) {
				if (jitter_side == 1)
					cmd->viewangles.y += lby_delta;
				else
					cmd->viewangles.y += desync - 190.0f;
			}
		}
		else {
			if (jitter_side == 1)
				cmd->viewangles.y += desync - 190.0;
			else
				cmd->viewangles.y += lby_delta;
			bSendPacket = true;
		}

		if (++m_iJitter >= 3)
			m_iJitter = 0;

		cmd->viewangles.y = Math::NormalizeYaw(cmd->viewangles.y);
	}break;
	case 3:
	{
		
			cmd->viewangles.y += (rand() % 180) + LocalPlayer->getmaxdesync();

			if (LocalPlayer->GetFlags() & FL_ONGROUND && cmd->sidemove < 3 && cmd->sidemove > -3) 
			{
				static bool switch_ = false;
				if (switch_)
					cmd->sidemove = 2;
				else
					cmd->sidemove = -2;
				switch_ = !switch_;
			}
	}break;
	}


	/*--------------DESYNC-----------------------*/

}


void CAntiAim::fix_movement(CUserCmd* cmd)
{
	
	if (!LocalPlayer)
		return;

	float normalizedx;


	cmd->forwardmove = clamp(cmd->forwardmove, -450.f, 450.f);
	cmd->sidemove = clamp(cmd->sidemove, -450.f, 450.f);
	cmd->upmove = clamp(cmd->upmove, -320.f, 320.f);

	cmd->viewangles.x = clamp(cmd->viewangles.x, -89.f, 89.f);

	
	normalizedx = fmod(cmd->viewangles.x + 180.f, 360.f) - 180.f;

	Vector real_viewangles;
	Interfaces::Engine()->GetViewAngles(real_viewangles);

	Vector vecMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
	float speed = sqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);

	Vector angMove;
	Math::VectorAngles2(vecMove, angMove);

	float yaw = DEG2RAD(cmd->viewangles.y - real_viewangles.y + angMove.y);

	cmd->forwardmove = cos(yaw) * speed;
	cmd->sidemove = sin(yaw) * speed;

	if (cmd->viewangles.x < -89.f || cmd->viewangles.x > 89.f) cmd->forwardmove *= -1;
}

void CAntiAim::Fakewalk(CUserCmd* cmd)
{

	if (settings.RageBot.walkspeed <= 0.f)
		return;

	if (GetAsyncKeyState(settings.RageBot.fakewalk))
	{

		float min_speed = (float)(FastSqrt(Square(cmd->forwardmove) + Square(cmd->sidemove) + Square(cmd->upmove)));
		if (min_speed <= 0.f)
			return;

		if (cmd->buttons & IN_DUCK)
			settings.RageBot.walkspeed *= 2.94117647f;

		if (min_speed <= settings.RageBot.walkspeed)
			return;

		float kys = settings.RageBot.walkspeed / min_speed;

		cmd->forwardmove *= kys;
		cmd->sidemove *= kys;
		cmd->upmove *= kys;
	}
}

/*
void CAntiAim::FakeDuck(CUserCmd * cmd)
{
	CBaseEntity* localplayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	if (GetAsyncKeyState(Settings::Ragebot::fakeduck))
	{
		cmd->buttons |= IN_BULLRUSH;

		static bool counter = false;
		static int counte = 0;

		counte++;

		if (counte == 21) {
			counte = 0;
			counter = !counter;
		}



		if (counter)
		{
			cmd->buttons &= ~IN_DUCK;
			bSendPacket = true;
		}
		else
		{
			cmd->buttons |= IN_DUCK;
			bSendPacket = false;
		}
	}
}
*/

void CAntiAim::OnCreateMove(CUserCmd* cmd)
{

	AntiAim->Fakewalk(cmd);
	//AntiAim->FakeDuck(cmd);

	if ((!settings.RageBot.antiaim_enabled) || (LocalPlayer->GetMoveType() == MOVETYPE_LADDER) || (LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP) || cmd->buttons & IN_ATTACK && mLocal->m_pWeaponEntity->CanShot() || mLocal->m_pWeaponEntity->IsGrenade()|| cmd->buttons & IN_USE || LocalPlayer->IsDead())
		return;
	if (bSendPacket)
	{
		do_antiaim(cmd);

		if (settings.RageBot.freestand)
			freestanding(cmd);
	}else
	do_antiaimdesync(cmd);

	

	Math::Clamp(cmd->viewangles);
	fix_movement(cmd);
}

CAntiAim*  AntiAim = new CAntiAim;