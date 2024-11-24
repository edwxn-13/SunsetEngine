#pragma once

#include "MathHeader.h"

class Vector3d
{
public:
	Vector3d() { x = y = z = 0; }
	Vector3d(double X, double Y, double Z) { x = X; y = Y; z = Z; }
	Vector3d(double a) { x = y = z = a; }

	inline Vector3d operator- (const Vector3f& c) const
	{
		Vector3d result;
		result.x = this->x - (double)c.x;
		result.y = this->y - (double)c.y;
		result.z = this->z - (double)c.z;
		return result;
	}

	inline Vector3d operator+ (const Vector3f& c) const
	{
		Vector3d result = *this;
		result.x = this->x + (double)c.x;
		result.y = this->y + (double)c.y;
		result.z = this->z + (double)c.z;
		return result;
	}

	inline Vector3d operator- (const Vector3d& c) const
	{
		Vector3d result;
		result.x = this->x - c.x;
		result.y = this->y - c.y;
		result.z = this->z - c.z;
		return result;
	}

	inline Vector3d operator+ (const Vector3d& c) const
	{
		Vector3d result = *this;
		result.x = this->x + c.x;
		result.y = this->y + c.y;
		result.z = this->z + c.z;
		return result;
	}

	double x, y, z;
};
