#pragma once
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#define C_PI 3.14159265358979323846

float radians(float d);
float r_2_degrees(float r);;
class Vector2f 
{
public:
	Vector2f() { x = y = 0; }
	Vector2f(float f) { x = y = f; }
	Vector2f(float X, float Y) { x = X; y = Y; }

	Vector2f operator+= (const glm::vec2& c) const
	{
		Vector2f result;
		result.x = this->x + c.x;
		result.y = this->y + c.y;
		return result;
	}

	Vector2f operator-= (const glm::vec2& c) const
	{
		Vector2f result;
		result.x = this->x - c.x;
		result.y = this->y - c.y;
		return result;
	}

	Vector2f operator+= (const Vector2f& c) const
	{
		Vector2f result = *this;
		result.x = this->x + c.x;
		result.y = this->y + c.y;
		return result;
	}

	Vector2f operator-= (const Vector2f& c) const
	{
		Vector2f result;
		result.x = this->x - c.x;
		result.y = this->y - c.y;
		return result;
	}

	Vector2f operator+ (const Vector2f& c) const
	{
		Vector2f result;
		result.x = this->x + c.x;
		result.y = this->y + c.y;
		return result;
	}

	Vector2f operator+ (const float& c) const
	{
		Vector2f result;
		result.x = this->x + c;
		result.y = this->y + c;
		return result;
	}

	Vector2f operator- (const Vector2f& c) const
	{
		Vector2f result;
		result.x = this->x - c.x;
		result.y = this->y - c.y;
		return result;
	}

	Vector2f operator- (const float& c) const
	{
		Vector2f result;
		result.x = this->x - c;
		result.y = this->y - c;
		return result;
	}

	Vector2f operator* (const float& c) const
	{
		Vector2f result;
		result.x = this->x * c;
		result.y = this->y * c;
		return result;
	}

	Vector2f operator/ (const float& c) const
	{
		Vector2f result;
		result.x = this->x / c;
		result.y = this->y / c;
		return result;
	}

	float magnitude()
	{
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	void Normalize()
	{
		double magnitude = sqrt(pow(x, 2) + pow(y, 2));
		x = x / magnitude;
		y = y / magnitude;
	}

	Vector2f normal()
	{
		double magnitude = sqrt(pow(x, 2) + pow(y, 2));
		x = x / magnitude;
		y = y / magnitude;
		return Vector2f(x, y);
	}

	float x, y;
};

class Vector3f 
{
public:

	static Vector3f Up, Down, Forward, Back, Right, Left;

	Vector3f() { x = y = z = 0; }

	Vector3f(Vector2f a) { x = a.x; y = a.y; z = 0; }

	Vector3f(Vector2f a, float b) { x = a.x; y = a.y; z = b; }

	Vector3f(float f) { x = y = z = f; }

	Vector3f(float X, float Y, float Z) { x = X; y = Y; z = Z; }

	Vector3f(glm::vec3 v) { x = v.x; y = v.y; z = v.z; }

	glm::vec3 glm() { return glm::vec3(x, y, z); }

	// Operators
	/*Vector3f operator= (const glm::vec3& c) const
	{
		Vector3f result;
		result.x = c.x;
		result.y = c.y;
		result.z = c.z;
		return result;
	}*/

	Vector3f operator+= (const glm::vec3& c) const 
	{
		Vector3f result;
		result.x = this->x + c.x;
		result.y = this->y + c.y;
		result.z = this->z + c.z;
		return result;
	}

	Vector3f operator-= (const glm::vec3& c) const
	{
		Vector3f result;
		result.x = this->x - c.x;
		result.y = this->y - c.y;
		result.z = this->z - c.z;
		return result;
	}

	Vector3f operator+= (const Vector3f& c) const
	{
		Vector3f result = *this;
		result.x = this->x + c.x;
		result.y = this->y + c.y;
		result.z = this->z + c.z;
		return result;
	}

	Vector3f operator-= (const Vector3f& c) const
	{
		Vector3f result;
		result.x = this->x - c.x;
		result.y = this->y - c.y;
		result.z = this->z - c.z;
		return result;
	}

	Vector3f operator+ (const Vector3f& c) const 
	{
		Vector3f result;
		result.x = this->x + c.x;
		result.y = this->y + c.y;
		result.z = this->z + c.z;
		return result;
	}

	Vector3f operator+ (const float& c) const
	{
		Vector3f result;
		result.x = this->x + c;
		result.y = this->y + c;
		result.z = this->z + c;
		return result;
	}

	Vector3f operator- (const Vector3f& c) const
	{
		Vector3f result;
		result.x = this->x - c.x;
		result.y = this->y - c.y;
		result.z = this->z - c.z;
		return result;
	}

