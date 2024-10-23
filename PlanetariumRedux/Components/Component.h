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
	std::vector<Component*> findObjectsOfType(int component_id);
	
	virtual void Update();
	virtual void Start();
	virtual void FixedUpdate();
	
	static Component* getGlobalComponantIndex(int i);
	static int getComponentListSize();
	int get_id() { return component_id; }
	bool isActive();

	Transform* transform;
protected:
	EngineObject* engineObject;
	bool active;
	int component_id;
	static std::vector<Component*> world_list;

private:

	


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
	void model_transform();

	void Update() override;
	virtual void Start() override;
	virtual void FixedUpdate() override;

	void Rotate(Vector3f angles);
	void Rotate(Quaternion q);

	void eulerRotate(Vector3f angles);

	void setEulerAngles(Vector3f angles);
	Vector3f getEulerAngles();


	glm::mat4 get_pos_mat();
	void set_pos_mat(glm::mat4);
private:

	Vector3f eulerRotation;
	glm::mat4 position_matrix;

};