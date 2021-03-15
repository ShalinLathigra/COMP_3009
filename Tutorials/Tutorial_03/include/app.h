/*
Description:
	Contains the main builder for the opengl app
Copyright (c):
	2021 Sharjeel Ali <sharjeelali@cmail.carleton.ca>
	2020 David Mould <mould@scs.carleton.ca>
	2016 Doran Nussbaum <nussbaum@scs.carleton.ca>
*/
#ifndef APP_H_
#define APP_H_

#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

#include <geometry_manager.h>
#include <scene_node.h>


//This is used to switch between the rendering type to 
//either render a wireframe (lines) or fill the shape
enum RenderType {
	WIREFRAME,
	FILL
};

class OpenGLApp {

private:


	void CreateHouse(std::string name, glm::vec2 translation);

	void CreateStar(std::string name, glm::vec2 translation);	// Task 2.1
	void CreateGround(std::string name, glm::vec2 translation);	// Task 2.2

	//This creates each SceneNode object used in the app
	void CreateNodes(void);

	//Creates the basic geometry vertex attributes
	void CreateGeometry();

	GeometryManager* manager_;

	void PrintOpenGLStats(void);

public:

	int Init(int arg, char ** argv);

	std::vector<SceneNode*> nodes;

	glm::vec3 background_color;
	int window_width_, window_height_, window_id_;
	
	RenderType renderType;

};


static OpenGLApp* instance_;

//callback functions
void RenderCallback();
void ResizeCallback(int width, int height);

//handles keyboard input
void KeyboardInputCallback(unsigned char key, int x, int y);

#endif