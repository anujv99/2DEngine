#define Vec3_CHECK(v) ASSERT(v)

inline bool CheckVec3(const Vec3 & _vec) {
	return _vec.x <= FLT_MAX && _vec.x >= -FLT_MAX
		&& _vec.y <= FLT_MAX && _vec.y >= -FLT_MAX
		&& _vec.z <= FLT_MAX && _vec.z >= -FLT_MAX;
}

inline Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
inline Vec3::Vec3(float _v) : x(_v), y(_v), z(_v) {}
inline Vec3::Vec3(Vec2 _vec, float _z) : x(_vec.x), y(_vec.y), z(_z) {}
inline Vec3::Vec3(gmvec3 _vec) : x(_vec.x), y(_vec.y), z(_vec.z) {}
inline Vec3::Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
inline Vec3::Vec3(const Vec2 & _vec) : x(_vec.x), y(_vec.y), z(0.0f) {}

inline Vec3 & Vec3::operator+=(const Vec3 & _vec) {
	x += _vec.x;
	y += _vec.y;
	z += _vec.z;
	Vec3_CHECK(CheckVec3(*this));
	return *this;
}

inline Vec3 & Vec3::operator-=(const Vec3 & _vec) {
	x -= _vec.x;
	y -= _vec.y;
	z -= _vec.z;
	Vec3_CHECK(CheckVec3(*this));
	return *this;
}

inline Vec3 & Vec3::operator*=(const Vec3 & _vec) {
	x *= _vec.x;
	y *= _vec.y;
	z *= _vec.z;
	Vec3_CHECK(CheckVec3(*this));
	return *this;
}

inline Vec3 & Vec3::operator/=(const Vec3 & _vec) {
	Vec3_CHECK(std::abs(_vec.x) > FLT_MIN);
	Vec3_CHECK(std::abs(_vec.y) > FLT_MIN);
	Vec3_CHECK(std::abs(_vec.z) > FLT_MIN);

	x /= _vec.x;
	y /= _vec.y;
	z /= _vec.z;
	Vec3_CHECK(CheckVec3(*this));
	return *this;
}

inline Vec3 & Vec3::operator+=(float _v) {
	x += _v;
	y += _v;
	z += _v;
	Vec3_CHECK(CheckVec3(*this));
	return *this;
}

inline Vec3 & Vec3::operator-=(float _v) {
	x -= _v;
	y -= _v;
	z -= _v;
	Vec3_CHECK(CheckVec3(*this));
	return *this;
}

inline Vec3 & Vec3::operator*=(float _v) {
	x *= _v;
	y *= _v;
	z *= _v;
	Vec3_CHECK(CheckVec3(*this));
	return *this;
}

inline Vec3 & Vec3::operator/=(float _v) {
	float div = 1.0f / _v;

	x *= div;
	y *= div;
	z *= div;
	Vec3_CHECK(CheckVec3(*this));
	return *this;
}

inline Vec3 Vec3::operator+(const Vec3 & _vec) const {
	Vec3_CHECK(CheckVec3(*this));
	Vec3_CHECK(CheckVec3(_vec));
	return Vec3(x + _vec.x, y + _vec.y, z + _vec.z);
}

inline Vec3 Vec3::operator-(const Vec3 & _vec) const {
	Vec3_CHECK(CheckVec3(*this));
	Vec3_CHECK(CheckVec3(_vec));
	return Vec3(x - _vec.x, y - _vec.y, z - _vec.z);
}

inline Vec3 Vec3::operator-() const {
	Vec3_CHECK(CheckVec3(*this));
	return Vec3(-x, -y, -z);
}

inline Vec3 Vec3::operator*(const Vec3 & _vec) const {
	Vec3_CHECK(CheckVec3(*this));
	Vec3_CHECK(CheckVec3(_vec));
	return Vec3(x * _vec.x, y * _vec.y, z * _vec.z);
}

inline Vec3 Vec3::operator/(const Vec3 & _vec) const {
	Vec3_CHECK(CheckVec3(*this));
	Vec3_CHECK(CheckVec3(_vec));
	Vec3_CHECK(std::fabs(_vec.x) > FLT_MIN);
	Vec3_CHECK(std::fabs(_vec.y) > FLT_MIN);
	Vec3_CHECK(std::fabs(_vec.z) > FLT_MIN);
	return Vec3(x / _vec.x, y / _vec.y, z / _vec.z);
}

inline Vec3 Vec3::operator*(float _v) const {
	Vec3_CHECK(CheckVec3(*this));
	return Vec3(x * _v, y * _v, z * _v);
}

inline Vec3 Vec3::operator/(float _v) const {
	Vec3_CHECK(std::fabs(_v) > FLT_MIN);
	Vec3_CHECK(CheckVec3(*this));
	float div = 1.0f / _v;
	return Vec3(x * div, y * div, z * div);
}

inline Vec2 Vec3::xy() const {
	return Vec2(x, y);
}

inline Vec2 Vec3::yx() const {
	return Vec2(y, z);
}

inline Vec2 Vec3::yz() const {
	return Vec2(y, z);
}

inline Vec2 Vec3::zy() const {
	return Vec2(z, y);
}

inline Vec2 Vec3::xz() const {
	return Vec2(x, z);
}

