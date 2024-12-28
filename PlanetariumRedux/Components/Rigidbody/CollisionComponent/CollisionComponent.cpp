#include "CollisionComponent.h"
#include "../Rigidbody.h"
#include "../../../EngineObjects/EngineObject.h"
#include "../../../Utils/ModelParser/ModelParser.h"

ColliderComponent::ColliderComponent(EngineObject* engineObject) : Component(engineObject)
{
	if (!rigidbody) { static_object = true; }

}

void ColliderComponent::Start()
{
	//rigidbody = engineObject->getComponentOfType<Rigidbody>();
	if (rigidbody) { static_object = true; }
}

void ColliderComponent::FixedUpdate(float deltaTime)
{
}

void ColliderComponent::Update(float deltaTime)
{
}


void EllipsoidCollider::Start()
{

}

void EllipsoidCollider::FixedUpdate(float deltaTime)
{

}

void EllipsoidCollider::Update(float deltaTime)
{
	if (!static_object) { return; }
	collider.Update();
	collider.transform = transform;
	SunsetPhysics::EllipsoidOrigin(collider);
}

bool EllipsoidCollider::TriangleCheck()
{

	return false;
}


EllipsoidCollider::EllipsoidCollider(EngineObject* engineObject) : ColliderComponent(engineObject), collider(Vector3f(4), transform->position, engineObject)
{

}

ModelCollider::ModelCollider(EngineObject* engineObject, Model* model) : ColliderComponent(engineObject)
{
	std::vector<Mesh> mesh_list = model->getMeshInfo();

	for (Mesh mesh : mesh_list)
	{
		SunsetPhysics::MeshColliderStruct new_mcs_struct;

		for (int i = 0; i < mesh.indices.size(); i = i + 3)
		{
			SunsetPhysics::ColliderTriangle temp_t;

			for (int j = 0; j < 3; j++) 
			{
				temp_t.triangle_points[j] = mesh.vertices[i + j].pos;
			}

			temp_t.position = SMath::Magnitude(temp_t.triangle_points[0] + temp_t.triangle_points[1] + temp_t.triangle_points[2] / 3);

			new_mcs_struct.collider.push_back(temp_t);
		}

		mcs_list.push_back(new_mcs_struct);
	}
}

void ModelCollider::Start()
{
}

void ModelCollider::FixedUpdate(float deltaTime)
{
}

void ModelCollider::Update(float deltaTime)
{
}
