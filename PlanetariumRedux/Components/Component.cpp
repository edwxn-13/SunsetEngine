#include "Component.h"
#include "../EngineObjects/EngineObject.h"
#include <glm/ext/matrix_transform.hpp>

Transform::Transform(EngineObject* engineObject) : Component(engineObject)
{
	position = Vector3f(0);
	eulerRotation = Vector3f(0);
	scale = Vector3f(1);
	position_matrix = glm::mat4(1.f);
}

Vector3f Transform::forward()
{
	return  Vector3f(position_matrix[2]).normal();
}

Vector3f Transform::right()
{
	return Vector3f(glm::cross(forward().glm(), Vector3f::Up.glm())).normal();
}

Vector3f Transform::up()
{
	return Vector3f(glm::cross(right().glm(), forward().glm())).normal();
}

Transform::Transform(EngineObject* engineObject, Vector3f pos, Vector3f rot, Vector3f s) : Component(engineObject)
{
	position = pos;
	eulerRotation = rot;
	scale = s;
	position_matrix = glm::mat4(1.f);
}

void Transform::model_transform()
{
	//Quaternion rotation_product = Quaternion(0, position.x, position.y, position.z);
	//rotation_product = (rotation * rotation_product) * rotation.conjugate();
	//position = Vector3f(rotation_product.x, rotation_product.y, rotation_product.z);
	if (engineObject->relationships.getParent()) 
	{
		position_matrix = engineObject->relationships.getParent()->transform.get_pos_mat();
		//position = position + engineObject->relationships.getParent()->transform.position;
		//rotation = engineObject->relationships.getParent()->transform.rotation * rotation;
	}
	rotation.normalize();
	glm::mat4 rotation_matrix = Quaternion::RotationMatrix(rotation);

	eulerRotation = rotation.ToEulerAngles();
	position_matrix = glm::mat4(1.f);
	position_matrix = glm::translate(position_matrix, position.glm());
	position_matrix = rotation_matrix * position_matrix;

	position_matrix = glm::scale(position_matrix, scale.glm());


	//glm::rotate(position_matrix, position);
	//position_matrix = glm::rotate(position_matrix, 2 * acos(rotation.w), glm::vec3( rotation.x, rotation.y, rotation.z));
	//position_matrix = glm::quat(1, 1, 1, 1) * position_matrix;
	//position_matrix = glm::rotate(position_matrix, eulerRotation.y, glm::vec3(0, 0, 1));
	//position_matrix = glm::rotate(position_matrix, eulerRotation.z, glm::vec3(0, 1, 0));
}

glm::mat4 Transform::get_pos_mat()
{
	return position_matrix;
}

void Transform::set_pos_mat(glm::mat4 mat)
{
	position_matrix = mat;
}

void Transform::Update()
{
	model_transform();
}

void Transform::Start()
{

}

void Transform::FixedUpdate()
{

}

void Transform::Rotate(Vector3f angles)
{
	Quaternion q = Quaternion::ToQuaternion(angles);
	rotation = q * rotation;
	//rotation = rotation * q.conjugate();
}

void Transform::Rotate(Quaternion q)
{
	rotation = q * rotation;
	//rotation = rotation * q.conjugate();
}

void Transform::eulerRotate(Vector3f angles)
{
	Vector3f result = rotation.ToEulerAngles();
	result = result + angles;
	setEulerAngles(result);
}

void Transform::setEulerAngles(Vector3f angles)
{
	rotation = Quaternion::ToQuaternion(angles);
}

Vector3f Transform::getEulerAngles()
{
	Vector3f rot = rotation.ToEulerAngles();
	return rot;
}


std::vector<Component *> Component::world_list = {};

Component::Component(EngineObject * engineObject)
{
	this->engineObject = engineObject;
	this->transform = &engineObject->transform;
	world_list.push_back(this);
	active = true;
}

Component* Component::getGlobalComponantIndex(int i)
{
	return world_list[i];
}

int Component::getComponentListSize()
{
	return world_list.size();
}

bool Component::isActive()
{
	return active;
}


void Component::Start()
{
}

void Component::Update()
{
}

void Component::FixedUpdate()
{
}

