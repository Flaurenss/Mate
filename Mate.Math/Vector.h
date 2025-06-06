#pragma once

struct Vector2
{
	float x, y;

	Vector2() = default;

	Vector2(float x, float y);
};

struct Vector3
{
	float x, y, z;

	Vector3() = default;

	Vector3(float v);
	Vector3(float x, float y, float z);

	// Utils
	Vector3 normalize() const;
	float length() const;
	static float dot(const Vector3& a, const Vector3& b);
	static Vector3 cross(const Vector3& a, const Vector3& b);

	// Operators
	bool operator==(const Vector3) const;
	bool operator!=(const Vector3) const;

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
	Vector3 operator/(float v) const;

	static const Vector3 Forward;
	static const Vector3 Back;
	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Right;
	static const Vector3 Left;
	static const Vector3 Zero;
	static const Vector3 One;

};

struct Vector4
{
	float x, y, z, w;

	Vector4() = default;

	Vector4(float x, float y, float z, float w);

	Vector4(Vector3 v3, float w = 0);

	// Utils
	Vector4 normalize() const;
	float length() const;
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

