#pragma once
#include "../../Maths/MathHeader.h"

struct p_vec3 { float x, y, z; };
struct p_vert { p_vec3 point, normal, tc, colour; float alti;};
struct p_tri { p_vert v1, v2, v3; };
struct p_index { unsigned int t[3]; int id; int p_id; };

inline p_vec3 normalize(p_vec3 vec);
inline float magnitude(p_vec3 vec);
inline p_vec3 add(p_vec3 a, p_vec3 b);
inline p_vec3 minus(p_vec3 a, p_vec3 b);
inline p_vec3 multi(p_vec3 a, float b);

#include "PlanetMath.inl"