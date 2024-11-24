#include "Vector3d.h"

namespace SMath {
	inline float Dot(Vector3d a, Vector3d b)
	{
		float result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
		return result;
	}

	inline float Magnitude(Vector3d a)
	{
		return sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
	}
};