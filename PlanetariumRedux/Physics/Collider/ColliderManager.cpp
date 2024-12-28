#include "ColliderManager.h"
#include "../../EngineObjects/EngineObject.h"
#include "../../Components/Rigidbody/Rigidbody.h"
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

void ColliderManager::UpdateCollisions()
{
	
}

ColliderTransform::ColliderTransform(Transform *transform)
{
	position = transform->position;
	rotation = transform->rotation;
}
