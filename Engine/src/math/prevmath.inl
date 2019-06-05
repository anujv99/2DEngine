
inline float Lerp(float _v1, float _v2, float _v) {
	return _v1 + _v * (_v2 - _v1);
}

inline float ToRadians(float _v) {
	return _v * PV_PI_180;
}

inline float ToDegrees(float _v) {
	return _v * PV_180_PI;
}

inline float Sin(float _angle) {
	float sine = std::sin(ToRadians(_angle));
	if (sine <= FLT_EPSILON)
		return 0;
	else if (1.0f - sine <= FLT_EPSILON)
		return 1.0f;
	else if (1.0f + sine <= FLT_EPSILON)
		return -1.0f;
	
	return sine;
}

inline float Cos(float _angle) {
	float cosine = std::cos(ToRadians(_angle));
	if (cosine <= FLT_EPSILON)
		return 0;
	else if (1.0f - cosine <= FLT_EPSILON)
		return 1.0f;
	else if (1.0f + cosine <= FLT_EPSILON)
		return -1.0f;

	return cosine;
}
