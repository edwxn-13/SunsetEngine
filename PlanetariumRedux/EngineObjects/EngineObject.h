#pragma once

#include "../Maths/MathHeader.h"
#include "../Components/Component.h"

class EngineObject;

class ObjectFamily
{
	std::vector<EngineObject*> child_objects;
	EngineObject* parent;

public:

	ObjectFamily();
	EngineObject* getParent();
	void setParent(EngineObject* parent);
	EngineObject* getChild(int index);
	void addChild(EngineObject* childObject);

};


class EngineObject
{
	static std::vector<EngineObject*> object_list;

public:

	EngineObject();
	~EngineObject();
	void addComponent(Component* component);

	void removeComponent(Component* component);

	template <class T>
	T* getComponentOfType();

	template <class T>
	std::vector<T*> getComponentsOfType();

	Transform* getTransform();

	void addChild(EngineObject* childObject);

	void removeChild(EngineObject* childObject);

	void Start();
	void Update();
	void FixedUpdate();
	void LateUpdate();

	static EngineObject* getGlobalObjectIndex(int i);
	static int getObjectListSize();
	Transform localTransform;
	Transform transform;
	ObjectFamily relationships;

	bool enabled;

private:
	std::vector<Component*> component_list;
};


/*
class Planet : public EngineObject
{
	unsigned int shaderProgram;

	int radius;
	int segments;
	int vertexCount;

	float orbit_r;
	float orbit_s;

	PerlinClass terrain;
	PerlinClass tBonus;

	Object planet;
	Object ocean;
	Object atmosphere;

public:

	Planet(unsigned int shader, unsigned int ocean_shader, float speed, float orbit)
	{
		terrain = PerlinClass(3, 0.8);
		tBonus = PerlinClass(4, 0.9);

		shaderProgram = shader;
		CreateSphere(180, 90, 200);
		orbit_r = orbit;
		orbit_s = speed;
		m_transform = Transform();

		m_transform.rotation.x += rand() % 20;
	}
	void setupPlanet()
	{
		CreateOcean(180, 90, 208);
	}

	void loadObj() override
	{

	}
	void CreateIcoSphere()
	{

	}

	glm::vec3 calcOrbit() 
	{
		float hComponent = glm::cos((float)glfwGetTime() * orbit_s) * orbit_r;
		float wComponent = glm::sin((float)glfwGetTime() * orbit_s) * orbit_r;

		glm::vec3 orbit_pos = glm::vec3(hComponent, m_transform.position.y,wComponent);

		return orbit_pos;
	}

	void transform(glm::mat4* model)
	{
		(*model) = glm::translate((*model), calcOrbit());
		(*model) = glm::scale((*model), m_transform.scale.glm());
		(*model) = glm::rotate((*model), (float)glfwGetTime() / 8, glm::vec3(0.f, 0.f, 1.f));
		(*model) = glm::rotate((*model), 1.0f,m_transform.rotation.glm());
	}

	void CreateSphere(int sectorCount, int stackCount, float radius) {

		vector<vec3> vert;
		vector<vec3>  uvs;
		vector<vec3> normals;
		vector<vertex> made_verticies;
		vector<triangle> trianglesInSphere;

		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
		float s, t;                                     // vertex texCoord

		float sectorStep = 2 * glm::pi<float>() / sectorCount;
		float stackStep = glm::pi<float>() / stackCount;
		float sectorAngle, stackAngle;
		bool testing = false;
		for (int i = 0; i <= stackCount; i++)
		{
			stackAngle = glm::pi<float>() / 2 - i * stackStep;

			xy = radius * cosf(stackAngle);
			z = radius * sinf(stackAngle);
			for (int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = j * sectorStep;

				x = xy * cosf(sectorAngle);
				y = xy * sinf(sectorAngle);


				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;

				vec3 temp_normal = vec3(nx, ny, nz);
				normals.push_back(temp_normal);

				vec3 temp_vert = vec3(x, y, z);


				s = (float)j / sectorCount;
				t = (float)i / stackCount;

				vec3 adjust = randheight(terrain.getNoise(s,t) + (0.08f * tBonus.getNoise(s, t)), temp_vert);

				Vector3f adj(0);
				temp_vert.x = temp_vert.x + adjust.x;
				temp_vert.y = temp_vert.y + adjust.y;
				temp_vert.z = temp_vert.z + adjust.z;

				vert.push_back(temp_vert);

				vec3 temp_uv = vec3(s, t, 0);
				uvs.push_back(temp_uv);

			}
		}

		std::vector<int> indices;
		std::vector<int> lineIndices;
		int k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);     // beginning of current stack
			k2 = k1 + sectorCount + 1;      // beginning of next stack

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}

		for (int i = 0; i < indices.size(); i += 3)
		{
			int vertSelection = indices[i];
			vertex temp1 = vertex(vert[vertSelection], uvs[vertSelection], normals[vertSelection]);
			vertSelection = indices[i + 1];
			vertex temp2 = vertex(vert[vertSelection], uvs[vertSelection], normals[vertSelection]);
			vertSelection = indices[i + 2];
			vertex temp3 = vertex(vert[vertSelection], uvs[vertSelection], normals[vertSelection]);

			trianglesInSphere.push_back(triangle(temp1, temp2, temp3));

		}

		Object temp = Object();
		temp.tris = trianglesInSphere;
		temp.mtl = Material();
		//temp.mtl.fil_name = "objs/T_Cap_02_D.png";

		int oceanSelect = rand() % 10;
		std::string index = std::to_string(oceanSelect);

		std::string fileroot = "tex/specular/landtex";
		std::string filetype = ".png";
		std::string full_diro = fileroot + index + filetype;

		temp.texture = CreateTexture(full_diro.c_str());
		temp.specular = CreateTexture("tex/specular/planet_specular.png");
		planet = temp;

		terrain.discard();
		terrain.~PerlinClass();

	}

	void CreateOcean(int sectorCount, int stackCount, float radius) {

		vector<vec3> vert;
		vector<vec3>  uvs;
		vector<vec3> normals;
		vector<vertex> made_verticies;
		vector<triangle> trianglesInSphere;

		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
		float s, t;                                     // vertex texCoord

		float sectorStep = 2 * glm::pi<float>() / sectorCount;
		float stackStep = glm::pi<float>() / stackCount;
		float sectorAngle, stackAngle;
		bool testing = false;
		for (int i = 0; i <= stackCount; i++)
		{
			stackAngle = glm::pi<float>() / 2 - i * stackStep;

			xy = radius * cosf(stackAngle);
			z = radius * sinf(stackAngle);
			for (int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = j * sectorStep;

				x = xy * cosf(sectorAngle);
				y = xy * sinf(sectorAngle);


				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;

				vec3 temp_normal = vec3(nx, ny, nz);
				normals.push_back(temp_normal);

				vec3 temp_vert = vec3(x, y, z);


				s = (float)j / sectorCount;
				t = (float)i / stackCount;

				vert.push_back(temp_vert);

				vec3 temp_uv = vec3(s, t, 0);
				uvs.push_back(temp_uv);

			}
		}

		std::vector<int> indices;
		std::vector<int> lineIndices;
		int k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);     // beginning of current stack
			k2 = k1 + sectorCount + 1;      // beginning of next stack

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}

		for (int i = 0; i < indices.size(); i += 3)
		{
			int vertSelection = indices[i];
			vertex temp1 = vertex(vert[vertSelection], uvs[vertSelection], normals[vertSelection]);
			vertSelection = indices[i + 1];
			vertex temp2 = vertex(vert[vertSelection], uvs[vertSelection], normals[vertSelection]);
			vertSelection = indices[i + 2];
			vertex temp3 = vertex(vert[vertSelection], uvs[vertSelection], normals[vertSelection]);

			trianglesInSphere.push_back(triangle(temp1, temp2, temp3));

		}

		Object temp = Object();
		temp.tris = trianglesInSphere;
		temp.mtl = Material();
		//temp.mtl.fil_name = "objs/T_Cap_02_D.png";
		int oceanSelect = rand() % 10;
		std::string index = std::to_string(oceanSelect);

		std::string fileroot = "tex/specular/landtex";
		std::string filetype = ".png";
		std::string full_diro = fileroot + index + filetype;

		temp.texture = CreateTexture(full_diro.c_str());
		temp.specular = CreateTexture("tex/specular/planet_specular.png");
		ocean = temp;

	}

	void setUpObject() override
	{
		glEnable(GL_DEPTH_TEST);

		glGenVertexArrays(1, &planet.VAO);
		glGenBuffers(1, &planet.VBO);
		glBindVertexArray(planet.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, planet.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (planet.tris.size() * 27), planet.tris.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	void setupOcean() 
	{
		glEnable(GL_DEPTH_TEST);

		glGenVertexArrays(1, &ocean.VAO);
		glGenBuffers(1, &ocean.VBO);
		glBindVertexArray(ocean.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, ocean.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (ocean.tris.size() * 27), ocean.tris.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	void renderObj() override
	{
		glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), planet.texture);
		glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), planet.specular);
		glUniform1f(glGetUniformLocation(shaderProgram, "opacity"), 1.0f);
		glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 49.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planet.texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, planet.specular);
		glBindVertexArray(planet.VAO);
		glm::mat4 model = glm::mat4(1.f);
		transform(&model);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, (planet.tris.size() * 3));
		
	}

	void renderOcean() 
	{
		glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), ocean.texture);
		glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), ocean.specular);
		glUniform1f(glGetUniformLocation(shaderProgram, "opacity"), 0.4f);
		glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 49.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ocean.texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ocean.specular);
		glBindVertexArray(ocean.VAO);
		glm::mat4 model = glm::mat4(1.f);
		transform(&model);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, (ocean.tris.size() * 3));
	}

};

class EngineMesh : public EngineObject
{
	unsigned int shaderProgram;
	vector<Object> objs;
	const char* filename;
	bool movement = false; 

public:

	EngineMesh() 
	{
		m_transform = Transform();
	}

	EngineMesh(const char* file, unsigned int shader)
	{
		shaderProgram = shader;
		filename = file;
		m_transform = Transform();
	}


	float movmement() 
	{
		if (movement) 
		{

		}
	}

	void transform(glm::mat4* model)
	{
		(*model) = glm::translate((*model), m_transform.position.glm());
		(*model) = glm::scale((*model), m_transform.scale.glm());
		//(*model) = glm::rotate((*model), (float)glfwGetTime() / 2, glm::vec3(0.f, 1.f, 0.f));
	}

	unsigned int getType()
	{
		return shaderProgram;
	}

	void loadObj() override
	{
		obj_parse(filename, &objs);

		int fire = 0;
	}
	vector<Object> getObj()
	{
		return objs;
	}
	void setUpObject() override
	{
		for (int i = 0; i < objs.size(); i++) {
			objs[i].texture = CreateTexture(objs[i].mtl.fil_name);
			objs[i].specular = CreateTexture(objs[i].mtl.spec_name);
			objs[i].bump = CreateTexture(objs[i].mtl.bump_name);

			glGenVertexArrays(1, &objs[i].VAO);
			glGenBuffers(1, &objs[i].VBO);
			glBindVertexArray(objs[i].VAO);
			glBindBuffer(GL_ARRAY_BUFFER, objs[i].VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (objs[i].tris.size() * 27), objs[i].tris.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);

		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	}

	void renderObj() override
	{
		for (int i = 0; i < objs.size(); i++)
		{


			glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), objs[i].texture);
			glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), objs[i].specular);
			glUniform1i(glGetUniformLocation(shaderProgram, "material.bump"), objs[i].bump);
			glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 49.0f);
			glUniform1f(glGetUniformLocation(shaderProgram, "opacity"), 1.0f);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, objs[i].texture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, objs[i].specular);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, objs[i].bump);


			glBindVertexArray(objs[i].VAO);
			glm::mat4 model = glm::mat4(1.f);
			transform(&model);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, (objs[i].tris.size() * 3));
		}
	}
};*/

#include "EngineObject.inl"