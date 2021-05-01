#pragma once
#include "../Engine/Engine.h"

namespace Engine
{
	class CMe;
	class CBaseEntity;
}

using namespace Engine;

namespace Math
{
	Vector calculate_angle(Vector src, Vector dst);
	int Distance(Vector2D point1, Vector2D point2);
	float DistanceScreen(Vector2D vDstPos, Vector2D vSrcPos);
	void		SmoothAngles(Vector MyViewAngles, Vector AimAngles, Vector &OutAngles, float Smoothing);
	float GRD_TO_BOG(float GRD);
	void AngleVectors2(const Vector &angles, Vector *forward);
	void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up);
	float RandomFloat(float min, float max);
	int RandomInt(int low, int high);

	void VectorAngles(const Vector& forward, Vector& up, Vector& angles);
	void NormalizeAngle(Vector& angle);
	void AngleNormalize(Vector& vAngles);
	bool Clamp(Vector &angles);
	extern Vector CalcAngle(Vector v1, Vector v2);
	extern void VectorAngles(const Vector& forward, QAngle& angles);
	extern void VectorAngles2(const Vector& forward, QAngle& angles);
	float NormalizeYaw(float yaw);
	void TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, CBaseEntity* ignore, trace_t* ptr);
	void ClipTraceToPlayers(const Vector & vecAbsStart, const Vector & vecAbsEnd, unsigned int mask, ITraceFilter * filter, trace_t * tr);
	extern inline void SinCos(float radians, float* sine, float* cosine);
	extern float GetFov(const Vector viewAngle, const Vector aimAngle);
}