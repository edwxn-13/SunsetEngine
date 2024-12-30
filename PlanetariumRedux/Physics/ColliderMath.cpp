#include "ColliderMath.h"
#include "../EngineObjects/EngineObject.h"

SunsetPhysics::EllipsoidCollider::EllipsoidCollider(Vector3f radius, Vector3d pos, EngineObject* origin) : Collider(pos, origin)
{
	this->radius = radius;
}

glm::mat4 SunsetPhysics::EllipsoidCollider::build_matrix()
{
	glm::mat4 e_space = glm::mat4(1.f);

	e_space = glm::scale(e_space, Vector3f(1/radius.x, 1/radius.y, 1/radius.z).glm());
	glm::mat4 rotation_mat = glm::inverse(Quaternion::RotationMatrix(transform.rotation));
	e_space = e_space * rotation_mat;
	//e_space = glm::translate(e_space, -transform.position.glm());
	//e_space = glm::inverse(e_space);
	return e_space;
}

void SunsetPhysics::EllipsoidCollider::Update()
{
	eSpace = build_matrix();
}

bool SunsetPhysics::EllipsoidCollider::inCollision()
{
	return collision.collision_found;
}


bool SunsetPhysics::checkPointInTriangle(const Vector3d& point, const Vector3d& pa, const Vector3d& pb, const Vector3d& pc)
{
	Vector3d e10 = pb - pa;
	Vector3d e20 = pc - pa;
	float a = SMath::Dot(e10, e10);
	float b = SMath::Dot(e10,e20);
	float c = SMath::Dot(e20, e20);
	float ac_bb = (a * c) - (b * b);
	Vector3d vp(point.x - pa.x, point.y - pa.y, point.z - pa.z);
	float d = SMath::Dot(vp,e10);
	float e = SMath::Dot(vp,e20);
	float x = (d * c) - (e * b);
	float y = (e * a) - (d * b);
	float z = x + y - ac_bb;
	return ((in(z) & ~(in(x) | in(y))) & 0x80000000);
	return false;
}

bool SunsetPhysics::getLowestRoot(float a, float b, float c , float maxR, float* root)
{
	float determinant = b * b - 4.0f * a * c;
	// If determinant is negative it means no solutions.
	if (determinant < 0.0f) return false;
	// calculate the two roots: (if determinant == 0 then
	// x1==x2 but let’s disregard that slight optimization)
	float sqrtD = sqrt(determinant);
	float r1 = (-b - sqrtD) / (2 * a);
	float r2 = (-b + sqrtD) / (2 * a);
	// Sort so x1 <= x2
	if (r1 > r2) {
		float temp = r2;
		r2 = r1;
		r1 = temp;
	}
	// Get lowest root:
	if (r1 > 0 && r1 < maxR) {
		*root = r1;
		return true;
	}
	// It is possible that we want x2 - this can happen
	// if x1 < 0
	if (r2 > 0 && r2 < maxR) {
		*root = r2;
		return true;
	}
	// No (valid) solutions
	return false;
}

