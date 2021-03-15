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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class SceneNode {
public:
	SceneNode(std::string name);

	//Assign a geometry blueprint for the node
	void SetGeometry(Geometry* geom);

	//setters
	void SetTranslation(glm::vec3 trans);

	//Need to specify an angle (degrees) and an axis to rotate by. 
	void SetRotation(float angle, glm::vec3 axis);
	void SetScale(glm::vec3 scale);

	//Translate the node by a given amount (Adds onto existing translation)
	void Translate(glm::vec3 trans);

	//Rotate the node by a given amount (Adds onto existing rotation)
	void Rotate(float angle, glm::vec3 axis);

	void SetColor(glm::vec3 color);

	//Render the scene node
	void Render();

	std::string GetName(void);

private:
	Geometry* geometry_;

	glm::vec3 translation_;
	glm::vec3 color_;

	//To maintain aspect ratio, make sure both the x and y values are the same
	glm::vec3 scale_;

	//Instead of an angle, we use a quaternion for rotation. 
	glm::quat orientation_;

	std::string name_;
};

#endif