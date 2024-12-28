#include "CollisionGrid.h"

void SunsetPhysGrid::InitGrid()
{
	for (size_t x = 0; x < grid_size; x++) 
	{
		for (size_t y = 0; y < grid_size; y++) 
		{
			for (size_t z = 0; z < grid_size; z++)
			{
				std::string coordinate = "(" + x + ',' + y + ',' + z + ')';
				int index = x + grid_size * (y + grid_size * z);
				gridCoordinateLookup.insert(std::map<std::string, int>::value_type(coordinate, index));
				mPhysicsGrid[index] = SunsetPhysGridNode();
			}
		}
	}

}
