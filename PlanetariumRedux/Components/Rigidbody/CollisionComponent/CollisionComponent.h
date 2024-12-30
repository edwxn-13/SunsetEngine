#pragma once
#include "../../Component.h"
#include "../../../Physics/ColliderMath.h"

class Rigidbody;
struct Model;

class ColliderComponent : public Component
{
protected:
	Rigidbody* rigidbody = nullptr;
	bool static_object;
	int col_count = 0;

public:
	ColliderComponent(EngineObject* engineObject);
	void Start() override;
	void FixedUpdate(float deltaTime) override;
	void Update(float deltaTime) override;
	
};

class SphereCollider : public ColliderComponent
{
public:
	SphereCollider(EngineObject* engineObject);
	void Start() override;
	void FixedUpdate(float deltaTime) override;
	void Update(float deltaTime) override;

	SunsetPhysics::SphereCollider collider;
};


class ModelCollider : public ColliderComponent 
{
public:
	ModelCollider(EngineObject* engineObject, Model* model);
	void Start() override;
	void FixedUpdate(float deltaTime) override;
	void Update(float deltaTime) override;
	
	std::vector<SunsetPhysics::MeshColliderStruct> mcs_list;
};

class EllipsoidCollider : public ColliderComponent
{
public:

	EllipsoidCollider(EngineObject* engineObject);
 
	void Start() override;
	void FixedUpdate(float deltaTime) override;
	void Update(float deltaTime) override;

	

	bool TriangleCheck();

	SunsetPhysics::EllipsoidCollider collider;
};
