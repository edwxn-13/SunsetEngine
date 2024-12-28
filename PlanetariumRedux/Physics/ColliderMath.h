#pragma once

#include "Collider/ColliderManager.h"
#include "CollisionUtils/Collision.h"

#include <vector>
namespace SunsetPhysics 
{
	typedef unsigned int uint32;
	#define in(a) ((uint32&) a)

	bool getLowestRoot(float a, float b, float c, float maxR, float* root);
	double squaredLegnth(Vector3d a);


	using Triangle = unsigned int [3];

	using TrianglePoints = Vector3d[3];

	using VertexList = std::vector<Vector3f>;

	struct ColliderTriangle 
	{
		TrianglePoints triangle_points;
		Vector3d position;
		glm::mat4 * position_matrix;
	};

	struct MeshColliderStruct
	{
		std::vector<ColliderTriangle> collider;
	};
	
	struct SphereCollider 
	{
		float radius;
		Vector3d position;
	};

	class Ray 
	{
	public:
		Vector3f position;
		Vector3f forward;
		float t;
		Vector3f point_on_line(float t);
	};

	struct Plane 
	{
		Vector3d origin;
		Vector3d normal;
		double plane_constant;

		Plane(Vector3d origin, Vector3d normal);
		Plane(Vector3d a, Vector3d b, Vector3d c);
		Plane(ColliderTriangle triangle);


		bool isFrontFacingTo(const Vector3d& direction);
		double signedDistance(Vector3d point);
	};

	class EllipsoidCollider : public Collider
	{
	public:

		EllipsoidCollider(Vector3f radius, Vector3d pos , EngineObject* origin);
		Vector3f radius;

		glm::mat4 eSpace;

		glm::mat4 build_matrix();
		void Update() override;
	};

	//TrianglePoints getTriangle(glm::mat4 matrix, Vector3f a, Vector3f b, Vector3f c);


	bool EllipsoidMesh(EllipsoidCollider& ellipsoid, MeshColliderStruct& mesh, Vector3f velocity);
	bool RayTriangle(const Vector3d &origin, const Vector3d &raydir, Vector3d &hit_point, const TrianglePoints &points);
	bool checkPointInTriangle(const Vector3d &point, const Vector3d &pa, const Vector3d &pb, const Vector3d &pc);
	bool TriangleOrigin(const ColliderTriangle& triangle);
	bool EllipsoidTriangle(ColliderTriangle&triangle, EllipsoidCollider &ellipsoid , Vector3f velocity);
	bool EllipsoidOrigin(EllipsoidCollider& ellipsoid_collider);
};