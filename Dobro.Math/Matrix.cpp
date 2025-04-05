#include "pch.h"
#include "Math.h"
#include "Matrix.h"
#include <iostream>

Matrix4::Matrix4()
{
	identity();
}

Matrix4::Matrix4(float m00, float m01, float m02, float m03,
	            float m10, float m11, float m12, float m13,
	            float m20, float m21, float m22, float m23,
	            float m30, float m31, float m32, float m33)
{
	m[0] = m00;  m[4] = m10;  m[8] = m20;  m[12] = m30;
	m[1] = m01;  m[5] = m11;  m[9] = m21;  m[13] = m31;
	m[2] = m02;  m[6] = m12;  m[10] = m22;  m[14] = m32;
	m[3] = m03;  m[7] = m13;  m[11] = m23;  m[15] = m33;
}

Matrix4::Matrix4(float* v)
{
    m[0] = v[0];  m[4] = v[4];  m[8] = v[8];  m[12] = v[12];
    m[1] = v[1];  m[5] = v[5];  m[9] = v[9];  m[13] = v[13];
    m[2] = v[2];  m[6] = v[6];  m[10] = v[10];  m[14] = v[14];
    m[3] = v[3];  m[7] = v[7];  m[11] = v[11];  m[15] = v[15];
}

const float* Matrix4::get() const
{
	return m;
}

Matrix4& Matrix4::identity()
{
	m[0] = 1.0f;  m[4] = 0.0f;  m[8] = 0.0f;  m[12] = 0.0f;
	m[1] = 0.0f;  m[5] = 1.0f;  m[9] = 0.0f;  m[13] = 0.0f;
	m[2] = 0.0f;  m[6] = 0.0f;  m[10] = 1.0f;  m[14] = 0.0f;
	m[3] = 0.0f;  m[7] = 0.0f;  m[11] = 0.0f;  m[15] = 1.0f;
	return *this;
}


Matrix4& Matrix4::translate(const Vector3& v)
{
	m[0] += m[3] * v.x;   m[4] += m[7] * v.x;   m[8] += m[11] * v.x;   m[12] += m[15] * v.x;
	m[1] += m[3] * v.y;   m[5] += m[7] * v.y;   m[9] += m[11] * v.y;   m[13] += m[15] * v.y;
	m[2] += m[3] * v.z;   m[6] += m[7] * v.z;   m[10] += m[11] * v.z;   m[14] += m[15] * v.z;
	return *this;
}

Matrix4& Matrix4::scale(float f)
{
	scale(Vector3(f, f, f));
	return *this;
}

Matrix4& Matrix4::scale(const Vector3& v)
{
	m[0] *= v.x;   m[4] *= v.x;   m[8] *= v.x;   m[12] *= v.x;
	m[1] *= v.y;   m[5] *= v.y;   m[9] *= v.y;   m[13] *= v.y;
	m[2] *= v.z;   m[6] *= v.z;   m[10] *= v.z;   m[14] *= v.z;
	return *this;
}

Matrix4& Matrix4::rotate(const float degrees, const Vector3 axis)
{
    float cosTheta = cosf(degrees * DEG2RAD);
    float sinTheta = sinf(degrees * DEG2RAD);
    float oneMinusCos = 1.0f - cosTheta;

    // Save actual matrix status
    float m0 = m[0], m4 = m[4], m8 = m[8], m12 = m[12],
        m1 = m[1], m5 = m[5], m9 = m[9], m13 = m[13],
        m2 = m[2], m6 = m[6], m10 = m[10], m14 = m[14];

    // Compute rotation matrix elements (Rodrigues' rotation formula)
    float r0 = axis.x * axis.x * oneMinusCos + cosTheta;
    float r1 = axis.x * axis.y * oneMinusCos + axis.z * sinTheta;
    float r2 = axis.x * axis.z * oneMinusCos - axis.y * sinTheta;
    float r4 = axis.x * axis.y * oneMinusCos - axis.z * sinTheta;
    float r5 = axis.y * axis.y * oneMinusCos + cosTheta;
    float r6 = axis.y * axis.z * oneMinusCos + axis.x * sinTheta;
    float r8 = axis.x * axis.z * oneMinusCos + axis.y * sinTheta;
    float r9 = axis.y * axis.z * oneMinusCos - axis.x * sinTheta;
    float r10 = axis.z * axis.z * oneMinusCos + cosTheta;

    // Apply rotation matrix multiplication
    m[0] = r0 * m0 + r4 * m1 + r8 * m2;
    m[1] = r1 * m0 + r5 * m1 + r9 * m2;
    m[2] = r2 * m0 + r6 * m1 + r10 * m2;
    m[4] = r0 * m4 + r4 * m5 + r8 * m6;
    m[5] = r1 * m4 + r5 * m5 + r9 * m6;
    m[6] = r2 * m4 + r6 * m5 + r10 * m6;
    m[8] = r0 * m8 + r4 * m9 + r8 * m10;
    m[9] = r1 * m8 + r5 * m9 + r9 * m10;
    m[10] = r2 * m8 + r6 * m9 + r10 * m10;
    m[12] = r0 * m12 + r4 * m13 + r8 * m14;
    m[13] = r1 * m12 + r5 * m13 + r9 * m14;
    m[14] = r2 * m12 + r6 * m13 + r10 * m14;

    return *this;
}

