#define VEC4_CHECK(x) ASSERT(x)

inline bool CheckVec4(const Vec4 & _vec) {
	return _vec.x <= FLT_MAX && _vec.x >= -FLT_MAX
		&& _vec.y <= FLT_MAX && _vec.y >= -FLT_MAX
		&& _vec.z <= FLT_MAX && _vec.z >= -FLT_MAX
		&& _vec.w <= FLT_MAX && _vec.w >= -FLT_MAX;
}

inline Vec4::Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
inline Vec4::Vec4(float _v) : x(_v), y(_v), z(_v), w(_v) {}
inline Vec4::Vec4(gmvec4 _vec) : x(_vec.x), y(_vec.y), z(_vec.z), w(_vec.w) {}
inline Vec4::Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
inline Vec4::Vec4(const Vec2 & _vec) : x(_vec.x), y(_vec.y), z(0.0f), w(0.0f) {}
inline Vec4::Vec4(const Vec2 & _vec1, const Vec2 & _vec2) : x(_vec1.x), y(_vec1.y), z(_vec2.x), w(_vec2.y) {}
inline Vec4::Vec4(const Vec3 & _vec) : x(_vec.x), y(_vec.y), z(_vec.z), w(0.0f) {}
inline Vec4::Vec4(const Vec3 & _vec, float _w) : x(_vec.x), y(_vec.y), z(_vec.z), w(_w) {}

inline Vec4 & Vec4::operator+=(const Vec4 & _vec) {
	x += _vec.x;
	y += _vec.y;
	z += _vec.z;
	w += _vec.w;
	VEC4_CHECK(CheckVec4(*this));
	return *this;
}

inline Vec4 & Vec4::operator-=(const Vec4 & _vec) {
	x -= _vec.x;
	y -= _vec.y;
	z -= _vec.z;
	w -= _vec.w;
	VEC4_CHECK(CheckVec4(*this));
	return *this;
}

inline Vec4 & Vec4::operator*=(const Vec4 & _vec) {
	x *= _vec.x;
	y *= _vec.y;
	z *= _vec.z;
	w *= _vec.w;
	VEC4_CHECK(CheckVec4(*this));
	return *this;
}

inline Vec4 & Vec4::operator/=(const Vec4 & _vec) {
	VEC4_CHECK(std::fabs(_vec.x) > FLT_MIN);
	VEC4_CHECK(std::fabs(_vec.y) > FLT_MIN);
	VEC4_CHECK(std::fabs(_vec.z) > FLT_MIN);
	VEC4_CHECK(std::fabs(_vec.w) > FLT_MIN);

	x /= _vec.x;
	y /= _vec.y;
	z /= _vec.z;
	w /= _vec.w;
	VEC4_CHECK(CheckVec4(*this));
	return *this;
}

inline Vec4 & Vec4::operator+=(float _v) {
	x += _v;
	y += _v;
	z += _v;
	w += _v;
	VEC4_CHECK(CheckVec4(*this));
	return *this;
}

inline Vec4 & Vec4::operator-=(float _v) {
	x -= _v;
	y -= _v;
	z -= _v;
	w -= _v;
	VEC4_CHECK(CheckVec4(*this));
	return *this;
}

inline Vec4 & Vec4::operator*=(float _v) {
	x *= _v;
	y *= _v;
	z *= _v;
	w *= _v;
	VEC4_CHECK(CheckVec4(*this));
	return *this;
}

inline Vec4 & Vec4::operator/=(float _v) {
	float div = 1.0f / _v;

	x *= div;
	y *= div;
	z *= div;
	w *= div;
	VEC4_CHECK(CheckVec4(*this));
	return *this;
}

inline Vec4 Vec4::operator+(const Vec4 & _vec) const {
	VEC4_CHECK(CheckVec4(*this));
	VEC4_CHECK(CheckVec4(_vec));
	return Vec4(x + _vec.x, y + _vec.y, z + _vec.z, w + _vec.w);
}

inline Vec4 Vec4::operator-(const Vec4 & _vec) const {
	VEC4_CHECK(CheckVec4(*this));
	VEC4_CHECK(CheckVec4(_vec));
	return Vec4(x - _vec.x, y - _vec.y, z - _vec.z, w - _vec.w);
}

inline Vec4 Vec4::operator-() const {
	VEC4_CHECK(CheckVec4(*this));
	return Vec4(-x, -y, -z, -w);
}

inline Vec4 Vec4::operator*(const Vec4 & _vec) const {
	VEC4_CHECK(CheckVec4(*this));
	VEC4_CHECK(CheckVec4(_vec));
	return Vec4(x * _vec.x, y * _vec.y, z * _vec.z, w * _vec.w);
}

inline Vec4 Vec4::operator/(const Vec4 & _vec) const {
	VEC4_CHECK(CheckVec4(*this));
	VEC4_CHECK(CheckVec4(_vec));
	VEC4_CHECK(std::fabs(_vec.x) > FLT_MIN);
	VEC4_CHECK(std::fabs(_vec.y) > FLT_MIN);
	VEC4_CHECK(std::fabs(_vec.z) > FLT_MIN);
	VEC4_CHECK(std::fabs(_vec.w) > FLT_MIN);
	return Vec4(x / _vec.x, y / _vec.y, z / _vec.z, w / _vec.w);
}

