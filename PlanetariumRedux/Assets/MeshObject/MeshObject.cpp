#include "MeshObject.h"
#include "../../Components/MeshComponent/MeshComponent.h"
#include "../../Components/Rigidbody/Rigidbody.h"

MeshObject::MeshObject(const char* filename , Scene * s) : EngineObject(s)
{
	MeshComponent* meshComponent = new MeshComponent(this, filename);
	Rigidbody* rigidbody = new Rigidbody(this);
	addComponent(meshComponent);
	addComponent(rigidbody);
}
