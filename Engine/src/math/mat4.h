#pragma once

#include "vec3.h"
#include "vec4.h"

namespace prev {

	/*
	DirectX specifies matrices as a
	one-dimensional array listed in
	column-major order, i.e. with elements
	ordered like this:

		x  y  z    w

		m0 m4 m8  m12
		m1 m5 m9  m13
		m2 m6 m10 m14
		m3 m7 m11 m15

	*/

	class Mat4 {
	public:
		Vec4 x, y, z, w;

		Mat4();
		Mat4(Vec4 _x, Vec4 _y, Vec4 _z, Vec4 _w);
		Mat4(const Mat4 & _mat);
		Mat4(float _data[16]);

		Vec4 & operator[](unsigned int i) { ASSERT(i < 4); return (&x)[i]; }
		const Vec4 & operator[](unsigned int i) const { ASSERT(i < 4); return (&x)[i]; }

		float * GetFloatPtr() { return &x[0]; }

		Vec4 operator*(const Vec4 _vec) const;
		Vec3 operator*(const Vec3 _vec) const;
		Mat4 operator*(const Mat4 & _mat) const;
		Mat4 operator*(const float _val) const;

		Mat4 operator+(const Mat4 & _mat) const;
		Mat4 operator-(const Mat4 & _mat) const;

		void operator*=(const Mat4 & _mat);
		void operator*=(const float _val);

		void operator+=(const Mat4 & _mat);
		void operator-=(const Mat4 & _mat);

		bool operator==(const Mat4 & _mat) const;
		bool operator!=(const Mat4 & _mat) const;

	};

	Mat4 Transpose(const Mat4 & _mat);
	Mat4 Inverse3x3(const Mat4 & _mat);
	Mat4 Inverse4x4(const Mat4 & _mat);
	Mat4 inverseRT(const Mat4 & _mat);
	Mat4 Identity();
	Mat4 Zero();
	Mat4 Translate(const Mat4 & _mat, const Vec2 & _vec);
	Mat4 Translate(const Mat4 & _mat, const Vec3 & _vec);
	Mat4 Translate(const Mat4 & _mat, const Vec4 & _vec);
	Mat4 Scale(const Mat4 & _mat, const Vec3 & _vec);
	Mat4 Basis(const Vec3 & _x, const Vec3 & _y, const Vec3 & _z);
	Mat4 Rotate(const Mat4 & _mat, const float _angle, const Vec4 & _axis);
	Mat4 Rotate(const Mat4 & _mat, const float _angle, const Vec3 & _axis);
	Mat4 LookAt(const Vec3 & _eye, const Vec3 & _target, const Vec3 & _up);
	Mat4 Perspective(const float _fov, const float _aspect, const float _near, const float _far);
	Mat4 Frustum(const float _left, const float _right, const float _bottom, const float _top, const float _near, const float _far);
	Mat4 Ortho(const float _left, const float _right, const float _bottom, const float _top, const float _near, const float _far);

	Vec3 GetTranslation(const Mat4 & _mat);

	float Determinant3x3(const Mat4 & _mat);
	float ExtractFOV(const Mat4 & _mat);
	float ExtractAspect(const Mat4 & _mat);
	float ExtractZNear(const Mat4 & _mat);
	float ExtractZFar(const Mat4 & _mat);
	float ExtractFocalLength(const Mat4 & _mat);

	void CancelTranslation(Mat4 & _mat);

	#include "mat4.inl"

}
