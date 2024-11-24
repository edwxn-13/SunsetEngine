#include "Component.h"
#include "../EngineObjects/EngineObject.h"
#include <glm/ext/matrix_transform.hpp>
#include "../Camera/camera.h"
Transform::Transform(EngineObject* engineObject) : Component(engineObject)
{
	position_d = Vector3d(0);
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
	return -Vector3f(position_matrix[0]).normal();
}

Vector3f Transform::up()
{
	return -Vector3f(glm::cross(right().glm(), forward().glm())).normal();
}

Transform::Transform(EngineObject* engineObject, Vector3f pos, Vector3f rot, Vector3f s) : Component(engineObject)
{
	position = pos;
	eulerRotation = rot;
	scale = s;
	position_matrix = glm::mat4(1.f);
	scene_cam = nullptr;
}

void Transform::CopyTransform(Transform* b)
{
	position = b->position;
	position_d = b->position_d;

	rotation = b->rotation;
	scale = b->scale;
	position_matrix = b->get_pos_mat();
}

void Transform::model_transform()
{	
	/*
	if (!scene_cam) 
	{
		scene_cam = SCamera::getSceneCamera();
	}

	position_d = position_d + position;

	if (scene_cam) 
	{
		position = position - scene_cam->getPosition();
	}

	else 
	{
		position = position - 0;
	}*/

	position_matrix = glm::mat4(1.f);

	rotation.normalize();
	eulerRotation = rotation.ToEulerAngles();

	glm::mat4 rotation_matrix = Quaternion::RotationMatrix(localTransform->rotation);

	//localTransform->position.z = localTransform->position.z;
	glm::vec3 transformed_position = localTransform->position.glm();
	//transformed_position.z = -transformed_position.z;

	position_matrix = glm::translate(position_matrix, transformed_position);
	position_matrix = position_matrix * (rotation_matrix);
	position_matrix = glm::scale(position_matrix, localTransform->scale.glm());

	if (engineObject->relationships.getParent()) 
	{
		glm::mat4 parent_position_matrix = engineObject->relationships.getParent()->transform.position_matrix;
		glm::mat4 temp_pos_mat = parent_position_matrix * position_matrix;
		transform->position_matrix = temp_pos_mat;
		
		transform->rotation = localTransform->rotation * engineObject->relationships.getParent()->transform.rotation;
		
		CalcGlobalTransform(parent_position_matrix);
	}
	 
	else
	{
		localTransform->CopyTransform(transform);
	}	
}

glm::mat4 Transform::get_pos_mat()
{
	return position_matrix;
}

void Transform::set_pos_mat(glm::mat4 mat)
{
	position_matrix = mat;
}

void Transform::CalcGlobalTransform(glm::mat4 parent_transform)
{
	glm::vec4 tempPosition = glm::vec4(localTransform->position.glm(), 1);
	glm::vec4 globalPos = glm::inverse(parent_transform) * tempPosition;
	transform->position = Vector3f(globalPos.x, globalPos.y, globalPos.z);
}

void Transform::Update(float deltaTime)
{
	model_transform();
}

void Transform::Start()
{

}

void Transform::FixedUpdate(float deltaTime)
{

}

void Transform::Rotate(Vector3f angles)
{
	Quaternion w = Quaternion::ToQuaternion(angles);
	//Quaternion w = Quaternion(glm::quat(angles.glm()));
	rotation = w * rotation;
}

void Transform::Rotate(float x, float y, float z)
{
	Quaternion w = Quaternion::ToQuaternion(Vector3f(x,y,z));
	//Quaternion w = Quaternion(glm::quat(angles.glm()));
	rotation = w * rotation;
}

void Transform::RotateAngleAxis(float angle, Vector3f axis) 
{
	Quaternion w = Quaternion::AngleAxis(angle, axis.normal());
	rotation = w * rotation;
}

void Transform::Rotate(Quaternion q)
{
	rotation = q * rotation;
	//rotation = rotation * q.conjugate();
}

void Transform::Translate(Vector3f trans)
{
	position = Vector3f(trans.x,trans.y,trans.z);
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

Component::~Component()
{

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

void Component::Update(float deltaTime)
{
}

void Component::FixedUpdate(float deltaTime)
{
}

LocalTransform::LocalTransform(EngineObject* engineObject) : Transform(engineObject)
{

}

void LocalTransform::Update(float deltaTime)
{
	position_d = Vector3d(0);
	position = Vector3f(0);
	eulerRotation = Vector3f(0);
	scale = Vector3f(1);
	position_matrix = glm::mat4(1.f);
}
