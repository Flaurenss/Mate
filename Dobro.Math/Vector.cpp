#include "pch.h"
#include "Math.h"
#include "Vector.h"
#include <stdexcept>
#include <cassert>

#pragma region Vector3
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{ }

Vector3 Vector3::normalize() const
{
	float len = length();
	if (len == 0.0f) return Vector3(0, 0, 0); // Handle zero-length vector

	float invLen = 1.0f / len;
	return Vector3(x * invLen, y * invLen, z * invLen);
}

float Vector3::length() const
{
	return sqrtf((x * x) + (y * y) + (z * z));
}

float Vector3::dot(const Vector3& a, const Vector3& b) const
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vector3 Vector3::cross(const Vector3& a, const Vector3& b)
{
	return Vector3(
		(a.y * b.z) - (a.z * b.y),
		(a.z * b.x) - (a.x * b.z),
		(a.x * b.y) - (a.y * b.x)
	);
}

float& Vector3::operator[](int i)
{
	return ((&x)[i]);
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator+(float v) const
{
	return Vector3(x + v, y + v, z + v);
}

Vector3 Vector3::operator+(const Vector3& v) const
{
	return Vector3(
		x + v.x,
		y + v.y,
		z + v.z
	);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	return Vector3(
		x - v.x,
		y - v.y,
		z - v.z);
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3 Vector3::operator*(float v) const
{
	return Vector3(x*v, y*v, z*v);
}

Vector3 operator*(float scalar, const Vector3& v) {
	return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
}

Vector3 Vector3::operator*(const Vector3& v) const
{
	return Vector3(
		x * v.x,
		y * v.y,
		z * v.z
	);
}

Vector3& Vector3::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vector3 Vector3::operator/(Vector3& v) const
{
	return Vector3(
		x / v.x,
		y / v.y,
		z / v.z
	);
}
#pragma endregion Vector3
#pragma region Vector4
Vector4::Vector4(float x, float y, float z, float w) :
	x(x), y(y), z(z), w(w)
{ }

float Vector4::lenght() const
{
	return sqrtf(x*x + y*y + z*z + w*w);
}

float Vector4::dot(const Vector4& a, const Vector4& b) const
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

float& Vector4::operator[](int i)
{
	assert(i >= 0 && i < 4);
	switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: throw std::out_of_range("Index out of bounds");
	}
}

Vector4 Vector4::operator-() const
{
	return Vector4(-x, -y, -z, -w);
}

Vector4 Vector4::operator+(float v) const
{
	return Vector4(
		x + v, 
		y + v, 
		z + v, 
		w + v);
}

Vector4 Vector4::operator+(Vector4& v) const
{
	return Vector4(
			x + v.x,
			y + v.y,
			z + v.z,
			w + v.w
	);
}

Vector4 Vector4::operator-(Vector4& v) const
{
	return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4 Vector4::operator*(Vector4& v) const
{
	return Vector4(
		x * v.x,
		y * v.y,
		z * v.z,
		w * v.w
	);
}

Vector4 Vector4::operator/(Vector4& v) const
{
	return Vector4(
		x / v.x,
		y / v.y,
		z / v.z,
		w / v.w
	);
}
#pragma endregion Vector4
