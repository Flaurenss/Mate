#pragma once

struct Vector3
{
	float x, y, z;

	Vector3() = default;

	Vector3(float x, float y, float z);

	// Utils
	Vector3 normalize() const;
	float length() const;
	float dot(const Vector3& a, const Vector3& b) const;
	static Vector3 cross(const Vector3& a, const Vector3& b);

	// Operators
	float& operator[](int i);
	Vector3 operator-() const;
	
	Vector3 operator+(float v) const;
	Vector3 operator+(const Vector3& v) const;
	Vector3& operator+=(const Vector3& v);
	
	Vector3 operator-(const Vector3& v) const;
	Vector3& operator-=(const Vector3& v);

	Vector3 operator*(float v) const;
	friend Vector3 operator*(float scalar, const Vector3& v);
	Vector3 operator*(const Vector3& v) const;
	Vector3& operator*=(const Vector3& v);
	
	Vector3 operator/(Vector3& v) const;

};

struct Vector4
{
	float x, y, z, w;

	Vector4() = default;

	Vector4(float x, float y, float z, float w);

	// Utils
	float lenght() const;
	float dot(const Vector4& a, const Vector4& b) const;

	// Operators
	float& operator[](int i);
	float operator[](int i) const;
	Vector4 operator-() const;
	Vector4 operator+(float v) const;
	Vector4 operator+(Vector4& v) const;
	Vector4 operator-(Vector4& v) const;
	Vector4 operator*(Vector4& v) const;
	Vector4 operator/(Vector4& v) const;
};