inline Vec2 Vec3::zx() const {
	return Vec2(z, x);
}

inline bool Vec3::operator==(const Vec3 & _vec) const {
	return x == _vec.x && y == _vec.y && z == _vec.z;
}

inline bool Vec3::operator!=(const Vec3 & _vec) const {
	return !(*this == _vec);
}

inline float Length(const Vec3 & _vec) {
	return std::sqrt(LengthSqr(_vec));
}

inline float LengthSqr(const Vec3 & _vec) {
	return _vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z;
}

inline float Dot(const Vec3 & _v1, const Vec3 & _v2) {
	return _v1.x * _v2.x + _v1.y * _v2.y + _v1.z * _v2.z;
}

inline float Distance(const Vec3 & _v1, const Vec3 & _v2) {
	return Length(_v2 - _v1);
}

inline Vec3 operator*(float _v, const Vec3 & _vec) {
	return Vec3(_vec.x * _v, _vec.y * _v, _vec.z * _v);
}

inline Vec3 Cross(const Vec3 & _v1, const Vec3 & _v2) {
	Vec3 res(_v1.y * _v2.z - _v1.z * _v2.y, _v1.z * _v2.x - _v1.x * _v2.z, _v1.x * _v2.y - _v1.y * _v2.x);
	Vec3_CHECK(CheckVec3(res));
	return res;
}

inline Vec3 Normalize(const Vec3 & _vec) {
	float len = Length(_vec);
	Vec3_CHECK(std::fabs(len) > FLT_MIN);
	float divLen = 1.0f / len;
	return _vec * divLen;
}

inline Vec3 Max(const Vec3 & _v1, const Vec3 & _v2) {
	return Vec3(std::max(_v1.x, _v2.x), std::max(_v1.y, _v2.y), std::max(_v1.z, _v2.z));
}

inline Vec3 Min(const Vec3 & _v1, const Vec3 & _v2) {
	return Vec3(std::min(_v1.x, _v2.x), std::min(_v1.y, _v2.y), std::min(_v1.z, _v2.z));
}

inline Vec3 Abs(const Vec3 & _vec) {
	return Vec3(std::fabs(_vec.x), std::fabs(_vec.y), std::fabs(_vec.z));
}

inline Vec3 Lerp(const Vec3 & _v1, const Vec3 & _v2, float _v) {
	return Vec3(Lerp(_v1.x, _v2.x, _v), Lerp(_v1.y, _v2.y, _v), Lerp(_v1.z, _v2.z, _v));
}

inline Vec3 Clamp(const Vec3 & _vec, const Vec3 & _min, const Vec3 & _max) {
	return Vec3(std::clamp(_vec.x, _min.x, _max.x), std::clamp(_vec.y, _min.y, _max.y), std::clamp(_vec.z, _min.z, _max.z));
}

inline Vec3 Saturate(const Vec3 & _vec) {
	return Vec3(std::clamp(_vec.x, 0.0f, 1.0f), std::clamp(_vec.y, 0.0f, 1.0f), std::clamp(_vec.z, 0.0f, 1.0f));
}

inline Vec3 RotateAxisX(const Vec3 & _dir, float _angle) {

	float cosine = prev::Cos(_angle);
	float sine = prev::Sin(_angle);

	float newY = _dir.y * cosine - _dir.z * sine;
	float newZ = _dir.y * sine + _dir.z * cosine;
	float newX = _dir.x;

	return Vec3(newX, newY, newZ);
}

inline Vec3 RotateAxisY(const Vec3 & _dir, float _angle) {

	float cosine = prev::Cos(_angle);
	float sine = prev::Sin(_angle);

	float newZ = _dir.z * cosine - _dir.x * sine;
	float newX = _dir.z * sine + _dir.x * cosine;
	float newY = _dir.y;

	return Vec3(newX, newY, newZ);
}

inline Vec3 RotateAxisZ(const Vec3 & _dir, float _angle) {

	float cosine = prev::Cos(_angle);
	float sine = prev::Sin(_angle);

	float newX = _dir.x * cosine - _dir.y * sine;
	float newY = _dir.x * sine + _dir.y * cosine;
	float newZ = _dir.z;

	return Vec3(newX, newY, newZ);
}

inline Vec3 RotateAxis(const Vec3 & _dir, float _angle, const Vec3 & _axis) {
	// http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/

	float cosine = prev::Cos(_angle);
	float cosineMinus = 1.0f - cosine;
	float sine = prev::Sin(_angle);

	float x = _dir.x;
	float y = _dir.y;
	float z = _dir.z;

	float u = _axis.x;
	float v = _axis.y;
	float w = _axis.z;

	float axisLenSqr = LengthSqr(_axis);
	float axisLen = std::sqrtf(axisLenSqr);
	float c0 = u * x + v * y + w * z;

	float xVal = u * c0 * cosineMinus + axisLenSqr * x * cosine + axisLen * (-w * y + v * z) * sine;
	float yVal = v * c0 * cosineMinus + axisLenSqr * y * cosine + axisLen * ( w * x - u * z) * sine;
	float zVal = w * c0 * cosineMinus + axisLenSqr * z * cosine + axisLen * (-v * x + u * y) * sine;

	Vec3 result = Vec3(xVal, yVal, zVal) / axisLenSqr;
	return result;
}