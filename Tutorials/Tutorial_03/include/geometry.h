/*
Description:
	Contains the geometry vertex attributes for each shape.
Copyright (c):
	2021 Sharjeel Ali <sharjeelali@cmail.carleton.ca>
	2020 David Mould <mould@scs.carleton.ca>
	2016 Doran Nussbaum <nussbaum@scs.carleton.ca>
*/
#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <string>

//define the Vertex struct containing position and color
struct Vertex {
	glm::vec2 pos_;
	glm::vec3 color_;
};

//The geometry class contains the vertex attributes, the type
// and the name of the shape to create
class Geometry {

private:
	std::vector<Vertex*>  vertexAtt_;

	//This can either be GL_POLYGON or GL_TRIANGLES
	int type_;

	std::string name_;

public:

	Geometry(std::string name, int type);
	~Geometry();

	std::string GetName(void);

	void AddVertex(Vertex* v);

	//Render the geometry using the transformation matrix and input color
	void Render(glm::mat3 trans, glm::vec3 color);

};

#endif

