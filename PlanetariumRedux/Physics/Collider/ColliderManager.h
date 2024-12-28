#pragma once

#include "../../Maths/MathHeader.h"
#include "../../Maths/Vector3d.h"
#include "../../Engine/Renderer/ShaderManager/ShaderManager.h"
#include "../../Components/Component.h"
#include "../CollisionUtils/Collision.h"
#include <vector>

class EngineObject;
class Collider;

class ColliderManager
{
	static ColliderManager* collider_pointer;
	int colliderDebugVAO;

public:
	ColliderManager();
	static ColliderManager* getManager();
	std::vector<Collider* > collider_list;
	void UpdateCollisions();

	friend class Collider;
};

struct ColliderTransform
{
	ColliderTransform() {}
	ColliderTransform(Transform * transform);
	Vector3d position = 0;
	Quaternion rotation = Quaternion();
	Vector3f Scale = 1.0f;
};

class Collider
{
public:
	Collider(Vector3d pos, EngineObject* origin, ColliderManager * manager = ColliderManager::getManager());
	virtual bool inCollision();
	EngineObject * getOriginator();

	virtual void setUpDebugMesh();
	virtual void renderDebugMesh();

	virtual void Update();

	void (*on_collision)(Collision);

protected:
	ColliderManager* collider_manager;
	EngineObject* originator;

public:
	ColliderTransform transform;
	float mu;
};