double SunsetPhysics::squaredLegnth(Vector3d a)
{
	return (pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
}

void SunsetPhysics::EllipsoidMesh(EllipsoidCollider& ellipsoid, MeshColliderStruct& mesh)
{
	for (ColliderTriangle triangle: mesh.collider) 
	{
		EllipsoidTriangle(triangle, ellipsoid);
	}
}

bool SunsetPhysics::RayTriangle(const Vector3d& origin, const Vector3d& raydir, Vector3d& hit_point, const TrianglePoints& points)
{
	Plane plane = Plane(points[0], points[1], points[2]);
	double s_distance = plane.signedDistance(origin);

	double t = -s_distance / (SMath::Dot(plane.normal, raydir));

	if (t > 0 && t > 1) 
	{
		Vector3d point = origin + (raydir * t);
		return checkPointInTriangle(point, points[0], points[1], points[2]);
	}

	return false;
}

bool SunsetPhysics::TriangleOrigin(const ColliderTriangle& triangle)
{
	if (triangle.position.y <= 0)
	{
		return true;
	}
	return false;
}
void SunsetPhysics::EllipsoidTriangle(ColliderTriangle& triangle, EllipsoidCollider& ellipsoid)
{
	Vector3f ta = ellipsoid.eSpace * triangle.triangle_points[0].glm4();
	Vector3f tb = ellipsoid.eSpace * triangle.triangle_points[1].glm4();
	Vector3f tc = ellipsoid.eSpace * triangle.triangle_points[2].glm4();
	Vector3f velocity = ellipsoid.eSpace * ellipsoid.collision.velocity.glm4();
	Vector3d position = (ellipsoid.eSpace * ellipsoid.transform.position.glm4());

	Plane triangle_plane = Plane(ta,tb,tc);

	ellipsoid.collision.collision_found = false;

	if (triangle_plane.isFrontFacingTo(SMath::Normalize(velocity)))
	{
		double t0, t1;

		bool embeddedInPlane = false;

		double signed_distance = triangle_plane.signedDistance(position);

		float normalDotVelocity = SMath::Dot(triangle_plane.normal, velocity);

		if (normalDotVelocity == 0.0f) {
			if (std::abs(signed_distance) >= 1.f) { return; }

			else
			{
				embeddedInPlane = true;
				t0 = 0;
				t1 = 1.0;
			}
		}

		else 
		{
			t0 = (-1.0 - signed_distance) / normalDotVelocity;
			t1 = (1.0 - signed_distance) / normalDotVelocity;

			if (t0 > t1) {
				double temp = t1;
				t1 = t0;
				t0 = temp;
			}

			t1 = glm::fclamp(t1, 0.0, 1.0);
			t0 = glm::fclamp(t0, 0.0, 1.0);
		}

		Vector3d collision_point;
		bool collision_found = false;
		float t = 1.0f;
		if (!embeddedInPlane) 
		{
			Vector3d plane_intersect_point = (position - triangle_plane.normal) + velocity * t0;
			if (checkPointInTriangle(collision_point, ta, tb, tc)) 
			{
				collision_found = true;
				collision_point = plane_intersect_point;
			}
		}

		if (!collision_found) 
		{
			Vector3d p_velocity = velocity;

			Vector3d base = position;

			float velocity_squared_length = squaredLegnth(p_velocity);
			float a, b, c;
			float newT;
			a = velocity_squared_length;
			b = 2.0 * (SMath::Dot(p_velocity, (base - ta)));
			c = squaredLegnth(ta - base.glm());

			if (getLowestRoot(a, b, c, t, &newT) )
			{
				t = newT;
				collision_found = true;
				collision_point = ta;
			}

			b = 2.0 * (SMath::Dot(p_velocity, (base - tb)));
			c = squaredLegnth(tb - base.glm());

			if (getLowestRoot(a, b, c, t, &newT))
			{
				t = newT;
				collision_found = true;
				collision_point = tb;
			}

			b = 2.0 * (SMath::Dot(p_velocity, (base - tc)));
			c = squaredLegnth(tc - base.glm());

			if (getLowestRoot(a, b, c, t, &newT))
			{
				t = newT;
				collision_found = true;
				collision_point = tc;
			}


			Vector3d edge = tb - ta;
			Vector3d baseToVertex = ta - base.glm();

			float edgeSquaredLength = squaredLegnth(edge);
			float edgeDotVelocity = SMath::Dot(edge, p_velocity);
			float edgeDotBaseToVertex = SMath::Dot(edge, baseToVertex);

			a = edgeSquaredLength * -velocity_squared_length +
				edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2 * SMath::Dot(velocity,baseToVertex)) -
				2.0 * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1 - squaredLegnth(baseToVertex)) +
				edgeDotBaseToVertex * edgeDotBaseToVertex;

			if (getLowestRoot(a, b, c, t, &newT)) {
				// Check if intersection is within line segment:
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) /
					edgeSquaredLength;
				if (f >= 0.0 && f <= 1.0) {
					// intersection took place within segment.
					t = newT;
					collision_found = true;
					collision_point = ta + (edge * f).glm();
				}
			}


			edge = tc - tb;
			baseToVertex = tb - base.glm();

			edgeSquaredLength = squaredLegnth(edge);
			edgeDotVelocity = SMath::Dot(edge, p_velocity);
			edgeDotBaseToVertex = SMath::Dot(edge, baseToVertex);

			a = edgeSquaredLength * -velocity_squared_length +
				edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2 * SMath::Dot(velocity, baseToVertex)) -
				2.0 * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1 - squaredLegnth(baseToVertex)) +
				edgeDotBaseToVertex * edgeDotBaseToVertex;

			if (getLowestRoot(a, b, c, t, &newT)) {
				// Check if intersection is within line segment:
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) /
					edgeSquaredLength;
				if (f >= 0.0 && f <= 1.0) {
					// intersection took place within segment.
					t = newT;
					collision_found = true;
					collision_point = ta + (edge * f).glm();
				}
			}


			edge = ta - tc;
			baseToVertex = tc - base.glm();

			edgeSquaredLength = squaredLegnth(edge);
			edgeDotVelocity = SMath::Dot(edge, p_velocity);
			edgeDotBaseToVertex = SMath::Dot(edge, baseToVertex);

			a = edgeSquaredLength * -velocity_squared_length +
				edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2 * SMath::Dot(velocity, baseToVertex)) -
				2.0 * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1 - squaredLegnth(baseToVertex)) +
				edgeDotBaseToVertex * edgeDotBaseToVertex;

			if (getLowestRoot(a, b, c, t, &newT)) {
				// Check if intersection is within line segment:
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) /
					edgeSquaredLength;
				if (f >= 0.0 && f <= 1.0) {
					// intersection took place within segment.
					t = newT;
					collision_found = true;
					collision_point = ta + (edge * f).glm();
				}
			}
		}

		if (collision_found == true) {
			float distToCollision = t * velocity.magnitude();

			if(ellipsoid.collision.collision_found == false ||
				distToCollision < ellipsoid.collision.collision_distance)
			{
				ellipsoid.collision.collision_distance = distToCollision;
				ellipsoid.collision.point = collision_point;
				ellipsoid.collision.collision_found = true;
			}
		
		}
		
	}
};

