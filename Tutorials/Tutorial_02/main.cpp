/*
Description:
	Program that renders various geometry using GLUT
Copyright (c):
	2021 Sharjeel Ali <sharjeelali@cmail.carleton.ca>
	2016 Doran Nussbaum <nussbaum@scs.carleton.ca>
*/

#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

#include <algorithm>    // std::min

glm::vec3 background_color;

int window_width_, window_height_, window_id_;

//Each of these vectors stores the specified verticies of the geometry
struct Vertex {
	glm::vec2 point_;
	glm::vec3 color_;
};

std::vector<Vertex> triangle_;
std::vector<Vertex> square_;

std::vector<glm::vec2> curve_;
std::vector<glm::vec2> control_;

std::vector<Vertex> roof_;

enum Shape {TRIANGLE, SQUARE, CURVE, ROOF};


float side_length = 40.0f;
float tile_height = sqrt(pow(side_length, 2) - pow(side_length*0.5, 2));
float tile_width = side_length;

//Create the triangle verticies
void CreateTriangle(void) {
	Vertex v;

	v.point_ = glm::vec2(-tile_width * 0.5, -tile_height * 0.5);
	v.color_ = glm::vec3(1.0, 0.0, 0.0);
	triangle_.push_back(v);

	v.point_ = glm::vec2(0, tile_height * 0.5);
	v.color_ = glm::vec3(0.0, 0.0, 1.0);
	triangle_.push_back(v);

	v.point_ = glm::vec2(tile_width * 0.5, -tile_height * 0.5);
	v.color_ = glm::vec3(0.0, 1.0, 0.0);
	triangle_.push_back(v);
}

void CreateRoof(void) {
	Vertex v;

	v.point_ = glm::vec2(-125, -40);
	v.color_ = glm::vec3(1.0, 0.0, 0.0);
	roof_.push_back(v);

	v.point_ = glm::vec2(0, 40);
	v.color_ = glm::vec3(0.0, 0.0, 1.0);
	roof_.push_back(v);

	v.point_ = glm::vec2(125, -40);
	v.color_ = glm::vec3(0.0, 1.0, 0.0);
	roof_.push_back(v);
}

//Create the square verticies
void CreateSquare(void) {
	Vertex v;

	v.point_ = glm::vec2(-100, 65);
	v.color_ = glm::vec3(1.0, 0.0, 0.0);
	square_.push_back(v);

	v.point_ = glm::vec2(100, 65);
	v.color_ = glm::vec3(0.0, 1.0, 0.0);
	square_.push_back(v);

	v.point_ = glm::vec2(100, -65);
	v.color_ = glm::vec3(0.0, 0.0, 1.0);
	square_.push_back(v);

	v.point_ = glm::vec2(-100, -65);
	v.color_ = glm::vec3(1.0, 1.0, 1.0);
	square_.push_back(v);
}


/*
- Calculate the position of the vertex for a hermite curve.
- For more information, use the following resource which was
  used as a reference for creating this curve:
- https://blog.demofox.org/2015/08/08/cubic-hermite-interpolation/#:~:text=Cubic%20Hermite%20splines%20have%20four,at%20the%20middle%20control%20points.
*/
glm::vec2 CalculateHermite(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D, float t)
{
	//We want to calculate the position using interpolation on both axes. 
	glm::vec2 p;
	for (int i = 0; i < 2; i++) {
		float a = -A[i] / 2.0f + (3.0f*B[i]) / 2.0f - (3.0f*C[i]) / 2.0f + D[i] / 2.0f;
		float b = A[i] - (5.0f*B[i]) / 2.0f + 2.0f*C[i] - D[i] / 2.0f;
		float c = -A[i] / 2.0f + C[i] / 2.0f;
		float d = B[i];
	
		//Use the cubic equation to solve
		p[i] =  a * t*t*t + b * t*t + c * t + d;
	}
	return p;
}

//Create the Hermite Curve verticies
void CreateHermiteCurve() {

	//Sample size per curve segment. This specifies the number of 
	//points per segment that are used to form lines in GLUT. 
	int sample_size = 100;

	//We first require adding in control points:
	control_.push_back(glm::vec2(50, 30));
	control_.push_back(glm::vec2(100, 110));
	control_.push_back(glm::vec2(167, 167));
	control_.push_back(glm::vec2(200, 20));
	control_.push_back(glm::vec2(330, 35));
	control_.push_back(glm::vec2(400, 200));
	control_.push_back(glm::vec2(350, 380));
	control_.push_back(glm::vec2(270, 300));

	glm::vec2 p, p1, p2, p3, p4;

	//Iterate through each control point, then create a segment
	for (int i = 1; i < control_.size()-2; i++) {

		p1 = control_[i-1]; //Used to calculate tangent of p2
		p2 = control_[i];   //Start point of curve segment
		p3 = control_[i+1]; //End point of curve segment
		p4 = control_[i+2]; //Used to calculate tangent of p3

		//Iterate through the sample size using t, and add points
		//to the curve
		for (float t = 0; t < 1.0; t += 1.0 / sample_size) {

			p = CalculateHermite(p1,p2,p3,p4, t);
			curve_.push_back(p);
		}
	}
}