	Vector3f operator- () const
	{
		Vector3f result;
		result.x = -x;
		result.y = -y;
		result.z =  -z;
		return result;
	}

	Vector3f operator- (const float& c) const
	{
		Vector3f result;
		result.x = this->x - c;
		result.y = this->y - c;
		result.z = this->z - c;
		return result;
	}

	Vector3f operator* (const float& c) const
	{
		Vector3f result;
		result.x = this->x * c;
		result.y = this->y * c;
		result.z = this->z * c;
		return result;
	}

	Vector3f operator* (const Vector3f& c) const
	{
		Vector3f result;
		result = glm::cross(glm::vec3(this->x, this->y, this->z), glm::vec3(c.x, c.y, c.z));
		return result;
	}

	Vector3f operator/ (const float& c) const
	{
		Vector3f result;
		result.x = this->x / c;
		result.y = this->y / c;
		result.z = this->z / c;
		return result;
	}

	float magnitude() 
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

	Vector3f normal()
	{
		double magnitude = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;
		return Vector3f(x, y, z);
	}
	float x, y, z;
};
	
class Quaternion
{
public:

	Quaternion()
	{
		w = 1; x = y = z = 0;
	}

	Quaternion(float w, float x, float y, float z)
	{
		this->w = w; this->x = x; this->y = y; this->z = z;
	}

	Quaternion(glm::quat quat_t)
	{
		x = quat_t.x;
		y = quat_t.y;
		z = quat_t.z;
		w = quat_t.w;
	}


