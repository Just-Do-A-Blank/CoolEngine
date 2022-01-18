#pragma once
#include <DirectXMath.h>

using namespace DirectX;

static class MathHelper
{
public:
	//Adds var2 to var1 and return it
	static XMFLOAT3& Plus(const XMFLOAT3& var1, const XMFLOAT3& var2)
	{
		return XMFLOAT3(var1.x + var2.x, var1.y + var2.y, var1.z + var2.z);
	}

	//Subtracts var1 with var2 and return it
	static XMFLOAT3& Minus(const XMFLOAT3& var1, const XMFLOAT3& var2)
	{
		return XMFLOAT3(var1.x - var2.x, var1.y - var2.y, var1.z - var2.z);
	}

	//Adds var2 to var1 and stores in var1
	static void PlusEquals(XMFLOAT3& var1, const XMFLOAT3& var2)
	{
		var1 = Minus(var1, var2);
	}

	//Subtracts var2 from var1 and stores in var1
	static void MinusEquals(XMFLOAT3& var1, const XMFLOAT3& var2)
	{
		var1 = Plus(var1, var2);
	}

	//Multiplies var1 with var2 and returns it
	static XMFLOAT3& Multiply(XMFLOAT3 var1, const float& value)
	{
		return XMFLOAT3(var1.x * value, var1.y * value, var1.z * value);
	}

	//Multiplies var1 with var2 and stores in var1
	static void MultiplyEquals(XMFLOAT3& var1, const float& value)
	{
		var1 = Multiply(var1, value);
	}

	//Divides var1 by value and returns it.
	static XMFLOAT3& Divide(XMFLOAT3 var1, const float& value)
	{
		return XMFLOAT3(var1.x * value, var1.y * value, var1.z * value);
	}

	//Divides var1 by value and stores in var1
	static void DivideEquals(XMFLOAT3& var1, const float& value)
	{
		var1 = Divide(var1, value);
	}

	//Returns dot product of var1 with var2
	static float DotProduct(const XMFLOAT3& var1, const XMFLOAT3& var2)
	{
		return (var1.x * var2.x) + (var1.y * var2.y) + (var1.z * var2.z);
	}

	//Return cross product of var1 with var2
	static XMFLOAT3& CrossProduct(const XMFLOAT3& var1, const XMFLOAT3& var2)
	{
		float nx = (var1.y * var2.z) - (var1.z * var2.y);
		float ny = (var1.z * var2.x) - (var1.x * var2.z);
		float nz = (var1.x * var2.y) - (var1.y * var2.x);

		return XMFLOAT3(nx, ny, nz);
	}

	//Return magnitude of var1
	static float Magnitude(const XMFLOAT3& var1)
	{
		return sqrt((var1.x * var1.x) + (var1.y * var1.y) + (var1.z * var1.z));
	}

	//Returns square magnitude of var1
	static float SquareMagnitude(const XMFLOAT3& var1)
	{
		return (var1.x * var1.x) + (var1.y * var1.y) + (var1.z * var1.z);
	}

	//Retruns normalized var1
	static XMFLOAT3& Normalize(const XMFLOAT3& var1)
	{
		float mag = Magnitude(var1);
		assert(mag != 0);
		return XMFLOAT3(var1.x / mag, var1.y / mag, var1.z / mag);
	}

	//Returns distance from var1 to var2
	static float Distance(const XMFLOAT3& var1, const XMFLOAT3& var2)
	{
		return sqrt(((var1.x - var2.x) * (var1.x - var2.x)) + ((var1.y - var2.y) * (var1.y - var2.y)) + ((var1.z - var2.z) * (var1.z - var2.z)));
	}

	//Returns squared distance from var1 to var2
	static float DistanceSquared(const XMFLOAT3& var1, const XMFLOAT3& var2)
	{
		return ((var1.x - var2.x) * (var1.x - var2.x)) + ((var1.y - var2.y) * (var1.y - var2.y)) + ((var1.z - var2.z) * (var1.z - var2.z));
	}
};