inline Vec4 Vec4::operator*(float _v) const {
	VEC4_CHECK(CheckVec4(*this));
	return Vec4(x * _v, y * _v, z * _v, w * _v);
}

inline Vec4 Vec4::operator/(float _v) const {
	VEC4_CHECK(std::fabs(_v) > FLT_MIN);
	VEC4_CHECK(CheckVec4(*this));
	float div = 1.0f / _v;
	return Vec4(x * div, y * div, z * div, w * div);
}

inline Vec2 Vec4::xy() const {
	return Vec2(x, y);
}

inline Vec2 Vec4::yx() const {
	return Vec2(y, z);
}

inline Vec2 Vec4::yz() const {
	return Vec2(y, z);
}

inline Vec2 Vec4::zy() const {
	return Vec2(z, y);
}

inline Vec2 Vec4::xz() const {
	return Vec2(x, z);
}

inline Vec2 Vec4::zx() const {
	return Vec2(z, x);
}

inline Vec3 Vec4::xyz() const {
	return Vec3(x, y, z);
}

inline Vec3 Vec4::xzy() const {
	return Vec3(x, z, y);
}

inline Vec3 Vec4::yxz() const {
	return Vec3(y, x, z);
}

inline Vec3 Vec4::yzx() const {
	return Vec3(y, z, x);
}

inline Vec3 Vec4::zxy() const {
	return Vec3(z, x, y);
}

inline Vec3 Vec4::zyx() const {
	return Vec3(z, y, x);
}

inline bool Vec4::operator==(const Vec4 & _vec) const {
	return x == _vec.x && y == _vec.y && z == _vec.z && w == _vec.w;
}

inline bool Vec4::operator!=(const Vec4 & _vec) const {
	return !(*this == _vec);
}

inline float Length(const Vec4 & _vec) {
	return std::sqrt(LengthSqr(_vec));
}

inline float LengthSqr(const Vec4 & _vec) {
	return _vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z + _vec.w * _vec.w;
}

inline float Dot(const Vec4 & _v1, const Vec4 & _v2) {
	return _v1.x * _v2.x + _v1.y * _v2.y + _v1.z * _v2.z + _v1.w * _v2.w;
}

inline float Distance(const Vec4 & _v1, const Vec4 & _v2) {
	return Length(_v2 - _v1);
}

inline Vec4 operator*(float _v, const Vec4 & _vec) {
	return Vec4(_vec.x * _v, _vec.y * _v, _vec.z * _v, _vec.w * _v);
}

inline Vec4 Cross(const Vec4 & _v1, const Vec4 & _v2) {
	Vec4 res(_v1.y * _v2.z - _v1.z * _v2.y, _v1.z * _v2.x - _v1.x * _v2.z, _v1.x * _v2.y - _v1.y * _v2.x, 0.0f);
	VEC4_CHECK(CheckVec4(res));
	return res;
}

inline Vec4 Normalize(const Vec4 & _vec) {
	float len = Length(_vec);
	VEC4_CHECK(std::fabs(len) > FLT_MIN);
	float divLen = 1.0f / len;
	return _vec * divLen;
}

inline Vec4 Max(const Vec4 & _v1, const Vec4 & _v2) {
	return Vec4(std::max(_v1.x, _v2.x), std::max(_v1.y, _v2.y), std::max(_v1.z, _v2.z), std::max(_v1.w, _v2.w));
}

inline Vec4 Min(const Vec4 & _v1, const Vec4 & _v2) {
	return Vec4(std::min(_v1.x, _v2.x), std::min(_v1.y, _v2.y), std::min(_v1.z, _v2.z), std::min(_v1.w, _v2.w));
}

inline Vec4 Abs(const Vec4 & _vec) {
	return Vec4(std::abs(_vec.x), std::abs(_vec.y), std::abs(_vec.z), std::abs(_vec.w));
}

inline Vec4 Lerp(const Vec4 & _v1, const Vec4 & _v2, float _v) {
	return Vec4(Lerp(_v1.x, _v2.x, _v), Lerp(_v1.y, _v2.y, _v), Lerp(_v1.z, _v2.z, _v), Lerp(_v1.w, _v2.w, _v));
}

inline Vec4 Clamp(const Vec4 & _vec, const Vec4 & _min, const Vec4 & _max) {
	return Vec4(std::clamp(_vec.x, _min.x, _max.x), std::clamp(_vec.y, _min.y, _max.y), 
		std::clamp(_vec.z, _min.z, _max.z), std::clamp(_vec.w, _min.w, _max.w));
}

inline Vec4 Saturate(const Vec4 & _vec) {
	return Vec4(std::clamp(_vec.x, 0.0f, 1.0f), std::clamp(_vec.y, 0.0f, 1.0f), 
		std::clamp(_vec.z, 0.0f, 1.0f), std::clamp(_vec.w, 0.0f, 1.0f));
}
