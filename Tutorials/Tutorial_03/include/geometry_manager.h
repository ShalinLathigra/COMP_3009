/*
Description:
	Manages and stores the geometry objects.
Copyright (c):
	2021 Sharjeel Ali <sharjeelali@cmail.carleton.ca>
	2020 David Mould <mould@scs.carleton.ca>
	2016 Doran Nussbaum <nussbaum@scs.carleton.ca>
*/
#ifndef GEOMETRY_MANAGER_H_
#define GEOMETRY_MANAGER_H_

#include <geometry.h>

class GeometryManager {
public:

	//Geometry creation functions. 
	//If you want to add in more shapes, create a function here. 
	//Note: When creating your geometry, make sure its centered on the origin
	//(0,0). Look at the following functions in geometry_manager.cpp for examples. 
	void CreateTriangle(std::string name);
	void CreateSquare(std::string name);

	//Get a geometry object using the name as the identifier. 
	Geometry* GetGeometry(std::string name);

private:
	std::vector<Geometry*> geometryList_;
};

#endif