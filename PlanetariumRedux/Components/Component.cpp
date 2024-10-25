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

void Transform::CopyTransform(Transform* b)
{
	position = b->position;
	rotation = b->rotation;
	scale = b->scale;
	position_matrix = b->get_pos_mat();
}

void Transform::model_transform()
{
	//Transform tempTransform(engineObject);
	//localTransform->CopyTransform(transform);

	position_matrix = glm::mat4(1.f);

	if (engineObject->relationships.getParent()) 
	{
		position_matrix = engineObject->relationships.getParent()->transform.position_matrix;
		

		Transform * parent_trans = &engineObject->relationships.getParent()->transform;


		//ptt - point to transform
		Vector3f ptt = (localTransform->position);
		Quaternion imaginary_point = Quaternion(0, ptt.x, ptt.y, ptt.z);
		Quaternion rotated_point = ((parent_trans->rotation * imaginary_point) * parent_trans->rotation.conjugate());
		Vector3f transformed_local_pos = Vector3f(rotated_point.x, rotated_point.y, rotated_point.z);
		transform->position = transformed_local_pos + parent_trans->position;
		//position = position + engineObject->relationships.getParent()->transform.position;
		//rotation = engineObject->relationships.getParent()->transform.rotation * rotation;
	}

	else
	{
		localTransform->CopyTransform(transform);
	}


	rotation.normalize();
	glm::mat4 rotation_matrix = Quaternion::RotationMatrix(localTransform->rotation);

	eulerRotation = rotation.ToEulerAngles();
	position_matrix = glm::translate(position_matrix, localTransform->position.glm());
	position_matrix = rotation_matrix * position_matrix;

	position_matrix = glm::scale(position_matrix, localTransform->scale.glm());
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
	this->localTransform = &engineObject->localTransform;
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

