
inline Mat4::Mat4() : 
	x(1.0f, 0.0f, 0.0f, 0.0f),
	y(0.0f, 1.0f, 0.0f, 0.0f),
	z(0.0f, 0.0f, 1.0f, 0.0f),
	w(0.0f, 0.0f, 0.0f, 1.0f) {}

inline Mat4::Mat4(Vec4 _x, Vec4 _y, Vec4 _z, Vec4 _w) :
	x(_x), y(_y), z(_z), w(_w) {}

inline Mat4::Mat4(const Mat4 & _mat) :
	x(_mat.x), y(_mat.y), z(_mat.z), w(_mat.w) {}

inline Mat4::Mat4(float _data[16]) :
	x(_data[0], _data[1], _data[2], _data[3]),
	y(_data[4], _data[5], _data[6], _data[7]),
	z(_data[8], _data[9], _data[10], _data[11]),
	w(_data[12], _data[13], _data[14], _data[15]) {}

inline Vec4 Mat4::operator*(const Vec4 _vec) const {
	Mat4 trasposed = Transpose(*this);
	return Vec4(
		Dot(trasposed.x, _vec),
		Dot(trasposed.y, _vec),
		Dot(trasposed.z, _vec),
		Dot(trasposed.w, _vec)
	);
}

inline Vec3 Mat4::operator*(const Vec3 _vec) const {
	Vec4 result = *this * Vec4(_vec);
	return Vec3(result.x, result.y, result.z);
}

inline Mat4 Mat4::operator*(const Mat4 & _mat) const {
	Mat4 t = Transpose(*this);

	Vec4 vecX = Vec4(Dot(t.x, _mat.x), Dot(t.y, _mat.x), Dot(t.z, _mat.x), Dot(t.w, _mat.x));
	Vec4 vecY = Vec4(Dot(t.x, _mat.y), Dot(t.y, _mat.y), Dot(t.z, _mat.y), Dot(t.w, _mat.y));
	Vec4 vecZ = Vec4(Dot(t.x, _mat.z), Dot(t.y, _mat.z), Dot(t.z, _mat.z), Dot(t.w, _mat.z));
	Vec4 vecW = Vec4(Dot(t.x, _mat.w), Dot(t.y, _mat.w), Dot(t.z, _mat.w), Dot(t.w, _mat.w));

	return Mat4(vecX, vecY, vecZ, vecW);
}

inline Mat4 Mat4::operator*(const float _val) const {
	return Mat4(x * _val, y * _val, z * _val, w * _val);
}

inline Mat4 Mat4::operator+(const Mat4 & _mat) const {
	return Mat4(x + _mat.x, y + _mat.y, z + _mat.z, w + _mat.w);
}

inline Mat4 Mat4::operator-(const Mat4 & _mat) const {
	return Mat4(x - _mat.x, y - _mat.y, z - _mat.z, w - _mat.w);
}

inline void Mat4::operator*=(const Mat4 & _mat) {
	*this = *this * _mat;
}

inline void Mat4::operator*=(const float _val) {
	*this = *this * _val;
}

inline void Mat4::operator+=(const Mat4 & _mat) {
	*this = *this + _mat;
}

inline void Mat4::operator-=(const Mat4 & _mat) {
	*this = *this - _mat;
}

inline bool Mat4::operator==(const Mat4 & _mat) const {
	return x == _mat.x && y == _mat.y && z == _mat.z && w == _mat.w;
}

inline bool Mat4::operator!=(const Mat4 & _mat) const {
	return !(*this == _mat);
}

inline Mat4 Transpose(const Mat4 & _mat) {
	Mat4 result;
	result.x = Vec4(_mat.x.x, _mat.y.x, _mat.z.x, _mat.w.x);
	result.y = Vec4(_mat.x.y, _mat.y.y, _mat.z.y, _mat.w.y);
	result.z = Vec4(_mat.x.z, _mat.y.z, _mat.z.z, _mat.w.z);
	result.w = Vec4(_mat.x.w, _mat.y.w, _mat.z.w, _mat.w.w);
	return result;
}

inline Mat4 Identity() {
	return Mat4();
}

inline Mat4 Zero() {
	return Mat4(Vec4(0.0f), Vec4(0.0f), Vec4(0.0f), Vec4(0.0f));
}

inline Mat4 Translate(const Mat4 & _mat, const Vec2 & _vec) {
	Mat4 result = _mat;
	result.w.x += _vec.x;
	result.w.y += _vec.y;
	return result;
}

inline Mat4 Translate(const Mat4 & _mat, const Vec3 & _vec) {
	Mat4 result = _mat;
	result.w.x += _vec.x;
	result.w.y += _vec.y;
	result.w.z += _vec.z;
	return result;
}

inline Mat4 Translate(const Mat4 & _mat, const Vec4 & _vec) {
	return Translate(_mat, Vec3(_vec.x, _vec.y, _vec.z));
}

inline Mat4 Scale(const Mat4 & _mat, const Vec3 & _vec) {
	Mat4 result = _mat;
	result.x.x *= _vec.x;
	result.y.y *= _vec.y;
	result.z.z *= _vec.z;
	return result;
}

