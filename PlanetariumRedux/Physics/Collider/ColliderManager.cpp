#include "ColliderManager.h"
#include "../../EngineObjects/EngineObject.h"
#include "../../Components/Rigidbody/Rigidbody.h"
#include "../ColliderMath.h"

ColliderManager* ColliderManager::collider_pointer = 0;


Collider::Collider(Vector3d pos, EngineObject* origin, ColliderManager * manager) : originator(origin), collider_manager(manager)
{
	mu = 0.2;
	//manager->collider_list.push_back(this);
	//originator_rigid = origin->getComponentOfType<Rigidbody>();
}

bool Collider::inCollision()
{
	return false;
}

EngineObject* Collider::getOriginator()
{
	return originator;
}

void Collider::setUpDebugMesh()
{

}

void Collider::renderDebugMesh()
{

}

void Collider::Update()
{

}

ColliderManager::ColliderManager()
{
	collider_pointer = this;
}

ColliderManager* ColliderManager::getManager()
{
	return collider_pointer;
}

void ColliderManager::EllipsoidMesh(std::vector<SunsetPhysics::EllipsoidCollider*> ellipsoid_list, std::vector<SunsetPhysics::MeshColliderStruct*> mesh_list)
{
	for (SunsetPhysics::EllipsoidCollider * ellipsoid: ellipsoid_list)
	{
		for (SunsetPhysics::MeshColliderStruct* mesh : mesh_list)
		{
			SunsetPhysics::EllipsoidMesh(*ellipsoid, *mesh);
		}
	}
}

void ColliderManager::UpdateCollisions()
{
	
}

ColliderTransform::ColliderTransform(Transform *transform)
{
	position = transform->position;
	rotation = transform->rotation;
}
