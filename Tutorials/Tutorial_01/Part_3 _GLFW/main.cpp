/*
Description: 
	Program that renders a red background to a window using GLFW
Copyright (c):
	2021 Sharjeel Ali <sharjeelali@cmail.carleton.ca>
	2018 David Mould <mould@scs.carleton.ca>
*/

#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <stdio.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>
int main(int arg, char ** argv){

	//initialize glfw library
	if (!glfwInit()) {
		fprintf(stderr, "Could not initialize GLFW");
		return 0;
	}

	//declare window attributes
	int width, height;

	//set the background to be an rgb value of red (255,0,0)
	glm::vec3 background_color = glm::vec3(1,0,0);

	//set the title
	std::string title = "COMP 3009: Tutorial 1 - GLFW ";

	//set the window width and height
	width = 600;
	height = 400;

	//create the window
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	//if the window is NULL, terminate the program
	if (!window) {
		glfwTerminate();
		fprintf(stderr, "Could not initialize window");
		return 0;
	}

	glfwMakeContextCurrent(window); // Set the glfw context

	//Need to set glewExperimental to true to allow OpenGL extensions to be loaded.
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		fprintf(stderr, "Could not initialize glew");
		return 0;
	}

	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	//main loop to process any new changes. Continue until window has been closed.
	while (!glfwWindowShouldClose(window)) {

		//Sets the background color of the buffer to clear to
		glClearColor(background_color[0],
			background_color[1],
			background_color[2], 0.0);

		//Clears the buffer/window each frame. input parameters are which buffers to clear.
		glClear(GL_COLOR_BUFFER_BIT);

		//Use this to poll for input events from callback functions. Removing this line results
		//in the window being unresponsive.
		glfwPollEvents();

		//Draw the new buffer into the window
		glfwSwapBuffers(window);
	}

	return 0;
}
