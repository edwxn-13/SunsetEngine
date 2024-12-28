#pragma once

#include "MathHeader.h"

/*
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
};*/
class Vector3d;
namespace SMath
{
	inline float Dot(Vector3d a, Vector3d b);
	inline Vector3d Cross(Vector3d a, Vector3d b);
	inline float Magnitude(Vector3d a);
	inline Vector3d Normalize(Vector3d a);
};


class Vector3d
{
public:

	Vector3d() { x = y = z = 0; }

	Vector3d(Vector2f a) { x = a.x; y = a.y; z = 0; }

	Vector3d(Vector2f a, double b) { x = a.x; y = a.y; z = b; }

	Vector3d(double f) { x = y = z = f; }

	Vector3d(double X, double Y, double Z) { x = X; y = Y; z = Z; }

	Vector3d(glm::vec3 v) { x = v.x; y = v.y; z = v.z; }

	Vector3d(glm::vec4 v) { x = v.x; y = v.y; z = v.z; }


	Vector3d(Vector3f v) { x = v.x; y = v.y; z = v.z; }

	//Vector3d(Vector3d v) { x = v.x; y = v.y; z = v.z; }

	glm::vec3 glm() { return glm::vec3(x, y, z); }

	glm::vec4 glm4() { return glm::vec4(x, y, z, 1.0f); }

	// Operators

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

	inline Vector3d operator+= (const glm::vec3& c) const
	{
		Vector3d result;
		result.x = this->x + c.x;
		result.y = this->y + c.y;
		result.z = this->z + c.z;
		return result;
	}

	inline Vector3d operator-= (const glm::vec3& c) const
	{
		Vector3d result;
		result.x = this->x - c.x;
		result.y = this->y - c.y;
		result.z = this->z - c.z;
		return result;
	}

	inline Vector3d operator+= (const Vector3d& c) const
	{
		Vector3d result = *this;
		result.x = this->x + c.x;
		result.y = this->y + c.y;
		result.z = this->z + c.z;
		return result;
	}

	inline Vector3d operator-= (const Vector3d& c) const
	{
		Vector3d result;
		result.x = this->x - c.x;
		result.y = this->y - c.y;
		result.z = this->z - c.z;
		return result;
	}

	inline Vector3d operator+ (const Vector3d& c) const
	{
		Vector3d result;
		result.x = this->x + c.x;
		result.y = this->y + c.y;
		result.z = this->z + c.z;
		return result;
	}

	inline Vector3d operator+ (const double& c) const
	{
		Vector3d result;
		result.x = this->x + c;
		result.y = this->y + c;
		result.z = this->z + c;
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

	inline Vector3d operator- () const
	{
		Vector3d result;
		result.x = -x;
		result.y = -y;
		result.z = -z;
		return result;
	}

	inline Vector3d operator- (const double& c) const
	{
		Vector3d result;
		result.x = this->x - c;
		result.y = this->y - c;
		result.z = this->z - c;
		return result;
	}

	inline Vector3d operator* (const double& c) const
	{
		Vector3d result;
		result.x = this->x * c;
		result.y = this->y * c;
		result.z = this->z * c;
		return result;
	}

	inline Vector3d operator* (const Vector3d& c) const
	{
		Vector3d result;
		result.x = this->x * c.x;
		result.y = this->y * c.y;
		result.z = this->z * c.z;
		return result;
	}

	inline Vector3d operator/ (const double& c) const
	{
		Vector3d result;
		result.x = this->x / c;
		result.y = this->y / c;
		result.z = this->z / c;
		return result;
	}


	bool operator == (const Vector3d& c) const
	{
		if (c.x == x && c.y == y && c.z == z) { return true; }
		return false;
	}

	double magnitude()
	{
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	void Normalize()
	{
		double magnitude = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;
	}

	Vector3f f() 
	{
		return Vector3f(x,y,z);
	}

	Vector3d normal()
	{
		double magnitude = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;
		return Vector3d(x, y, z);
	}
	double x, y, z;
};

#include "Vector3d.inl"