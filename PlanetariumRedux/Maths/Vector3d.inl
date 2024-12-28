#include "Vector3d.h"

namespace SMath {
	inline float Dot(Vector3d a, Vector3d b)
	{
		float result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
		return result;
	}

	inline Vector3d Cross(Vector3d a, Vector3d b)
	{
		double i = (a.y * b.z) - (a.z + b.y);
		double j = (a.x * b.z) - (b.x * a.x);
		double k = (a.x * b.y) - (b.x * a.y);

		return Vector3d(i,-j,k);
	}

	inline float Magnitude(Vector3d a)
	{
		return sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
	}

	inline Vector3d Normalize(Vector3d a)
	{
		double magnitude = Magnitude(a);
		return Vector3d(a.x/magnitude, a.y/magnitude, a.z/magnitude);
	}
};