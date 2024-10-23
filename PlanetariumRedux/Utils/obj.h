#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fstream> 
#include <iostream> 
#include <string> 
#include <vector>
#include "file.h"
#include <string> 
#include <GL/glcorearb.h>

using namespace std;

int clamp(int value, int min, int max) 
{
	if (value > max) { return max; }
	if (value < min) { return min; }
	return value;
}
class vec3
{
public:
	float x, y, z;

	vec3() {}
	vec3(float l, float m, float r)
	{
		x = l;
		y = m;
		z = r;
	}
	~vec3() {}
};

class vec2
{
public:
	float x, y;

	vec2() {}
	vec2(float l, float r)
	{ 
		x = l;
		y = r;
	}
	~vec2() {}
};

struct vertex
{
public:
	vec3 vc;
	vec3 tc;
	vec3 nrm;

	vertex() {}
	vertex(vec3 vc_in, vec3 tc_in, vec3 nrm_in)
	{
		vc = vc_in;
		tc = tc_in;
		nrm = nrm_in;
	}


	~vertex() {}
};
struct triangle
{
public:
	vertex verts[3];

	triangle() {}
	triangle(vertex v0, vertex v1, vertex v2)
	{
		verts[0] = v0;
		verts[1] = v1;
		verts[2] = v2;
	}
	~triangle() {}
};

class Material
{
public:
	char mtl_name[256];
	char fil_name[256];
	char spec_name[256];
	char bump_name[256];


	Material() {}
	Material(char* n, char* f, char* s, char * b) 
	{
		strcpy(mtl_name, n); 
		strcpy(fil_name, f);
		strcpy(spec_name, s);
		strcpy(bump_name, b);

	}
	~Material() 
	{
	}
};


class Object
{
public:
	unsigned int VAO;
	unsigned int VBO;
	vector<triangle> tris;
	Material mtl;
	GLuint texture;
	GLuint specular;
	GLuint bump;

	Object() {}
	Object(Material m)
	{
		strcpy(mtl.fil_name, m.fil_name);
		strcpy(mtl.mtl_name, m.mtl_name);
	}
	~Object()
	{
	}
};
string getRootDirectory(const char* filename);

struct vs 
{
	float x, y, z;
};

vs parse_line2(char* temp)
{
	char* token;
	token = strtok(temp, " ");

	float x, y, z = 0;

	int i = 0;
	while (token != NULL) {

		token = strtok(NULL, " ");

		if (token == NULL)
		{
			continue;
		}

		if (i == 0)
		{
			x = stof(token);
		}
		if (i == 1)
		{
			y = stof(token);
		}
		if (i == 2)
		{
			z = stof(token);
		}
		else
		{
			z = 0;
		}
		i++;
	}
	free(temp);

	return vs{ x, y, z };
}

vs parse_line3(char * temp) 
{
	char* token;
	token = strtok(temp, " ");

	float x, y, z = 0;

	int i = 0;
	while (token != NULL) {

		token = strtok(NULL, " ");

		if (token == " ")
		{
			continue;
		}

		if (i == 0) 
		{
			x = stof(token);
		}
		if (i == 1)
		{
			y = stof(token);
		}
		if (i == 2)
		{
			if (token == NULL)
			{
				continue;
			}
			z = stof(token);
		}

		i++;
	}
	free(temp);

	return vs{ x, y, z };
}


vs parse_triangle(char* temp)
{
	char* token;

	token = strtok(temp, "/");

	float x, y, z = 0;

	int i = 0;
	while (token != NULL) {

		if (token == " ")
		{
			continue;
		}

		if (i == 0)
		{
			x = stof(token);
		}
		if (i == 1)
		{
			y = stof(token);
		}
		if (i == 2)
		{
			if (token == NULL)
			{
				continue;
			}
			z = stof(token);
		}

		token = strtok(NULL, "/");

		i++;
	}
	//free(temp);

	return vs{ x, y, z };
}