//Render a curve
void RenderCurve(void) {

	//Set the line width of the curve
	glLineWidth(1);

	//Set the geometry to be drawn (lines)
	glBegin(GL_LINE_STRIP);

	//Iterate over each vertex and specify it to the renderer. 
	for (unsigned i = 0; i < curve_.size(); ++i)
	{
		glVertex2fv((GLfloat *)&curve_[i]);
	}

	//Stop sampling vertices
	glEnd(); 
}

void RenderShape(int SHAPE_TO_RENDER, glm::vec3 offset = glm::vec3(window_width_ * 0.5, window_height_ * 0.5, 0.0f), glm::vec3 override_color = glm::vec3(-1))
{
	//Iterate over each vertex and specify it to the renderer. 
	float interp = std::min(1.0f + override_color.r, 1.0f);

	glm::vec4 point;
	glm::mat4 translate_mat;
	glm::vec3 modified_color;
	switch (SHAPE_TO_RENDER) {

	case TRIANGLE: //Draw a triangle

		//Set the geometry to be drawn (triangle)
		glBegin(GL_TRIANGLES);




		for (int i = 0; i < triangle_.size(); i++) {
			modified_color = interp * override_color + (1 - interp) * triangle_[i].color_;
			glColor3fv((GLfloat *)&modified_color);
			point = glm::vec4(triangle_[i].point_.x, triangle_[i].point_.y, 0.0f, 1.0f);

			translate_mat = glm::translate(glm::mat4(), offset);

			point = translate_mat * point;
			glVertex4fv((GLfloat *)&point);
		}

		//Stop sampling vertices
		glEnd();

		break;

	case SQUARE: //Draw a square

		//Set the geometry to be drawn (square)
		glBegin(GL_POLYGON);

		//Iterate over each vertex and specify it to the renderer. 
		for (int i = 0; i < square_.size(); i++) {
			modified_color = interp * override_color + (1 - interp) * square_[i].color_;
			glColor3fv((GLfloat *)&modified_color);
			glm::vec2 translated = square_[i].point_;

			point = glm::vec4(square_[i].point_.x, square_[i].point_.y, 0.0f, 1.0f);

			translate_mat = glm::translate(glm::mat4(), offset);

			point = translate_mat * point;

			glVertex4fv((GLfloat *)&point);
		}

		//Stop sampling vertices
		glEnd();

		break;

	case CURVE: //Draw a curve
	//set the geometry color
		glColor3f(0.0, 1.0, 0.0);
		RenderCurve();
		break;
	case ROOF: //Draw a triangle
		//Set the geometry to be drawn (triangle)
		glBegin(GL_TRIANGLES);
		
		for (int i = 0; i < roof_.size(); i++) {
			modified_color = interp * override_color + (1 - interp) * roof_[i].color_;
			glColor3fv((GLfloat *)&modified_color);
			point = glm::vec4(roof_[i].point_.x, roof_[i].point_.y, 0.0f, 1.0f);

			translate_mat = glm::translate(glm::mat4(), offset);

			point = translate_mat * point;
			glVertex4fv((GLfloat *)&point);
		}

		//Stop sampling vertices
		glEnd();

		break;
	}
	glFlush();
}


void RenderHouse(glm::vec3 offset = glm::vec3())
{
	RenderShape(SQUARE, glm::vec3(0.0f) + offset, glm::vec3(1.0, 0.0, 0.0));
	RenderShape(ROOF, glm::vec3(0.0f, 95.0f, 0.0f) + offset, glm::vec3(0.0, 1.0, 0.0));
}

void RenderNeighborhood() 
{
	RenderHouse(glm::vec3(150.0f + 50.0f,  100.0f + 200.0f, 0.0f));
	RenderHouse(glm::vec3(150.0f + 450.0f, 100.0f + 200.0f, 0.0f));
	RenderHouse(glm::vec3(150.0f + 250.0f, 100.0f + 0.0f, 0.0f));
}