Matrix4& Matrix4::transform(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
    Matrix4 identity;

    identity.scale(scale);

    identity.rotate(rotation.z, Vector3(0, 0, 1));
    identity.rotate(rotation.y, Vector3(0, 1, 0));
    identity.rotate(rotation.x, Vector3(1, 0, 0));

    identity.translate(position);

    return identity;
}

Matrix4& Matrix4::ortho(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
    // TODO: apply to current global m
    Matrix4 m;
    // Scaling
    m[0] = 2 / (right - left);
    m[5] = 2 / (top - bottom);
    m[10] = -2 / (far - near);
    // Translation
    m[12] = - (right + left) / (right - left);
    m[13] = - (top + bottom) / (top - bottom);
    m[14] = - (far + near) / (far - near);

    *this = m * (*this);

    return *this;
}

/// <summary>
/// Perspectice projection matrix.
/// For a clearer explanation please check:
/// https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/opengl-perspective-projection-matrix.html
/// </summary>
/// <param name="fov">The field of view.</param>
/// <param name="aspect">The aspect ratio.</param>
/// <param name="near">The near value.</param>
/// <param name="far">The far value.</param>
/// <returns>The perspective projection matrix.</returns>
Matrix4& Matrix4::perspective(const float fov, const float aspectRatio, const float near, const float far)
{
    Matrix4 p;
    auto rad = fov * DEG2RAD;
    auto top = tanf(rad * 0.5f) * near;
    auto bottom = -top;
    auto right = top * aspectRatio;
    auto left = -right;
    
    p[0] = (2 * near) / (right - left);
    
    p[5] = (2 * near) / (top - bottom);
    
    p[8] = (right + left) / (right - left);
    p[9] = (top + bottom) / (top - bottom);
    p[10] = -(far + near) / (far - near);
    p[11] = -1;
    
    p[14] = -(2 * far * near) / (far - near);
    p[15] = 0.0f;

    *this = p * (*this);

    return *this;
}

Matrix4 Matrix4::lookAt(const Vector3 position, const Vector3 target, const Vector3 up)
{
    auto zAxis = (position - target).normalize();
    auto xAxis = Vector3::cross(up.normalize(), zAxis).normalize();
    auto yAxis = Vector3::cross(zAxis, xAxis);

    Matrix4 rotation = Matrix4();
    Matrix4 translation = Matrix4();

    rotation[0] = xAxis.x;
    rotation[4] = xAxis.y;
    rotation[8] = xAxis.z;
    rotation[1] = yAxis.x;
    rotation[5] = yAxis.y;
    rotation[9] = yAxis.z;
    rotation[2] = zAxis.x;
    rotation[6] = zAxis.y;
    rotation[10] = zAxis.z;

    translation[12] = -position.x;
    translation[13] = -position.y;
    translation[14] = -position.z;

    return translation * rotation;
}

// Based on https://www.geometrictools.com/GTE/Mathematics/Rotation.h
Matrix4 Matrix4::ToMatrix(const float x, const float y, const float z, const float w)
{
    Matrix4 r;

    float twoX = 2 * x;
    float twoY = 2 * y;
    float twoZ = 2 * z;
    float twoXX = twoX * x;
    float twoXY = twoX * y;
    float twoXZ = twoX * z;
    float twoXW = twoX * w;
    float twoYY = twoY * y;
    float twoYZ = twoY * z;
    float twoYW = twoY * w;
    float twoZZ = twoZ * z;
    float twoZW = twoZ * w;

    r[0] = 1 - twoYY - twoZZ;
    r[4] = twoXY - twoZW;
    r[8] = twoXZ + twoYW;
    r[1] = twoXY + twoZW;
    r[5] = 1 - twoXX - twoZZ;
    r[9] = twoYZ - twoXW;
    r[2] = twoXZ - twoYW;
    r[6] = twoYZ + twoXW;
    r[10] = 1 - twoXX - twoYY;

    return r;
}

float& Matrix4::operator[](int index)
{
    return m[index];
}

float Matrix4::operator[](const int index) const
{
    return m[index];
}

Matrix4& Matrix4::operator=(const Matrix4& other)
{
    if (this != &other) 
    { // Prevent self-assignment
        for (int i = 0; i < 16; i++) {
            m[i] = other.m[i];
        }
    }
    return *this;
}

Vector4 Matrix4::operator*(const Vector4& v) const
{
	return Vector4(
		m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w,
		m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w,
		m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w,
		m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w);
}

Matrix4 Matrix4::operator*(const Matrix4& n)
{
    Matrix4 result;

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result[row * 4 + col] =
                m[row * 4 + 0] * n[0 * 4 + col] +
                m[row * 4 + 1] * n[1 * 4 + col] +
                m[row * 4 + 2] * n[2 * 4 + col] +
                m[row * 4 + 3] * n[3 * 4 + col];
        }
    }

    return result;
}

void Matrix4::print()
{
    std::cout << "Custom Rotation Matrix:" << std::endl;
    for (int i = 0; i < 16; i++) {
        std::cout << m[i] << ((i % 4 == 3) ? "\n" : " ");
    }
}
