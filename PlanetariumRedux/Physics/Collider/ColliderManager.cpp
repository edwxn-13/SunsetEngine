#include "ColliderManager.h"

Collider::Collider(Vector3d pos, EngineObject* origin, ColliderManager * manager) : originator(origin), collider_manager(manager)
{
	
}

bool Collider::inCollision()
{
	return false;
}

EngineObject* Collider::getOriginator()
{
	return originator;
}

Rigidbody* Collider::getRigid()
{
	return originator_rigid;
}

void Collider::setUpDebugMesh()
{

}

void Collider::renderDebugMesh()
{

}

ColliderManager * ColliderManager::collider_pointer = 0;
ColliderManager::ColliderManager()
{
	collider_pointer = this;
}

ColliderManager* ColliderManager::getManager()
{
	return collider_pointer;
}
