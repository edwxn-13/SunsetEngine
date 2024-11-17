#include "../RenderingComponent.h"
#include "../../Utils/ModelParser/ModelParser.h"

class MeshRenderer : public RenderingComponent
{
public:
	MeshRenderer(EngineObject* engineObject, const char* file);
	unsigned int getType();
	void loadMesh() override;
	void setUpMesh() override;
	void renderMesh(unsigned int shader) override;

private:
	Model model;
	SunsetShader shader;
	const char* filename;
};