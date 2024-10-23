#include "MathHeader.h"


Vector3f Vector3f::Up = Vector3f(0,1,0);
Vector3f Vector3f::Forward = Vector3f(0, 0, 1);
Vector3f Vector3f::Back = Vector3f(0, 0, -1);

Vector3f Vector3f::Right = Vector3f(1, 0, 0);
Vector3f Vector3f::Down = Vector3f(0, -1, 0);
Vector3f Vector3f::Left = Vector3f(-1, 0, 0);

//const static Quaternion Identity = Quaternion(0, 0, 0, 1);


float radians(float d)
{
	return d * (C_PI / 180.0f);
}

float r_2_degrees(float r)
{
	return r * (180.0f/glm::pi<float>());
}

glm::mat3 Quaternion::mat3quat(Quaternion q)
{
	glm::mat3 Result(1);
	float qxx = (q.x * q.x);
	float qyy = (q.y * q.y);
	float qzz = (q.z * q.z);
	float qxz = (q.x * q.z);
	float qxy = (q.x * q.y);
	float qyz = (q.y * q.z);
	float qwx = (q.w * q.x);
	float qwy = (q.w * q.y);
	float qwz = (q.w * q.z);

	Result[0][0] = float(1) - float(2) * (qyy + qzz);
	Result[0][1] = float(2) * (qxy + qwz);
	Result[0][2] = float(2) * (qxz - qwy);

	Result[1][0] = float(2) * (qxy - qwz);
	Result[1][1] = float(1) - float(2) * (qxx + qzz);
	Result[1][2] = float(2) * (qyz + qwx);

	Result[2][0] = float(2) * (qxz + qwy);
	Result[2][1] = float(2) * (qyz - qwx);
	Result[2][2] = float(1) - float(2) * (qxx + qyy);
	return Result;
}

glm::mat4 Quaternion::mat4quat(Quaternion q)
{
	return glm::mat4((mat3quat(q)));
}

