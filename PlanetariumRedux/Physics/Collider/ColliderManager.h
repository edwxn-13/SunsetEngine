#pragma once

#include <vector>
#include "../../Maths/MathHeader.h"
#include "../../Maths/Vector3d.h"

#include "../../Engine/Renderer/ShaderManager/ShaderManager.h"
#include "../../Components/Component.h"
class EngineObject;
class Rigidbody;


class Collider;

class ColliderManager
{
	static ColliderManager* collider_pointer;
public:
	ColliderManager();
	static ColliderManager* getManager();
	std::vector<Collider* > collider_list;
};


struct ColliderTransform
{
	Vector3d position = 0;
	Quaternion rotation = Quaternion();
	Vector3f Scale = 0;
};

class Collider
{
public:
	Collider(Vector3d pos, EngineObject* origin, ColliderManager * manager = ColliderManager::getManager());
	virtual bool inCollision();
	EngineObject * getOriginator();
	Rigidbody* getRigid();

	virtual void setUpDebugMesh();
	virtual void renderDebugMesh();

protected:
	ColliderManager* collider_manager;
	EngineObject* originator;
	Rigidbody* originator_rigid;

public:
	ColliderTransform transform;
	float mu;
};