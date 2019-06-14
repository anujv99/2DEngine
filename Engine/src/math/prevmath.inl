
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
	if (std::abs(sine) <= FLT_EPSILON)
		return 0;
	else if (1.0f - sine <= FLT_EPSILON)
		return 1.0f;
	else if (1.0f + sine <= FLT_EPSILON)
		return -1.0f;
	
	return sine;
}

inline float Cos(float _angle) {
	float cosine = std::cos(ToRadians(_angle));
	if (std::abs(cosine) <= FLT_EPSILON)
		return 0;
	else if (1.0f - cosine <= FLT_EPSILON)
		return 1.0f;
	else if (1.0f + cosine <= FLT_EPSILON)
		return -1.0f;

	return cosine;
}

inline float Tan(float _angle) {
	float tangent = std::tan(ToRadians(_angle));
	if (std::abs(tangent) <= FLT_EPSILON)
		return 0;
	else if (1.0f - tangent <= FLT_EPSILON)
		return 1.0f;
	else if (1.0f + tangent <= FLT_EPSILON)
		return -1.0f;

	return tangent;
}

inline float ATan(float _angle) {
	float atangent = std::atan(ToRadians(_angle));
	if (std::abs(atangent) <= FLT_EPSILON)
		return 0;
	else if (1.0f - atangent <= FLT_EPSILON)
		return 1.0f;
	else if (1.0f + atangent <= FLT_EPSILON)
		return -1.0f;

	return atangent;
}

inline float Clamp(float _val, float _min, float _max) {
	return std::min(std::max(_val, _min), _max);
}

inline float Saturate(float _val) {
	return Clamp(_val, 0.0f, 1.0f);
}