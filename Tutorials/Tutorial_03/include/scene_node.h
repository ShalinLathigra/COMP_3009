/*
Description:
	Contains a scene node object that is rendered to the screen
Copyright (c):
	2021 Sharjeel Ali <sharjeelali@cmail.carleton.ca>
	2020 David Mould <mould@scs.carleton.ca>
	2016 Doran Nussbaum <nussbaum@scs.carleton.ca>
*/
#ifndef SCENE_NODE_H_
#define SCENE_NODE_H_

#include <geometry.h>
#include <glm/gtx/matrix_transform_2d.hpp>

class SceneNode {
public:
	SceneNode(std::string name);

	//Assign a geometry blueprint for the node
	void SetGeometry(Geometry* geom);

	//setters
	void SetTranslation(glm::vec2 trans);
	void SetPivot(glm::vec2 pivot);
	void SetRotation(float angle);
	void SetScale(glm::vec2 scale);

	//Translate the node by a given amount (Adds onto existing translation)
	void Translate(glm::vec2 trans);

	//Rotate the node by a given amount (Adds onto existing rotation)
	void Rotate(float angle);
	
	void SetColor(glm::vec3 color);

	//Render the scene node
	void Render();

private:
	Geometry* geometry_;

	glm::vec2 translation_;
	glm::vec2 pivot_;
	glm::vec3 color_;

	//To maintain aspect ratio, make sure both the x and y values are the same
	glm::vec2 scale_;

	float rotation_;

	std::string name_;
};

#endif