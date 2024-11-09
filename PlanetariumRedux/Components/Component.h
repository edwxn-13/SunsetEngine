#pragma once

#include <vector>
#include "../Maths/MathHeader.h"
class EngineObject;
class Transform;

enum ComponentName
{
	
};

class Component
{
public:
	Component(EngineObject* engineObject);
	~Component();
	template <class T>
	std::vector<Component*> findObjectsOfType(T * component);

	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void FixedUpdate(float deltaTime);


	static Component* getGlobalComponantIndex(int i);

	static int getComponentListSize();
	int get_id() { return component_id; }
	bool isActive();

	Transform* localTransform;
	Transform* transform;
protected:
	EngineObject* engineObject;
	bool active;
	int component_id;
	static std::vector<Component*> world_list;
};


class Transform : public Component
{

public:
	Vector3f position;
	Quaternion rotation;
	Vector3f scale;

	Vector3f forward();
	Vector3f right();
	Vector3f up();

	Transform(EngineObject * engineObject);
	Transform(EngineObject* engineObject, Vector3f pos, Vector3f rot, Vector3f s);

	void CopyTransform(Transform* b);
	void model_transform();

	void Update(float deltaTime) override;
	virtual void Start() override;
	virtual void FixedUpdate(float deltaTime) override;

	void Rotate(Vector3f angles);
	void Rotate(Quaternion q);
	void RotateAngleAxis(float angle, Vector3f axis);
	void eulerRotate(Vector3f angles);

	void setEulerAngles(Vector3f angles);
	Vector3f getEulerAngles();


	glm::mat4 get_pos_mat();
	void set_pos_mat(glm::mat4);
private:

	Vector3f eulerRotation;
	glm::mat4 position_matrix;

};

