#pragma once

#include "common/timer.h"

namespace prev {

	#define PV_PI 3.14159265358979f
	#define PV_PI_180 0.01745329251994329576923690768489f
	#define PV_180_PI 57.295779513082320876798154814105f

	float Lerp(float _v1, float _v2, float _v);
	float ToRadians(float _v);
	float ToDegrees(float _v);

	float Sin(float _angle);
	float Cos(float _angle);
	float Tan(float _angle);

	float ATan(float _angle);

	float Clamp(float _val, float _min, float _max);
	float Saturate(float _val);

	#include "prevmath.inl"

}
