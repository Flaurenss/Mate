#pragma once

#include "Vector.h"

struct Matrix4
{
private:
	float m[16];
	const float DEG2RAD = 3.141593f / 180.0f;
	const float RAD2DEG = 180.0f / 3.141593f;
	const float EPSILON = 0.00001f;

public:
	/// <summary>
	/// Matrix constructor, by default sets the identity.
	/// </summary>
	Matrix4();

	Matrix4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13, 
		float m20, float m21, float m22, float m23, 
		float m30, float m31, float m32, float m33);

	Matrix4(float* v);
	
	const float* get() const;

	/// <summary>
	/// Returns the identity matrix:
	/// 1 0 0 0
	/// 0 1 0 0 
	/// 0 0 1 0 
	/// 0 0 0 1
	/// </summary>
	/// <returns></returns>
	Matrix4& identity();
	/// <summary>
	/// Translate operation based in:
	/// Mr = Mt * M
	/// Mr -> Matrix result.
	/// Mt -> Translate matrix.
	/// M -> Current matrix.
	/// </summary>
	/// <param name="vector">The vector that will be applied to the translation.</param>
	/// <returns>The matrix translated based on the provided vector.</returns>
	Matrix4& translate(const Vector3& v);
	
	/// <summary>
	/// Scale by a factor.
	/// </summary>
	/// <param name="f">The factor to scale.</param>
	/// <returns>The matrix scaled.</returns>
	Matrix4& scale(float f);

	/// <summary>
	/// Scale operation based in:
	/// Mr = Ms * M
	/// Mr -> Matrix result.
	/// Ms -> Scale matrix.
	/// M -> Current matrix.
	/// </summary>
	/// <param name="vector">The vector that will be applied to the translation.</param>
	/// <returns>The matrix translated based on the provided vector.</returns>
	Matrix4& scale(const Vector3& v);

	Matrix4& rotate(const float degrees, const Vector3 axis);


	Matrix4& ortho(const float left, const float right, const float bottom, const float top, const float near, const float far);
	Matrix4& perspective(const float fov, const float aspectRatio, const float near, const float far);

	static Matrix4 lookAt(const Vector3 position, const Vector3 target, const Vector3 up);
	static Matrix4 ToMatrix(const float x, const float y, const float z, const float w);
	Matrix4 inverse() const;

	// Operators
	float& operator[](int index);
	float operator[](const int index) const;

	Matrix4& operator=(const Matrix4& other);
	Vector4 operator*(const Vector4& v) const;
	Matrix4 operator*(const Matrix4& m) const;

	void print();
};