int mtl_parse(char* filename, vector<Material>* mtls)
{
	ifstream file(filename);
	string line;


	vector<string> image_names;
	vector<string> spec_names;
	vector<string> material_names;
	vector<string> bump_names;


	while (getline(file, line))
	{
		const char* lineChar = line.c_str();
		char* temp = _strdup(lineChar);
		char* token;


		if (!line.find("newmtl"))
		{
			token = strtok(temp, " ");

			while (token != NULL)
			{
				token = strtok(NULL, " ");
				if (token == NULL)
				{
					continue;
				}

				//logic here

				material_names.push_back(token);
			}
			free(temp);

			continue;
		}


		if (!line.find("\tmap_Kd"))
		{
			token = strtok(temp, "	");
			char* image;

			image = strtok(token, " ");

			while (image != NULL)
			{

				
				image = strtok(NULL, " ");

				if (image == NULL)
				{
					continue;
				}

				//logic here
				image_names.push_back(image);


			}
			free(temp);

			continue;
		}

		if (!line.find("map_Ka"))
		{
			token = strtok(temp, " ");
			//char* image;

			//image = strtok(token, " ");

			while (token != NULL)
			{


				token = strtok(NULL, " ");

				if (token == NULL)
				{
					continue;
				}

				//logic here
				image_names.push_back(token);


			}
			free(temp);

			continue;
		}


		if (!line.find("\tmap_Ka"))
		{
			token = strtok(temp, "	");
			char* image;

			image = strtok(token, " ");

			while (image != NULL)
			{


				image = strtok(NULL, " ");

				if (image == NULL)
				{
					continue;
				}

				//logic here
				spec_names.push_back(image);


			}
			free(temp);

			continue;
		}

		if (!line.find("map_Kd"))
		{
			token = strtok(temp, " ");
			//char* image;

			//image = strtok(token, " ");

			while (token != NULL)
			{


				token = strtok(NULL, " ");

				if (token == NULL)
				{
					continue;
				}

				//logic here
				spec_names.push_back(token);


			}
			free(temp);

			continue;
		}


		if (!line.find("\tmap_bump"))
		{
			token = strtok(temp, "	");
			char* image;

			image = strtok(token, " ");

			while (image != NULL)
			{


				image = strtok(NULL, " ");

				if (image == NULL)
				{
					continue;
				}

				//logic here
				bump_names.push_back(image);


			}
			free(temp);

			continue;
		}

		if (!line.find("map_bump"))
		{
			token = strtok(temp, " ");
			//char* image;

			//image = strtok(token, " ");

			while (token != NULL)
			{


				token = strtok(NULL, " ");

				if (token == NULL)
				{
					continue;
				}

				//logic here
				bump_names.push_back(token);


			}
			free(temp);

			continue;
		}
		
		
	}

	string root_dir = getRootDirectory(filename);

	for (int i = 0; i < mtls->size(); i++)
	{
		string image_dir = root_dir + image_names[i];
		const char* imagepath = image_dir.c_str();
		char* tempimage = _strdup(imagepath);


		string spec_dir = root_dir + spec_names[i];
		const char* specpath = spec_dir.c_str();
		char* tempspec = _strdup(specpath);

		string bump_dir = root_dir + bump_names[i];
		const char* bumppath = bump_dir.c_str();
		char* tempbump = _strdup(bumppath);

		char* matname = mtls[0][i].mtl_name;
		mtls[0][i] = Material(matname,tempimage,tempspec,tempbump);
	}
	return 1;
}

string getRootDirectory(const char* filename) 
{
	const char* temp = _strdup(filename);

	string root = temp;

	do
	{
		root.pop_back();
	} while (root[root.size() - 1] != '/');

	//temp = root.c_str();
	return root;
}