inline Mat4 Basis(const Vec3 & _x, const Vec3 & _y, const Vec3 & _z) {
	return Mat4(
		Vec4(_x.x, _x.y, _x.z, 0.0f),
		Vec4(_y.x, _y.y, _y.z, 0.0f),
		Vec4(_z.x, _z.y, _z.z, 0.0f),
		Vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

inline Mat4 Rotate(const Mat4 & _mat, const float _angle, const Vec4 & _axis) {
	return Rotate(_mat, _angle, Vec3(_axis.x, _axis.y, _axis.z));
}

inline Mat4 Rotate(const Mat4 & _mat, const float _angle, const Vec3 & _axis) {
	const float c = Cos(_angle);
	const Vec3 s(_axis * Sin(_angle));
	const Vec3 t(_axis * (1.0f - c));

	Mat4 rotationMat(
		Vec4(t.x * _axis.x + c   , t.y * _axis.x + s.z , t.z * _axis.x - s.y , 0.0f),
		Vec4(t.x * _axis.y - s.z , t.y * _axis.y + c   , t.z * _axis.y + s.x , 0.0f),
		Vec4(t.x * _axis.z + s.y , t.y * _axis.z - s.x , t.z * _axis.z + c   , 0.0f),   
		Vec4(0.0f,0.0f,0.0f,1.0f)
	);

	return _mat * rotationMat;
}

inline Mat4 LookAt(const Vec3 & _eye, const Vec3 & _target, const Vec3 & _up) {
	Vec3 vecZ = Normalize(_eye - _target);
	Vec3 vecX = Normalize(Cross(_up, vecZ));
	Vec3 vecY = Normalize(Cross(vecZ, vecX));

	return Mat4(Vec4(vecX, 0.0f), Vec4(vecY, 0.0f), Vec4(vecZ, 0.0f), Vec4(_eye, 1.0f));
}

inline Mat4 Perspective(const float _fov, const float _aspect, const float _near, const float _far) {
	const float t = _near * Tan(_fov * 0.5f);
	const float b = -t;
	const float r = t * _aspect;
	const float l = -r;

	return Frustum(l, r, b, t, _near, _far);
}

inline Mat4 Frustum(const float _left, const float _right, const float _bottom, const float _top, const float _near, const float _far) {
	const float E = 2.f * _near / (_right - _left);
	const float F = 2.f * _near / (_top - _bottom);

	const float A = (_right + _left) / (_right - _left);
	const float B = (_top + _bottom) / (_top - _bottom);
	const float C = -(_far + _near) / (_far - _near);
	const float D = -2.f * _far * _near / (_far - _near);

	return Mat4(
		Vec4(E  , 0.f, 0.f, 0.f),
		Vec4(0.f, F  , 0.f, 0.f),
		Vec4(A  , B  , C  ,-1.f),
		Vec4(0.f, 0.f, D  , 0.f)
	);
}

inline Mat4 Ortho(const float _left, const float _right, const float _bottom, const float _top, const float _near, const float _far) {
	const float r_minus_l_inv = 1.f / (_right - _left);
	const float t_minus_b_inv = 1.f / (_top - _bottom);
	const float n_minus_f_inv = 1.f / (_near - _far);

	return Mat4(
		Vec4(2.f * r_minus_l_inv, 0.f, 0.f, 0.f),
		Vec4(0.f, 2.f * t_minus_b_inv, 0.f, 0.f),
		Vec4(0.f, 0.f, 2.f * n_minus_f_inv, 0.f),
		Vec4(
			-(_right + _left)   * r_minus_l_inv,
			-(_top   + _bottom) * t_minus_b_inv,
			 (_far   + _near)   * n_minus_f_inv, 1.f));
}

inline Vec3 GetTranslation(const Mat4 & _mat) {
	return Vec3(_mat.w.x, _mat.w.y, _mat.w.z);
}

inline float Determinant3x3(const Mat4 & _mat) {
	return 
		_mat.x.x * _mat.y.y * _mat.z.z +
		_mat.y.x * _mat.z.y * _mat.x.z +
		_mat.z.x * _mat.x.y * _mat.y.z -
		_mat.z.x * _mat.y.y * _mat.x.z -
		_mat.y.x * _mat.x.y * _mat.z.z -
		_mat.x.x * _mat.z.y * _mat.y.z;
}

inline float ExtractFOV(const Mat4 & _mat) {
	return 2.0f * ATan(1.0f / _mat.y.y);
}

inline float ExtractAspect(const Mat4 & _mat) {
	return _mat.y.y / _mat.x.x;
}

inline float ExtractZNear(const Mat4 & _mat) {
	return _mat.w.z / (_mat.z.z - 1.0f);
}

inline float ExtractZFar(const Mat4 & _mat) {
	return _mat.w.z / (_mat.z.z + 1.0f);
}

inline float ExtractFocalLength(const Mat4 & _mat) {
	return _mat.x.x;
}

inline void CancelTranslation(Mat4 & _mat) {
	_mat.w = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
}