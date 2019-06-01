inline Vec2i::Vec2i() : x(0), y(0) {}
inline Vec2i::Vec2i(int _v) : x(_v), y(_v) {}
inline Vec2i::Vec2i(int _x, int _y) : x(_x), y(_y) {}

inline Vec2i & Vec2i::operator+=(const Vec2i & _vec) {
	x += _vec.x;
	y += _vec.y;
	return *this;
}

inline Vec2i & Vec2i::operator-=(const Vec2i & _vec) {
	x -= _vec.x;
	y -= _vec.y;
	return *this;
}

inline Vec2i & Vec2i::operator*=(const Vec2i & _vec) {
	x *= _vec.x;
	y *= _vec.y;
	return *this;
}

inline Vec2i & Vec2i::operator/=(const Vec2i & _vec) {
	ASSERT(_vec.x && _vec.y, "division by zero");
	x /= _vec.x;
	y /= _vec.y;
	return *this;
}

inline Vec2i & Vec2i::operator+=(int _v) {
	x += _v;
	y += _v;
	return *this;
}

inline Vec2i & Vec2i::operator-=(int _v) {
	x -= _v;
	y -= _v;
	return *this;
}

inline Vec2i & Vec2i::operator*=(int _v) {
	x *= _v;
	y *= _v;
	return *this;
}

inline Vec2i & Vec2i::operator/=(int _v) {
	ASSERT(_v, "division by zero");
	x /= _v;
	y /= _v;
	return *this;
}

inline Vec2i Vec2i::operator+(const Vec2i & _vec) const {
	return Vec2i(x + _vec.x, y + _vec.y);
}

inline Vec2i Vec2i::operator-(const Vec2i & _vec) const {
	return Vec2i(x - _vec.x, y - _vec.y);
}

inline Vec2i Vec2i::operator-() const {
	return Vec2i(-x, -y);
}

inline Vec2i Vec2i::operator*(const Vec2i & _vec) const {
	return Vec2i(x * _vec.x, y * _vec.y);
}

inline Vec2i Vec2i::operator/(const Vec2i & _vec) const {
	ASSERT(_vec.x && _vec.y, "division by zero");
	return Vec2i(x / _vec.x, y / _vec.y);
}

inline Vec2i Vec2i::operator*(int _v) const {
	return Vec2i(x * _v, y * _v);
}

inline Vec2i Vec2i::operator/(int _v) const {
	ASSERT(_v, "division by zero");
	return Vec2i(x / _v, y / _v);
}

inline bool Vec2i::operator==(const Vec2i & _vec) const {
	return x == _vec.x && y == _vec.y;
}

inline bool Vec2i::operator!=(const Vec2i & _vec) const {
	return !(*this == _vec);
}

inline float Length(const Vec2i & _vec) {
	return (float)std::sqrt(LengthSqr(_vec));
}

inline int LengthSqr(const Vec2i & _vec) {
	return (_vec.x * _vec.x) + (_vec.y * _vec.y);
}

inline int Dot(const Vec2i & _v1, const Vec2i & _v2) {
	return (_v1.x * _v2.x) + (_v1.y + _v2.y);
}

inline float Distance(const Vec2i & _v1, const Vec2i & _v2) {
	return Length(_v2 - _v1);
}

inline Vec2i operator*(int _v, const Vec2i & _vec) {
	return Vec2i(_vec.x * _v, _vec.y * _v);
}

inline Vec2i Max(const Vec2i & _v1, const Vec2i & _v2) {
	return Vec2i(std::max(_v1.x, _v2.x), std::max(_v1.y, _v2.y));
}

inline Vec2i Min(const Vec2i & _v1, const Vec2i & _v2) {
	return Vec2i(std::min(_v1.x, _v2.x), std::min(_v1.y, _v2.y));
}

inline Vec2i Abs(const Vec2i & _vec) {
	return Vec2i(std::abs(_vec.x), std::abs(_vec.y));
}

inline Vec2i Lerp(const Vec2i & _v1, const Vec2i & _v2, float _v) {
	int xVal = _v1.x + int((_v2.x - _v1.x) * _v);
	int yVal = _v1.y + int((_v2.y - _v1.y) * _v);
}

inline Vec2i Clamp(const Vec2i & _vec, const Vec2i & _min, const Vec2i & _max) {
	return Vec2i(std::clamp(_vec.x, _min.x, _max.y), std::clamp(_vec.y, _min.y, _max.y));
}

inline Vec2i Saturate(const Vec2i & _vec) {
	return Vec2i(std::clamp(_vec.x, 0, 1), std::clamp(_vec.y, 0, 1));
}

inline Vec2i Perpendicular(const Vec2i & _vec) {
	return Vec2i(-_vec.y, _vec.x);
}