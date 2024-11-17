
//INLINE FILE WOOOOOO


namespace SunsetMath 
{
	inline float Lerp(float a, float target, float rate)
	{
		return (1 - rate) * a + rate * target;
	}

	inline glm::mat4 rotation_matrix(Quaternion q)
	{

		glm::quat quaternion;
		quaternion.w = q.w;
		quaternion.x = q.x;
		quaternion.y = q.y;
		quaternion.z = q.z;

		glm::mat4 rotation_matrix = glm::toMat4(quaternion);

		return rotation_matrix;
	}

	inline float Magnitude(Quaternion a)
	{
		float magnitude = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2) + pow(a.w, 2));
		return magnitude;
	}

	inline Quaternion Normalize(Quaternion a)
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

	inline float Angle(Quaternion a, Quaternion b)
	{
		Quaternion conj = Quaternion(a.w, -a.x, -a.y, -a.z);
		conj.normalize();
		Quaternion product = conj * b;
		product.normalize();
		float angle = 2 * glm::acos(product.w);

		return r_2_degrees(angle);
	}

	inline glm::mat4 RotationMatrix(Quaternion q) {
		float xx = q.x * q.x;
		float yy = q.y * q.y;
		float zz = q.z * q.z;
		return glm::mat4(
			1 - 2 * yy - 2 * zz, 2 * q.x * q.y + 2 * q.z * q.w, 2 * q.x * q.z - 2 * q.y * q.w, 0,
			2 * q.x * q.y - 2 * q.z * q.w, 1 - 2 * xx - 2 * zz, 2 * q.y * q.z + 2 * q.x * q.w, 0,
			2 * q.x * q.z + 2 * q.y * q.w, 2 * q.y * q.z - 2 * q.x * q.w, 1 - 2 * xx - 2 * yy, 0,
			0, 0, 0, 1);
	}

	inline Quaternion AngleAxis(float angle, Vector3f axis) {

		float rad_angle = radians(angle);
		float s = sin(rad_angle / 2);
		Quaternion r = Quaternion(axis.x * s,
			axis.y * s,
			axis.z * s,
			cos(rad_angle / 2));
		return r;
	}

	inline Quaternion Exp(Quaternion q) {
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

	inline Quaternion Inverse(Quaternion q) {
		float m = Quaternion::Magnitude(q);
		if (m == 0) return Quaternion(0, 0, 0, 0);
		m *= m;
		return Quaternion(-q.x / m, -q.y / m, -q.z / m, q.w / m);
	}

	inline Quaternion Difference(Quaternion a, Quaternion b) {
		return (Quaternion::Inverse(a) * b);
	}

	inline Quaternion Scale(Quaternion q, float s) {
		return Quaternion(q.x * s, q.y * s, q.z * s, q.w * s);
	}

	inline Quaternion Log(Quaternion q) {
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

	inline Quaternion Pow(Quaternion q, float n) {
		return Quaternion::Exp(Quaternion::Scale(Quaternion::Log(q), n));
	}

	inline float Dot(Quaternion q1, Quaternion q2) {
		return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
	}

	inline Quaternion Slerp(Quaternion q1, Quaternion q2, float t) {
		t = t < 0 ? 0 : t;
		t = t > 1 ? 1 : t;
		if (Dot(q1, q2) < 0) q2 = Scale(q2, -1);
		return (q1 * Pow((Inverse(q1) * q2), t));
	}
};
