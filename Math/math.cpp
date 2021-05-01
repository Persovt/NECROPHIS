#include "math.h"

DWORD 	ClipTraceToPlayersFn = CSX::Memory::FindPattern(("client_panorama.dll"), ("53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10"));

namespace Math
{

	void inline SinCos(float radians, float* sine, float* cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);
	}

	float GRD_TO_BOG(float GRD) 
	{
		return (M_PI / 180) * GRD;
	}
	int Distance(Vector2D point1, Vector2D point2)
	{
		int diffY = point1.y - point2.y;
		int diffX = point1.x - point2.x;
		return sqrt((diffY * diffY) + (diffX * diffX));
	}
	void VectorAngles(const Vector& forward, Vector& up, Vector& angles)
	{
		Vector left = left.CrossProduct(up, forward);
		left.NormalizeInPlace();

		float forwardDist = forward.Length2D();

		if (forwardDist > 0.001f) {
			angles.x = atan2f(-forward.z, forwardDist) * 180 / M_PI;
			angles.y = atan2f(forward.y, forward.x) * 180 / M_PI;

			float upZ = (left.y * forward.x) - (left.x * forward.y);
			angles.z = atan2f(left.z, upZ) * 180 / M_PI;
		}
		else {
			angles.x = atan2f(-forward.z, forwardDist) * 180 / M_PI;
			angles.y = atan2f(-left.x, left.y) * 180 / M_PI;
			angles.z = 0;
		}
	}
	void VectorAngles(const Vector& forward, QAngle& angles)
	{
		if (forward[1] == 0.0f && forward[0] == 0.0f)
		{
			angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
			angles[1] = 0.0f;  //yaw left/right
		}
		else
		{
			angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
			angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

			if (angles[1] > 90)
				angles[1] -= 180;
			else if (angles[1] < 90)
				angles[1] += 180;
			else if (angles[1] == 90)
				angles[1] = 0;
		}

		angles[2] = 0.0f;
	}
	float NormalizeYaw(float yaw)
	{
		if (yaw > 180)
			yaw -= (round(yaw / 360) * 360.f);
		else if (yaw < -180)
			yaw += (round(yaw / 360) * -360.f);

		return yaw;
	}
	void VectorAngles2(const Vector& forward, QAngle& angles)
	{
		float tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
			{
				pitch = 270;
			}
			else
			{
				pitch = 90;
			}
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
			if (yaw < 0)
			{
				yaw += 360;
			}
			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / M_PI);

			if (pitch < 0)
			{
				pitch += 360;
			}
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}

	void AngleVectors2(const Vector &angles, Vector *forward)
	{
		Assert(s_bMathlibInitialized);
		Assert(forward);

		float	sp, sy, cp, cy;

		sy = sin(DEG2RAD(angles[1]));
		cy = cos(DEG2RAD(angles[1]));

		sp = sin(DEG2RAD(angles[0]));
		cp = cos(DEG2RAD(angles[0]));

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}
	void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
	{
		float sp, sy, sr, cp, cy, cr;

		
			Math::SinCos(DEG2RAD(angles.x), &sp, &cp);
			Math::SinCos(DEG2RAD(angles.y), &sy, &cy);
			Math::SinCos(DEG2RAD(angles.z), &sr, &cr);
		

		if (forward != nullptr)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right != nullptr)
		{
			right->x = -1 * sr * sp * cy + -1 * cr * -sy;
			right->y = -1 * sr * sp * sy + -1 * cr * cy;
			right->z = -1 * sr * cp;
		}

		if (up != nullptr)
		{
			up->x = cr * sp * cy + -sr * -sy;
			up->y = cr * sp * sy + -sr * cy;
			up->z = cr * cp;
		}
	}

	

	float RandomFloat(float min, float max)
	{
		static auto random_float = reinterpret_cast<float(*)(float, float)>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloat"));

		return random_float(min, max);
	}


	int RandomInt(int low, int high) 
	{
		return rand() % (high - low + 1) + low;
	}


	

	enum
	{
		PITCH = 0, // up / down
		YAW, // left / right
		ROLL // fall over
	};
	
	
	Vector CalcAngle(Vector src, Vector dst)
	{
		Vector angles;
		Vector delta = src - dst;
		//AngleVectors(angles, &delta ,0,0);
		VectorAngles(delta, angles);
		delta.Normalized();
		return angles;
	}

	float GetFov(const Vector viewAngle, const Vector aimAngle)
	{
		Vector ang, aim;

		AngleVectors(viewAngle, &aim,0 ,0);
		AngleVectors(aimAngle, &ang, 0, 0);

		return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
	}

	Vector calculate_angle(Vector src, Vector dst) 
	{
		Vector angles;

		Vector delta = src - dst;
		float hyp = delta.Length2D();

		angles.y = atanf(delta.y / delta.x) * 57.2957795131f;
		angles.x = atanf(-delta.z / hyp) * -57.2957795131f;
		angles.z = 0.0f;

		if (delta.x >= 0.0f)
			angles.y += 180.0f;

		return angles;
	}
	void ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
	{
		if (!ClipTraceToPlayersFn)
			return;

		_asm
		{
			MOV		EAX, filter
			LEA		ECX, tr
			PUSH	ECX
			PUSH	EAX
			PUSH	mask
			LEA		EDX, vecAbsEnd
			LEA		ECX, vecAbsStart
			CALL	ClipTraceToPlayersFn
			ADD		ESP, 0xC
		}
	}
	void TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, CBaseEntity* ignore, trace_t* ptr)
	{
		Ray_t ray;
		ray.Init(vecAbsStart, vecAbsEnd);
		CTraceFilter filter;
		filter.pSkip = ignore;

		Interfaces::EngineTrace()->TraceRay(ray, mask, &filter, ptr);
	}

	void AngleNormalize(Vector& vAngles)
	{
		if (vAngles.x > 89.0f && vAngles.x <= 180.0f)
			vAngles.x = 89.0f;
		while (vAngles.x > 180.f)
			vAngles.x -= 360.f;
		while (vAngles.x < -89.0f)
			vAngles.x = -89.0f;
		while (vAngles.y > 180.f)
			vAngles.y -= 360.f;
		while (vAngles.y < -180.f)
			vAngles.y += 360.f;
		vAngles.z = 0;
	}
	
	void NormalizeAngle(Vector& angle)
	{
		for (auto i = 0; i < 3; i++) {
			while (angle[i] < -180.0f) angle[i] += 360.0f;
			while (angle[i] > 180.0f) angle[i] -= 360.0f;
		}
	}

	void ClampAngles(Vector& angles)
	{
		if (angles.x > 89.0f) angles.x = 89.0f;
		else if (angles.x < -89.0f) angles.x = -89.0f;

		if (angles.y > 180.0f) angles.y = 180.0f;
		else if (angles.y < -180.0f) angles.y = -180.0f;

		angles.z = 0;
	}

	bool Clamp(Vector &angles)
	{
		Vector a = angles;
		NormalizeAngle(a);
		ClampAngles(a);

		if (isnan(a.x) || isinf(a.x) ||
			isnan(a.y) || isinf(a.y) ||
			isnan(a.z) || isinf(a.z)) {
			return false;
		}
		else {
			angles = a;
			return true;
		}
	}
	
	void SmoothAngles(Vector MyViewAngles, Vector AimAngles, Vector &OutAngles, float Smoothing)
	{

		OutAngles = AimAngles - MyViewAngles;
		NormalizeAngle(OutAngles);
		OutAngles = OutAngles / Smoothing;
		NormalizeAngle(OutAngles);
	}

	

	float DistanceScreen(Vector2D vDstPos, Vector2D vSrcPos)
	{
		return (sqrt(pow(vSrcPos.x - vDstPos.x, 2) + pow(vSrcPos.y - vDstPos.y, 2)));
	}
}