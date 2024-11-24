#include "Component.h"
#include "../EngineObjects/EngineObject.h"
#include <glm/ext/matrix_transform.hpp>
#include "../Camera/camera.h"
Transform::Transform(EngineObject* engineObject) : Component(engineObject)
{
	position_d = Vector3d(0);
	position = Vector3d(0);
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

Transform::Transform(EngineObject* engineObject, Vector3d pos, Vector3f rot, Vector3f s) : Component(engineObject)
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
	transform->position = Vector3d(globalPos.x, globalPos.y, globalPos.z);
}

void Transform::FloatingOrigin()
{
	if (!scene_cam)
	{
		scene_cam = SCamera::getSceneCamera();
	}
	else{
		position_d = position_d + position;
		Vector3d transformed_pos = position_d - scene_cam->getRootPosition();
		position = Vector3d(transformed_pos.x, transformed_pos.y, transformed_pos.z);
	}
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

void Transform::Translate(Vector3d trans)
{
	position = Vector3d(trans.x,trans.y,trans.z);
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
	position_d = Vector3d(0);
	position = Vector3d(0);
	eulerRotation = Vector3f(0);
	scale = Vector3f(1);
	position_matrix = glm::mat4(1.f);
}

void Transform::Update(float deltaTime)
{
	if (!engineObject->relationships.getParent())
	{

		glm::vec3 zeroed_pos; 

		if (!scene_cam)
		{
			scene_cam = SCamera::getSceneCamera();
			zeroed_pos = position.glm();
		}

		else 
		{
			Vector3d temp = position - scene_cam->getRootPosition();
			zeroed_pos = temp.glm();
		}

		position_matrix = glm::mat4(1.0f);
		position_matrix = glm::translate(position_matrix, zeroed_pos);

		glm::mat4 rotation_matrix = Quaternion::RotationMatrix(rotation);

		position_matrix = position_matrix * rotation_matrix;
		position_matrix = glm::scale(position_matrix, scale.glm());
	}

	if (EngineObject* parent = engineObject->relationships.getParent())
	{
		glm::mat4 pt_mat = parent->transform.get_pos_mat();
		position_matrix = pt_mat * localTransform->get_pos_mat();

		glm::vec4 global_pos = glm::vec4(localTransform->position.glm(), 0.0f);
		global_pos = glm::inverse(pt_mat) * global_pos;
		position = Vector3d(global_pos.x, global_pos.y, global_pos.z);
	}
}

void Transform::model_transform()
{

}


void LocalTransform::Update(float deltaTime)
{
	if (EngineObject * parent = engineObject->relationships.getParent())
	{
		glm::mat4 p_mat = glm::mat4(1.0f);
		p_mat = glm::translate(p_mat, position.glm());

		glm::mat4 rotation_matrix = Quaternion::RotationMatrix(rotation);
		p_mat = p_mat * rotation_matrix;
		p_mat = glm::scale(p_mat, scale.glm());

		position_matrix = p_mat;
	}

	else
	{
		CopyTransform(transform);
	}
}
