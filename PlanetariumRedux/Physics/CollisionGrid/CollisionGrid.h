#pragma once
#include <vector>
#include <Map>

#include "../Collider/ColliderManager.h"
#include <string>

struct SunsetPhysGridNode 
{
	double height;
	std::vector<Collider *> cell_members;
	SunsetPhysGridNode* children[8] = { nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr };

	void Subdivide();
};

struct SunsetPhysGrid 
{	
	int max_depth;
	long double height = 40000;
	int grid_size = 255;

	std::map<std::string,int> gridCoordinateLookup;
	SunsetPhysGridNode mPhysicsGrid[255 * 255 * 255];

	void InitGrid();
	void UpdateGrid();
	//Debug
	bool renderGrid;
	void SetUpDebugRender();
	void GridDebugRender();
};