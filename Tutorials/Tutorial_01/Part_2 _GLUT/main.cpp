/*
Description: 
	Program that renders a red background to a window using GLUT
Copyright (c):
	2021 Sharjeel Ali <sharjeelali@cmail.carleton.ca>
	2016 Doran Nussbaum <nussbaum@scs.carleton.ca>
*/

#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

#include <string>
#include <stdio.h>
#include <iostream>


glm::vec3 background_color;

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400


void  Render() 
{
	//Sets the background color of the buffer to clear to
	glClearColor(background_color[0],
		background_color[1],
		background_color[2], 0.0);

	//Clears the buffer/window each frame. input parameters are which buffers to clear. 
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3ub(255, 255, 255); // changed glColor3i to glColor3ub (see below)
	glVertex2f(10, 10);
	glVertex2f(100, 10);
	glVertex2f(100, 100);
	glVertex2f(10, 100);
	glEnd();

	glFlush();
}

int main(int arg, char ** argv) {

	//initalize the glut library
	glutInit(&arg, argv);

	//declare window attributes
	int width, height, id;

	//set the background to be an rgb value of red (255,0,0)
	background_color = glm::vec3(1, 0, 0);

	//set the title
	std::string title = "COMP 3009: Tutorial 1 - GLUT ";

	//set the window width and height
	width = WINDOW_WIDTH;
	height = WINDOW_HEIGHT;


	//set the window position on the screen
	glutInitWindowPosition(50, 50);

	//set the window size
	glutInitWindowSize(width, height);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	//create the glut window
	id = glutCreateWindow(title.c_str());

	// change perspective
	glOrtho(0.0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1, 0, -1.0, 1.0);

	//set the callback function to use
	glutDisplayFunc(Render);

	//set the proper buffers for the window

	//initialize the glew extension
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Could not initialize glew");
		return (0);
	}

	//output the OpenGL information
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	//start the event loop
	glutMainLoop();
	return 0;
}