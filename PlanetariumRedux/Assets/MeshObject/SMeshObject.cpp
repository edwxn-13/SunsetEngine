#include "SMeshObject.h"
#include "../../Components/Rigidbody/Rigidbody.h"
#include "../../Components/MeshRenderer/MeshRenderer.h"


SMeshObject::SMeshObject(const char* filename, Scene* s) : EngineObject(s)
{
	MeshRenderer* meshRenderer = new MeshRenderer(this, filename);
	Rigidbody* rigidbody = new Rigidbody(this);
	addComponent(meshRenderer);
	addComponent(rigidbody);
}
