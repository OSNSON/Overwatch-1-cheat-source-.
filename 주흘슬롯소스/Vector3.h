#pragma once
#pragma once
#include <math.h>
#include <Windows.h>
#include <DirectXMath.h>

using namespace DirectX;

class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3() { x = 0; y = 0; z = 0; return; }

	Vector3(float X, float Y, float Z) {
		x = X;
		y = Y;
		z = Z;
	}

	float Distance(Vector3 v)
	{
		return sqrtf(powf(v.x - x, 2.0f) + powf(v.y - y, 2.0f) + powf(v.z - z, 2.0f));
	}

	inline Vector3 operator+(Vector3 v)
	{
		return { x + v.x, y + v.y, z + v.z };
	}

	inline Vector3 operator-(Vector3 v)
	{
		return { x - v.x, y - v.y, z - v.z };
	}
};

class Vector2 {
public:
	float x, y;

	Vector2() { x = 0; y = 0; }

	Vector2(float X, float Y)
	{
		x = X;
		y = Y;
	}

	__forceinline float Distance(Vector2 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0)));
	}

	__forceinline float Size() const {
		return sqrt(x * x + y * y);
	}
};

struct MatrixTo {
	float m11, m12, m13, m14,
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m44;

	XMFLOAT3 GetCameraVec()
	{
		float	A = m22 * m33 - m32 * m23,
			B = m32 * m13 - m12 * m33,
			C = m12 * m23 - m22 * m13,
			Z = m11 * A + m21 * B + m31 * C;

		if (abs(Z) < 0.0001) return XMFLOAT3();

		float	D = m31 * m23 - m21 * m33,
			E = m11 * m33 - m31 * m13,
			F = m21 * m13 - m11 * m23,
			G = m21 * m32 - m31 * m22,
			H = m31 * m12 - m11 * m32,
			K = m11 * m22 - m21 * m12;

		return XMFLOAT3(-(A * m41 + D * m42 + G * m43) / Z, -(B * m41 + E * m42 + H * m43) / Z, -(C * m41 + F * m42 + K * m43) / Z);
	}

	bool WorldToScreen(XMFLOAT3 worldPos, XMFLOAT2* OutPos) {
		float screenX = (m11 * worldPos.x) + (m21 * worldPos.y) + (m31 * worldPos.z) + m41;
		float screenY = (m12 * worldPos.x) + (m22 * worldPos.y) + (m32 * worldPos.z) + m42;
		float screenW = (m14 * worldPos.x) + (m24 * worldPos.y) + (m34 * worldPos.z) + m44;

		float camX = 1920 / 2.0f;
		float camY = 1080 / 2.0f;

		float x = camX + (camX * screenX / screenW);
		float y = camY - (camY * screenY / screenW);

		OutPos->x = x;
		OutPos->y = y;

		return (screenW > 0.001f);
	}
};

struct Matrix {
	float m11, m12, m13, m14,
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m44;

	Vector3 GetCameraVec()
	{
		float	A = m22 * m33 - m32 * m23,
			B = m32 * m13 - m12 * m33,
			C = m12 * m23 - m22 * m13,
			Z = m11 * A + m21 * B + m31 * C;

		if (abs(Z) < 0.0001) return Vector3();

		float	D = m31 * m23 - m21 * m33,
			E = m11 * m33 - m31 * m13,
			F = m21 * m13 - m11 * m23,
			G = m21 * m32 - m31 * m22,
			H = m31 * m12 - m11 * m32,
			K = m11 * m22 - m21 * m12;

		return Vector3(-(A * m41 + D * m42 + G * m43) / Z, -(B * m41 + E * m42 + H * m43) / Z, -(C * m41 + F * m42 + K * m43) / Z);
	}

	Vector2 WorldToScreen(Vector3 worldPos, int width, int height)
	{
		float screenx = (m11 * worldPos.x) + (m21 * worldPos.y) + (m31 * worldPos.z) + m41;
		float screeny = (m12 * worldPos.x) + (m22 * worldPos.y) + (m32 * worldPos.z) + m42;
		float screenW = (m14 * worldPos.x) + (m24 * worldPos.y) + (m34 * worldPos.z) + m44;

		float camX = width / 2.0f;
		float camY = height / 2.0f;

		float x = camX + (camX * screenx / screenW);
		float y = camY - (camY * screeny / screenW);

		return (screenW > 0.001f) ? Vector2(x, y) : Vector2(-1, -1);
	}

	bool WorldToScreen(Vector3 worldPos, int width, int height, Vector2& OutPos) {
		float screenX = (m11 * worldPos.x) + (m21 * worldPos.y) + (m31 * worldPos.z) + m41;
		float screenY = (m12 * worldPos.x) + (m22 * worldPos.y) + (m32 * worldPos.z) + m42;
		float screenW = (m14 * worldPos.x) + (m24 * worldPos.y) + (m34 * worldPos.z) + m44;

		float camX = width / 2.0f;
		float camY = height / 2.0f;

		float x = camX + (camX * screenX / screenW);
		float y = camY - (camY * screenY / screenW);

		OutPos = { x, y };

		return (screenW > 0.001f);
	}

	bool IntersectRayWithAABB(MatrixTo viewMatrix, XMFLOAT3 origin, XMFLOAT3 direction, XMFLOAT3 min, XMFLOAT3 max, float scale, XMFLOAT3 basePos, bool isHead)
	{
		XMStoreFloat3(&min, XMLoadFloat3(&min) * scale);
		XMStoreFloat3(&max, XMLoadFloat3(&max) * scale);

		XMFLOAT3 points[] = {
			XMFLOAT3(min.x, min.y, min.z),
			XMFLOAT3(min.x, max.y, min.z),
			XMFLOAT3(max.x, max.y, min.z),
			XMFLOAT3(max.x, min.y, min.z),
			XMFLOAT3(max.x, max.y, max.z),
			XMFLOAT3(min.x, max.y, max.z),
			XMFLOAT3(min.x, min.y, max.z),
			XMFLOAT3(max.x, min.y, max.z)
		};

		XMFLOAT2 pointsTransformed[8];
		RECT minmaxTransformed{ 518523, 518523, 0, 0 };
		for (int i = 0; i < 8; i++)
		{
			XMStoreFloat3(&points[i], XMLoadFloat3(&points[i]) + XMLoadFloat3(&basePos));
			viewMatrix.WorldToScreen(points[i], &pointsTransformed[i]);
			minmaxTransformed.left = fmin(minmaxTransformed.left, pointsTransformed[i].x);
			minmaxTransformed.top = fmin(minmaxTransformed.top, pointsTransformed[i].y);
			minmaxTransformed.right = fmax(minmaxTransformed.right, pointsTransformed[i].x);
			minmaxTransformed.bottom = fmax(minmaxTransformed.bottom, pointsTransformed[i].y);
		}

		RECT rcTemp{}, crossHair{ 1920 / 2 - 1, 1080 / 2 - 1, 1920 / 2 + 1, 1080 / 2 + 1 };
		return IntersectRect(&rcTemp, &minmaxTransformed, &crossHair);
	}
};