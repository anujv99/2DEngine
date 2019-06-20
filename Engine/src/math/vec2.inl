#define Vec2_CHECK(v) ASSERT(v)

inline Vec2::Vec2() : x(0.0f), y(0.0f) {}
inline Vec2::Vec2(float _v) : x(_v), y(_v) {}
inline Vec2::Vec2(gmvec2 _vec) : x(_vec.x), y(_vec.y) {}
inline Vec2::Vec2(float _x, float _y) : x(_x), y(_y) {}

inline bool CheckVec2(const Vec2 & _vec) {
	return _vec.x <= FLT_MAX && _vec.x >= -FLT_MAX
		&& _vec.y <= FLT_MAX && _vec.y >= -FLT_MAX;
}

inline Vec2 & Vec2::operator+=(const Vec2 & _vec) {
	x += _vec.x;
	y += _vec.y;
	Vec2_CHECK(CheckVec2(*this));
	return *this;
}

inline Vec2 & Vec2::operator-=(const Vec2 & _vec) {
	x -= _vec.x;
	y -= _vec.y;
	Vec2_CHECK(CheckVec2(*this));
	return *this;
}

inline Vec2 & Vec2::operator*=(const Vec2 & _vec) {
	x *= _vec.x;
	y *= _vec.y;
	Vec2_CHECK(CheckVec2(*this));
	return *this;
}

inline Vec2 & Vec2::operator/=(const Vec2 & _vec) {
	Vec2_CHECK(std::abs(_vec.x) > FLT_MIN);
	Vec2_CHECK(std::abs(_vec.y) > FLT_MIN);

	x /= _vec.x;
	y /= _vec.y;
	Vec2_CHECK(CheckVec2(*this));
	return *this;
}

inline Vec2 & Vec2::operator+=(float _v) {
	x += _v;
	y += _v;
	Vec2_CHECK(CheckVec2(*this));
	return *this;
}

inline Vec2 & Vec2::operator-=(float _v) {
	x -= _v;
	y -= _v;
	Vec2_CHECK(CheckVec2(*this));
	return *this;
}

inline Vec2 & Vec2::operator*=(float _v) {
	x *= _v;
	y *= _v;
	Vec2_CHECK(CheckVec2(*this));
	return *this;
}

inline Vec2 & Vec2::operator/=(float _v) {
	float div = 1.0f / _v;

	x *= div;
	y *= div;
	Vec2_CHECK(CheckVec2(*this));
	return *this;
}

inline Vec2 Vec2::operator+(const Vec2 & _vec) const {
	Vec2_CHECK(CheckVec2(*this));
	Vec2_CHECK(CheckVec2(_vec));
	return Vec2(x + _vec.x, y + _vec.y);
}

inline Vec2 Vec2::operator-(const Vec2 & _vec) const {
	Vec2_CHECK(CheckVec2(*this));
	Vec2_CHECK(CheckVec2(_vec));
	return Vec2(x - _vec.x, y - _vec.y);
}

inline Vec2 Vec2::operator-() const {
	Vec2_CHECK(CheckVec2(*this));
	return Vec2(-x, -y);
}

inline Vec2 Vec2::operator*(const Vec2 & _vec) const {
	Vec2_CHECK(CheckVec2(*this));
	Vec2_CHECK(CheckVec2(_vec));
	return Vec2(x * _vec.x, y * _vec.y);
}

inline Vec2 Vec2::operator/(const Vec2 & _vec) const {
	Vec2_CHECK(CheckVec2(*this));
	Vec2_CHECK(CheckVec2(_vec));
	Vec2_CHECK(std::abs(_vec.x) > FLT_MIN);
	Vec2_CHECK(std::abs(_vec.y) > FLT_MIN);
	return Vec2(x / _vec.x, y / _vec.y);
}

inline Vec2 Vec2::operator*(float _v) const {
	Vec2_CHECK(CheckVec2(*this));
	return Vec2(x * _v, y * _v);
}

inline Vec2 Vec2::operator/(float _v) const {
	Vec2_CHECK(std::abs(_v) > FLT_MIN);
	Vec2_CHECK(CheckVec2(*this));
	float div = 1.0f / _v;
	return Vec2(x * div, y * div);
}

inline bool Vec2::operator==(const Vec2 & _vec) const {
	return x == _vec.x && y == _vec.y;
}

inline bool Vec2::operator!=(const Vec2 & _vec) const {
	return !(*this == _vec);
}

inline float Length(const Vec2 & _vec) {
	return std::sqrt(LengthSqr(_vec));
}

inline float LengthSqr(const Vec2 & _vec) {
	return (_vec.x * _vec.x) + (_vec.y * _vec.y);
}

inline float Dot(const Vec2 & _v1, const Vec2 & _v2) {
	return (_v1.x * _v2.x) + (_v1.y + _v2.y);
}

inline float Distance(const Vec2 & _v1, const Vec2 & _v2) {
	return Length(_v2 - _v1);
}

inline Vec2 operator*(float _v, const Vec2 & _vec) {
	return Vec2(_vec.x * _v, _vec.y * _v);
}

inline Vec2 Normalize(const Vec2 & _vec) {
	float len = Length(_vec);
	Vec2_CHECK(std::abs(len) > FLT_MIN);
	float divLen = 1.0f / len;
	return _vec * divLen;
}

inline Vec2 Max(const Vec2 & _v1, const Vec2 & _v2) {
	return Vec2(std::max(_v1.x, _v2.x), std::max(_v1.y, _v2.y));
}

inline Vec2 Min(const Vec2 & _v1, const Vec2 & _v2) {
	return Vec2(std::min(_v1.x, _v2.x), std::min(_v1.y, _v2.y));
}

inline Vec2 Abs(const Vec2 & _vec) {
	return Vec2(std::abs(_vec.x), std::abs(_vec.y));
}

inline Vec2 Lerp(const Vec2 & _v1, const Vec2 & _v2, float _v) {
	return Vec2(Lerp(_v1.x, _v2.x, _v), Lerp(_v1.y, _v2.y, _v));
}

inline Vec2 Clamp(const Vec2 & _vec, const Vec2 & _min, const Vec2 & _max) {
	return Vec2(std::clamp(_vec.x, _min.x, _max.y), std::clamp(_vec.y, _min.y, _max.y));
}

inline Vec2 Saturate(const Vec2 & _vec) {
	return Vec2(std::clamp(_vec.x, 0.0f, 1.0f), std::clamp(_vec.y, 0.0f, 1.0f));
}

inline Vec2 Perpendicular(const Vec2 & _vec) {
	return Vec2(-_vec.y, _vec.x);
}

inline Vec2 UnitCircle(float _angle) {
	return Vec2(prev::Cos(_angle), prev::Sin(_angle));
}

inline Vec2 Hermite(const Vec2 & _v1, const Vec2 & _tan1, const Vec2 & _v2, const Vec2 & _tan2, float _val) {
	float val_cubed = _val * _val * _val;
	float val_sqr = _val * _val;

	float h1 = 2.0f * val_cubed - 3.0f * val_sqr + 1.0f;   // calculate basis function 1
	float h2 = -2.0f * val_cubed + 3.0f * val_sqr;          // calculate basis function 2
	float h3 = val_cubed - 2.0f * val_sqr + _val;			// calculate basis function 3
	float h4 = val_cubed - val_sqr;					// calculate basis function 4

	return h1 * _v1 + h2 * _v2 + h3 * _tan1 + h4 * _tan2;
}