bool SunsetPhysics::EllipsoidOrigin(EllipsoidCollider& ellipsoid_collider)
{
	Vector3f eSpaceOrigin = ellipsoid_collider.eSpace * Vector3f(0).glm4();
	Vector3f position = ellipsoid_collider.eSpace * ellipsoid_collider.transform.position.glm4();

	if (eSpaceOrigin.y > -1)
	{
		printf("colliding - %f \n", position.y);

		//ellipsoid_collider.getOriginator()->transform.position.y = 0;
		//ellipsoid_collider.getOriginator()->transform.position.y += ellipsoid_collider.getOriginator()->transform.position.y/2;

		return true;
	}

	return false;
}

SunsetPhysics::Plane::Plane(Vector3d origin, Vector3d normal) : normal(normal), origin(origin)
{
	plane_constant = -(normal.x * origin.x + normal.y * origin.y + normal.z * origin.z);
}

SunsetPhysics::Plane::Plane(Vector3d a, Vector3d b, Vector3d c)
{
	Vector3d ab = b - a;
	Vector3d ac = c - a;	

	normal = SMath::Normalize(SMath::Cross(ab, ac));
	origin = a;

	plane_constant = -(normal.x * origin.x + normal.y * origin.y + normal.z * origin.z);
}

SunsetPhysics::Plane::Plane(ColliderTriangle triangle)
{
	Vector3d a = triangle.triangle_points[0];
	Vector3d b = triangle.triangle_points[1];
	Vector3d c = triangle.triangle_points[2];

	Vector3d ab = b - a;
	Vector3d ac = c - a;

	normal = SMath::Normalize(SMath::Cross(ab, ac));
	origin = a;

	plane_constant = -(normal.x * origin.x + normal.y * origin.y + normal.z * origin.z);
}

bool SunsetPhysics::Plane::isFrontFacingTo(const Vector3d& direction)
{
	double dot = SMath::Dot(normal,direction);
	return (dot <= 0);
}

double SunsetPhysics::Plane::signedDistance(Vector3d point)
{
	return SMath::Dot(point, normal) + plane_constant;
}

