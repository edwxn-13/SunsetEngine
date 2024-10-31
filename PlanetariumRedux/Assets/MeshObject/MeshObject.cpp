#include "MeshObject.h"
#include "../../Components/MeshComponent/MeshComponent.h"
#include "../../Components/Rigidbody/Rigidbody.h"

MeshObject::MeshObject(const char* filename, unsigned int shader)
{
	MeshComponent* meshComponent = new MeshComponent(this, filename, shader);
	//Rigidbody* rigidbody = new Rigidbody(this);
	addComponent(meshComponent);
	//addComponent(rigidbody);
}
