#pragma once

class Test
{
public:
	virtual void Model(glm::mat4* model) = 0;
	virtual vector<Object> ParseObj() = 0;
};

class Test0 : public Test
{
public:
	void Model(glm::mat4* model)
	{
	}
	virtual vector<Object> ParseObj()
	{
		vector<Object> objs;
		return objs;
	}
};

class Test1 : public Test
{
public:
	void Model(glm::mat4* model)
	{
		(*model) = glm::rotate((*model), (float)glfwGetTime() / 2, glm::vec3(0.f, 1.f, 0.f));
	}
	virtual vector<Object> ParseObj()
	{
		vector<Object> objs;
		obj_parse("objs/bird/textured_quad.obj", &objs);
		return objs;
	}
};


class Test2 : public Test
{
public:
	void Model(glm::mat4* model)
	{
		(*model) = glm::translate((*model), glm::vec3(0.f, -1.8f, -1.f));
		(*model) = glm::rotate((*model), (float)glfwGetTime() / 2, glm::vec3(0.f, 1.f, 0.f));
		(*model) = glm::scale((*model), glm::vec3(.005f, .005f, .005f));
	}
	virtual vector<Object> ParseObj()
	{
		vector<Object> objs;
		obj_parse("objs/white_oak/white_oak.obj", &objs);
		return objs;
	}
};