int obj_parse(const char * filename, vector<Object> *objs)
{
	vector<Material> materials;
	vector<vec3> vecs;

	vector<vs> triangle_matrix;

	vector<vec3> uvs;

	vector<vec3> normals;

	ifstream file(filename);
	string line;


	string mtl_name;

	vector<string> material_ids;

	vector<int> object_stages;

	int triangleCounter = 0;

	while (getline(file, line)) {
		//Output the text from the file

		const char* lineChar = line.c_str();
		char* temp = _strdup(lineChar);
		char* token;


		//char * lineID = strtok(temp, " ");

		if (!line.find("#"))
		{
			continue;
		}

		if (!line.find("vn"))
		{
			vs tempNorm = parse_line2(temp);
			normals.push_back(vec3(tempNorm.x, tempNorm.y, tempNorm.z));
			continue;
		}

		if (!line.find("vt"))
		{
			vs uvuv = parse_line2(temp);
			uvs.push_back(vec3(uvuv.x, uvuv.y, uvuv.z));
			continue;
		}

		else if (lineChar[0] == 'v')
		{
			vs vertex = parse_line3(temp);
			vecs.push_back(vec3(vertex.x, vertex.y, vertex.z));
			continue;
		}

		else if (!line.find("mtllib"))
		{
			token = strtok(temp, " ");

			while (token != NULL) 
			{
				token = strtok(NULL, " ");
				if (token == NULL)
				{
					continue;
				}

				mtl_name = token;

			}			
			free(temp);

			continue;
		}

		else if (!line.find("usemtl"))
		{
			object_stages.push_back(triangleCounter);
			token = strtok(temp, " ");

			while (token != NULL)
			{
				token = strtok(NULL, " ");
				if (token == NULL)
				{
					continue;
				}

				bool exist = false;
				if(!material_ids.empty())
					for (int i = 0; i < material_ids.size(); i++) 
					{
						if (token == material_ids[i]) 
						{
							exist = true;
						}
					}
				if (!exist) 
				{
					material_ids.push_back(token);
				}
			}
			free(temp);

			continue;
		}

		else if (lineChar[0] == 'f')
		{
			triangleCounter++;
			token = strtok(temp, " ");
			vector<string> strings;

			while (token != NULL)
			{
				token = strtok(NULL, " ");
				if (token == NULL) 
				{
					continue;
				}
				char* copy = _strdup(token);
				strings.push_back(copy);
			}

			for (size_t i = 0; i < strings.size(); i++)
			{
				const char* triangleMap = strings[i].c_str();
				char* trigMap = _strdup(triangleMap);
				vs temp = parse_triangle(trigMap);
				if (temp.x < 1) { temp.x = 1; }
				if (temp.y < 1) { temp.y = 1; }
				if (temp.z < 1) { temp.z = 1; }

				triangle_matrix.push_back(temp);
			}
			free(temp);
		}

	}
	object_stages.push_back(triangleCounter);

	file.close();

	int size = triangle_matrix.size();

	int tNumber = size / 3;

	//initla material parse
	string root_dir = getRootDirectory(filename);

	string material_dir = root_dir + mtl_name;

	const char* temp_material_dir = material_dir.c_str();

	char* char_material_dir = _strdup(temp_material_dir);

	for (size_t i = 0; i < material_ids.size(); i++)
	{
		const char* temp = material_ids[i].c_str();
		char* material_name_id = _strdup(temp);
		materials.push_back(Material(material_name_id, char_material_dir, char_material_dir, char_material_dir));
	}


	mtl_parse(char_material_dir, &materials);

	// ----------------------------------------------------------

	for (int tstage = 1; tstage < object_stages.size(); tstage++) 
	{
		Object new_object = Object();
		for (size_t i = object_stages[tstage - 1]; i < object_stages[tstage]; i++)
		{
			int index = i * 3;

			vertex temp1(vecs[clamp(triangle_matrix[index].x - 1,0,vecs.size() - 1)], uvs[clamp(triangle_matrix[index].y - 1, 0, uvs.size() - 1)] , normals[clamp(triangle_matrix[index].z - 1, 0, normals.size() - 1)]);
			vertex temp2(vecs[clamp(triangle_matrix[index + 1].x - 1, 0, vecs.size() - 1)], uvs[clamp(triangle_matrix[index + 1].y - 1, 0, uvs.size() - 1)], normals[clamp(triangle_matrix[index + 1].z - 1, 0, normals.size() - 1)]);
			vertex temp3(vecs[clamp(triangle_matrix[index + 2].x - 1, 0, vecs.size() - 1)], uvs[clamp(triangle_matrix[index + 2].y - 1, 0, uvs.size() - 1)], normals[clamp(triangle_matrix[index + 2].z - 1, 0, normals.size() - 1)]);

			new_object.tris.push_back(triangle(temp1, temp2, temp3));  //vecs[triangle_matrix[xx].x - 1], vecs[triangle_matrix[xx + 1].x - 1], vecs[triangle_matrix[xx + 2].x - 1]
		}
		new_object.mtl = materials[tstage - 1];
		objs->push_back(new_object);
	}

	//mtl_parse(char_material_dir, &materials);

	//success
	return 1;
}