	void setQuaternion(float w, float x, float y, float z)
	{
		float magnitude = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2) + pow(w, 2));
		if (magnitude == 1)
		{
			this->w = w;
			this->x = x;
			this->y = y;
			this->z = z;
			return;
		}
		this->w = w / magnitude;
		this->x = x / magnitude;
		this->y = y / magnitude;
		this->z = z / magnitude;
	}

	static glm::mat3 mat3quat(Quaternion q);

	static glm::mat4 mat4quat(Quaternion q);

	static glm::mat4 rotation_matrix(Quaternion q) 
	{
		
		glm::quat quaternion;
		quaternion.w = q.w;
		quaternion.x = q.x;
		quaternion.y = q.y;
		quaternion.z = q.z;

		glm::mat4 rotation_matrix = glm::toMat4(quaternion);

		return rotation_matrix;
	}

	static float Magnitude(Quaternion a) 
	{
		float magnitude = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2) + pow(a.w, 2));
		return magnitude;
	}

	static Quaternion Normalize(Quaternion a)
	{
		float magnitude = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2) + pow(a.w, 2));

		if (magnitude == 1)
		{
			return a;
		}
		a.w = a.w / magnitude;
		a.x = a.x / magnitude;
		a.y = a.y / magnitude;
		a.z = a.z / magnitude;

		return a;
	}

	static float Angle(Quaternion a, Quaternion b)
	{
		Quaternion conj = Quaternion(a.w, -a.x, -a.y, -a.z);
		conj.normalize();
		Quaternion product = conj * b;
		product.normalize();
		float angle = 2 * glm::acos(product.w);

		return r_2_degrees(angle);
	}

	void normalize()
	{
		float magnitude = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2) + pow(w, 2));
		if (magnitude == 1)
		{
			return;
		}
		w = w / magnitude;
		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;
	}

	Quaternion conjugate() 
	{
		Quaternion conj = Quaternion(w, -x, -y, -z);
		return conj;
	}

	static Quaternion ToQuaternion(Vector3f rotation) // roll (x), pitch (y), yaw (z), angles are in radians
	{
		// Abbreviations for the various angular functions
		Quaternion q;

		double roll = radians(rotation.x) ;
		double pitch = radians(rotation.y);
		double yaw = radians(rotation.z);

		double cr = cos(roll * 0.5);
		double sr = sin(roll * 0.5);
		double cp = cos(pitch * 0.5);
		double sp = sin(pitch * 0.5);
		double cy = cos(yaw * 0.5);
		double sy = sin(yaw * 0.5);

		q.w = cr * cp * cy + sr * sp * sy;
		q.x = sr * cp * cy - cr * sp * sy;
		q.y = cr * sp * cy + sr * cp * sy;
		q.z = cr * cp * sy - sr * sp * cy;
		
		//q.normalize();

		return q;
	}

	static glm::mat4 RotationMatrix(Quaternion q) {
		float xx = q.x * q.x;
		float yy = q.y * q.y;
		float zz = q.z * q.z;
		return glm::mat4(
			1 - 2 * yy - 2 * zz, 2 * q.x * q.y + 2 * q.z * q.w, 2 * q.x * q.z - 2 * q.y * q.w, 0,
			2 * q.x * q.y - 2 * q.z * q.w, 1 - 2 * xx - 2 * zz, 2 * q.y * q.z + 2 * q.x * q.w, 0,
			2 * q.x * q.z + 2 * q.y * q.w, 2 * q.y * q.z - 2 * q.x * q.w, 1 - 2 * xx - 2 * yy, 0,
			0, 0, 0, 1);
	}

	static Quaternion AngleAxis(float angle, Vector3f axis) {
		float s = sin(angle / 2);
		Quaternion r = Quaternion(axis.x * s,
			axis.y * s,
			axis.z * s,
			cos(angle / 2));

		r.normalize();
		return r;


	}

	static Quaternion Exp(Quaternion q) {
		Vector3f v = Vector3f(q.x, q.y, q.z);
		float v_m = v.magnitude();
		Vector3f v_n = v.normal();
		float sin_v = sin(v_m);
		float exp_w = exp(q.w);
		return Quaternion(
			v_n.x * sin_v * exp_w,
			v_n.y * sin_v * exp_w,
			v_n.z * sin_v * exp_w,
			cos(v_m) * exp_w);
	}

	static Quaternion Inverse(Quaternion q) {
		float m = Quaternion::Magnitude(q);
		if (m == 0) return Quaternion( 0, 0, 0, 0 );
		m *= m;
		return Quaternion(- q.x / m, -q.y / m, -q.z / m, q.w / m);
	}

	static Quaternion Difference(Quaternion a, Quaternion b) {
		return (Quaternion::Inverse(a) * b);
	}

	static Quaternion Scale(Quaternion q, float s) {
		return Quaternion( q.x* s, q.y* s, q.z* s, q.w* s );
	}

	static Quaternion Log(Quaternion q) {
		Vector3f v = Vector3f(q.x, q.y, q.z);
		Vector3f v_n = v.normal();
		float m = Magnitude(q);
		float a = acos(q.w / m);
		return Quaternion(
			v_n.x * a,
			v_n.y * a,
			v_n.z * a,
			log(m));
		
	}

	static Quaternion Pow(Quaternion q, float n) {
		return Quaternion::Exp(Quaternion::Scale(Quaternion::Log(q), n));
	}

	static float Dot(Quaternion q1, Quaternion q2) {
		return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
	}

	static Quaternion Slerp(Quaternion q1, Quaternion q2, float t) {
		t = t < 0 ? 0 : t;
		t = t > 1 ? 1 : t;
		if (Dot(q1, q2) < 0) q2 = Scale(q2, -1);
		return (q1 * Pow((Inverse(q1) * q2), t));
	}

	Vector3f ToEulerAngles() {
		Vector3f angles;
		normalize();
		// roll (x-axis rotation)
		double sinr_cosp = 2 * (w * x + y * z);
		double cosr_cosp = 1 - 2 * (x * x + y * y);
		angles.x = std::atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		double sinp = std::sqrt(1 + 2 * (w * y - x * z));
		double cosp = std::sqrt(1 - 2 * (w * y - x * z));
		angles.y = 2 * std::atan2(sinp, cosp) - glm::pi<float>() / 2;

		// yaw (z-axis rotation)
		double siny_cosp = 2 * (w * z + x * y);
		double cosy_cosp = 1 - 2 * (y * y + z * z);
		angles.z = std::atan2(siny_cosp, cosy_cosp);

		return angles;
	}

	glm::quat glm() 
	{
		glm::quat glm_version;
		glm_version.x = x;
		glm_version.y = y;
		glm_version.z = z;
		glm_version.w = w;

		return glm_version;
	}

	Quaternion operator* (const Quaternion& a) const
	{
		Quaternion result;
		//result.w = (w * a.w) - (x * a.x) + (y * a.y) - (z * a.z);
		//result.x = (w * a.x) + (x * a.w) + (y * a.z) - (z * a.y);
		//result.y = (w * a.y) - (x * a.z) + (y * a.w) + (z * a.x);
		//result.z = (w * a.z) + (x * a.z) + (y * a.x) + (z * a.w);

		glm::quat aaa; 
		aaa.x = a.x;
		aaa.y = a.y;
		aaa.z = a.z;
		aaa.w = a.w;

		glm::quat thisOne;
		thisOne.x = x;
		thisOne.y = y;
		thisOne.z = z;
		thisOne.w = w;

		glm::quat aNSWER = thisOne * aaa;
		result.x = aNSWER.x;
		result.y = aNSWER.y;
		result.z = aNSWER.z;
		result.w = aNSWER.w;

		return result;
	}

	float w, x, y, z;
};