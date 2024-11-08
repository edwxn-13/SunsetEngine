
#include "EngineObject.h"
#include "../Components/Component.h"
#include <glm/ext/matrix_transform.hpp>

std::vector<EngineObject*> EngineObject::object_list = {};

EngineObject::EngineObject() : transform(this), localTransform(this), enabled(true)
{
	object_list.push_back(this);
}

EngineObject::~EngineObject()
{
	for (int i = 0; i < component_list.size(); i++)
	{
		free(component_list[i]);
	}

	free(this);
}

void EngineObject::addComponent(Component* component)
{
	component_list.push_back(component);
}

void EngineObject::removeComponent(Component* component)
{
	//TODO - change to linked list
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

void EngineObject::Start() 
{
	for (int i = 0; i < component_list.size(); i++)
	{
		component_list[i]->Start();
	}
}
void EngineObject::Update() 
{
	for (int i = 0; i < component_list.size(); i++)
	{
		component_list[i]->Update();
	}
}
void EngineObject::FixedUpdate() 
{
	for (int i = 0; i < component_list.size(); i++)
	{
		component_list[i]->FixedUpdate();
	}
}
void EngineObject::LateUpdate() 
{
	for (int i = 0; i < component_list.size(); i++)
	{
		//component_list[i]->LateUpdate();
	}
}

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
