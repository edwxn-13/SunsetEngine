
#include "EngineObject.h"
#include "../Components/Component.h"
#include <glm/ext/matrix_transform.hpp>

std::vector<EngineObject*> EngineObject::object_list = {};

EngineObject::EngineObject() : transform(this), localTransform(this)
{
	object_list.push_back(this);
}

void EngineObject::addComponent(Component* component)
{
	component_list.push_back(component);
}

void EngineObject::removeComponent(Component* component)
{
	//TODO - change to linked list
}

Component* EngineObject::getComponentOfType(int id)
{
	for (int i = 0; i < component_list.size(); i++) {
		if (component_list[i]->get_id() == id) return component_list[i];
	}

	return nullptr;
}

std::vector<Component*> EngineObject::getComponentsOfType(int id)
{
	std::vector<Component*> list = {};

	for (int i = 0; i < component_list.size(); i++) {
		if (component_list[i]->get_id() == id) list.push_back(component_list[i]);
	}
	return list;
}

Transform* EngineObject::getTransform()
{
	return &transform;
}

void EngineObject::addChild(EngineObject * childObject)
{
	childObject->relationships.setParent(this);
	relationships.addChild(childObject);
}

void EngineObject::removeChild(EngineObject* childObject)
{
	//TODO - make linked list for children
}

void EngineObject::Start() {}
void EngineObject::Update() {}
void EngineObject::FixedUpdate() {}
void EngineObject::LateUpdate() {}

EngineObject* EngineObject::getGlobalObjectIndex(int i)
{
	return object_list[i];
}

int EngineObject::getObjectListSize()
{
	return object_list.size();
}

ObjectFamily::ObjectFamily()
{
	
}

EngineObject* ObjectFamily::getParent()
{
	return parent;
}

void ObjectFamily::setParent(EngineObject* parent)
{
	this->parent = parent;
}

EngineObject* ObjectFamily::getChild(int index)
{
	return child_objects[index];
}

void ObjectFamily::addChild(EngineObject* childObject)
{
	child_objects.push_back(childObject);
}