void RenderPyramid(int tiers = 5)
{
	int pyramid_height = tiers * tile_height;
	/*
		Want to center the pyramid such that the top tier is drawn at an offset of exactly (0.0, 1/2 pyramid_height)
		and the center of the last tier is exactly
	*/
	glm::vec3 top = glm::vec3(window_width_ * 0.5f, window_height_ * 0.5f + pyramid_height * 0.5f, 0.0f);
	glm::vec3 source;
	for (int j = 0; j < tiers; j++)
	{
		source = top - glm::vec3(j * tile_width * 0.5f, j * tile_height, 0.0f);
		for (int i = 0; i < j + 1; i++) {
			RenderShape(TRIANGLE, source, glm::vec3(0.0f, 1.0f, 0.0f));
			source.x += tile_width;
		}

	}
}

void ProcessSierpinski(int level, glm::vec3 top)
{
	int pyramid_height = level * tile_height;
	if (level == 1) {
		RenderShape(TRIANGLE, top);
	}
	else if (level > 1) {
		int next_level = level - 1;
		glm::vec3 h_offset = glm::vec3(
			0.0f,
			-1.0f * pow(2, next_level - 1) * tile_height,
			0.0f
		);
		glm::vec3 r_offset = glm::vec3(
			-0.5f * pow(2, next_level - 1) * tile_width,
			0.0f,
			0.0f
		);

		ProcessSierpinski(next_level, top);	//Top Child
		ProcessSierpinski(next_level, top + h_offset - r_offset);	//Left Child
		ProcessSierpinski(next_level, top + h_offset + r_offset);	//Right Child
	}
	/*
		Algorithm for Sierpinski: level, top
			if (level > 1):
				adjustedTop = top
				h_off = 2^(level-1)*tile_height
				w_off = 0.5 * h_off
				adjustedLeft = top - h_off - w_off
				adjustedRight = top - h_off + w_off

				RenderSierpinski(level - 1, adjustedCenter)
				RenderSierpinski(level - 1, adjustedLeft)
				RenderSierpinski(level - 1, adjustedRight)
			else:
				DrawShape(Triangle, center)
	*/
}

void RenderSierpinski(int level, glm::vec3 top = glm::vec3(window_width_ * 0.5f, window_height_ - tile_height, 0.0f)) {
	ProcessSierpinski(level, top);
}
void  Render() {

	//Sets the background color of the buffer to clear to
	glClearColor(background_color[0],
		background_color[1],
		background_color[2], 0.0);

	//Clears the buffer/window each frame. input parameters are which buffers to clear. 
	glClear(GL_COLOR_BUFFER_BIT);
	//Modify this value.
	//int SHAPE_TO_RENDER = SQUARE;
	//RenderShape(TRIANGLE);
	//RenderShape(SQUARE);
	//RenderNeighborhood();
	//RenderPyramid(10);
	//RenderSierpinski(5);
	//RenderShape(CURVE);
}


void Resize(int width, int height) {

	//specifies how to transform the rendered positions into normalized positions
	//by modifying the viewport to be the full window width and height
	glViewport(0, 0, width, height);

	//specifies which matrix to apply the subsquent transformations to (GL_PROJECTION)
	glMatrixMode(GL_PROJECTION); 

	//replaces the projection matrix with the identity matrix. 
	glLoadIdentity();

	//set the projection matrix to be orthographic with the following inputs:
	//left, right, bottom, top
	gluOrtho2D(0.0, window_width_, 0.0, window_height_);
}

void PrintOpenGLInfo(void) {

	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

}

int main(int arg, char ** argv){

	//initalize the glut library
	glutInit(&arg, argv);

	//declare window attributes

	//set the background to be an rgb value of red (255,0,0)
	background_color = glm::vec3(0, 0, 0);

	//set the title
	std::string title = "COMP 3009: Tutorial 2";

	//set the window width and height
	window_width_ = 1000;
	window_height_ = 750;


	//set the window position on the screen
	glutInitWindowPosition(50, 50);

	//set the window size
	glutInitWindowSize(window_width_, window_height_);

	//create the glut window
	window_id_ = glutCreateWindow(title.c_str());

	glutReshapeFunc(Resize);

	//set the callback function to use
	glutDisplayFunc(Render);

	//set the proper buffers for the window
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	//initialize the glew extension
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Could not initialize glew");
		return (0);
	}
	
	//print the opengl information
	PrintOpenGLInfo();

	//create triangle geometry
	CreateTriangle();

	//create square geometry
	CreateSquare();

	//create curve geometry
	CreateHermiteCurve();

	//create roof because scaling is busted
	CreateRoof();

	//start the event loop
	glutMainLoop();
	return 0; 
}