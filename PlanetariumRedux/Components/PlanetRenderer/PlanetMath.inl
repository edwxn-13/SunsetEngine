
inline p_vec3 normalize(p_vec3 vec)
{
	float mag = magnitude(vec);
	return { vec.x / mag, vec.y / mag, vec.z / mag };
}

inline float magnitude(p_vec3 vec)
{
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}

inline p_vec3 add(p_vec3 a, p_vec3 b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline p_vec3 minus(p_vec3 a, p_vec3 b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

inline p_vec3 multi(p_vec3 a, float b)
{
	return { a.x * b, a.y * b, a.z * b };
}