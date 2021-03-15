/*
Description:
	Program that renders a cube and a flat surface with textures
Copyright (c):
	2021 Sharjeel Ali <sharjeelali@cmail.carleton.ca>
	2020 David Mould <mould@scs.carleton.ca>
	2018 Oliver van Kaick <Oliver.vanKaick@carleton.ca>
	2016 Doran Nussbaum <nussbaum@scs.carleton.ca>
*/


#include <iostream>
#include <stdexcept>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <camera.h>

#include <fstream>
#include <sstream>
#include <vector>

//Change this file path to point to the resource/ directory containing the shaders. 
#define DIRECTORY "D:/Winter2021/3009/Assignments/A02/code/"

// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "COMP3009: Tutorial 8";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 background(0.0, 0.0, 0.0);

// Globals that define the OpenGL camera view and projection
glm::vec3 camera_position_g(0, 0, 3.0); // Position of camera
glm::vec3 camera_look_at_g(0.0, 0.0, 0.0); // Point looking at
glm::vec3 camera_up_g(0.0, 1.0, 0.0); // Up vector
float camera_near_clip_distance_g = 0.01; // Near clipping plane
float camera_far_clip_distance_g = 1000.0; // Far clipping plane
float camera_fov_g = 60.0; // Field-of-view for projection

//Vertex structure which stores positions
struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 uv;
};

//triangle array of vertices
std::vector<Vertex>  vertices;
std::vector<int>  indices;

//matricies
glm::mat4 view_matrix, projection_matrix;

GLFWwindow* window;
Camera* camera;
float shine = 5.0f;
glm::vec3 light_pos;

typedef struct Geometry {
	GLuint vbo; // OpenGL vertex buffer object
	GLuint ibo; // OpenGL index buffer object
	GLuint vao; //OpenGL vertex array object. Ignore this as its meant for Mac compatability and isn't important for the course. 
	GLuint size; // Size of data to be drawn
} Geometry;

//This function is used to read the shader programs. OpenGL does not read them as a
//specific file type, but instead as simply a text file containing c code. 
std::string LoadTextFile(std::string filename) {

	const char* char_file = filename.c_str();
	std::ifstream f;
	f.open(char_file);
	if (f.fail()) {
		throw(std::ios_base::failure(std::string("Error opening file ") + std::string(filename)));
	}

	std::string content;
	std::string line;
	while (std::getline(f, line)) {
		content += line + "\n";
	}

	f.close();

	return content;
}

//This function loads the fragment and vertex shader. 
GLuint LoadShaders(std::string shaderName) {

	// Create a shader from vertex program source code
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	
	//Read in the shaders into strings
	std::string fragment_shader = LoadTextFile(std::string(DIRECTORY) + "resource/"+shaderName +".frag");
	std::string vertex_shader = LoadTextFile(std::string(DIRECTORY) +"resource/" +shaderName +".vert");

	//Compile the vertex shader from the source string
	const char* const_vertex_shader = vertex_shader.c_str();
	glShaderSource(vs, 1, &const_vertex_shader, NULL);
	glCompileShader(vs);

	// Check if shader compiled successfully
	GLint status;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(vs, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error compiling vertex shader: ") + std::string(buffer)));
	}

	//Compile the fragment shader from the source string
	const char* const_fragment_shader = fragment_shader.c_str();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &const_fragment_shader, NULL);
	glCompileShader(fs);

	// Check if shader compiled successfully
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(fs, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error compiling fragment shader: ") + std::string(buffer)));
	}

	// Create a shader program linking both vertex and fragment shaders together
	GLuint shader = glCreateProgram();
	glAttachShader(shader, vs);
	glAttachShader(shader, fs);
	glLinkProgram(shader);

	// Check if shaders were linked successfully
	glGetProgramiv(shader, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error linking shaders: ") + std::string(buffer)));
	}

	// Delete memory used by shaders, since they were already compiled
	// and linked
	glDeleteShader(vs);
	glDeleteShader(fs);

	return shader;

}

// Callback for when a key is pressed
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	// Quit the program when pressing 'q'
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	// View control
	float rot_factor(1.0f);
	float trans_factor = 1.0f;

	if (key == GLFW_KEY_UP) {
		camera->Pitch(rot_factor);
	}
	if (key == GLFW_KEY_DOWN) {
		camera->Pitch(-rot_factor);
	}
	if (key == GLFW_KEY_LEFT) {
		camera->Yaw(-rot_factor);
	}
	if (key == GLFW_KEY_RIGHT) {
		camera->Yaw(rot_factor);
	}
	if (key == GLFW_KEY_A) {
		camera->Roll(-rot_factor);
	}
	if (key == GLFW_KEY_D) {
		camera->Roll(rot_factor);
	}
	if (key == GLFW_KEY_W) {
		camera->MoveForward(trans_factor);
	}
	if (key == GLFW_KEY_S) {
		camera->MoveBackward(trans_factor);
	}
	if (key == GLFW_KEY_MINUS || key == GLFW_KEY_KP_SUBTRACT) {
		shine -= 1.0f;
	}
	if (key == GLFW_KEY_KP_ADD || key == GLFW_KEY_EQUAL) {
		shine += 1.0f;
	}
}



// Callback for when the window is resized
void ResizeCallback(GLFWwindow* window, int width, int height) {

	// Set OpenGL viewport based on framebuffer width and height
	glViewport(0, 0, width, height);

	// Update projection matrix
	void* ptr = glfwGetWindowUserPointer(window);
	glm::mat4 *projection_matrix = (glm::mat4 *) ptr;
	float top = tan((camera_fov_g / 2.0)*(glm::pi<float>() / 180.0))*camera_near_clip_distance_g;
	float right = top * width / height;
	(*projection_matrix) = glm::frustum(-right, right, -top, top, camera_near_clip_distance_g, camera_far_clip_distance_g);
}


Geometry* CreateCube(float width=0.5, float height=0.5, float depth=0.5) {

	// Number of vertices and faces to be created
	const GLuint vertex_num = 24;
	const GLuint face_num = 6*2; 

	// Number of attributes for faces
	const int face_att = 3; 

	// Allocate memory for buffers
	vertices.resize(vertex_num);
	indices.resize(face_num*face_att);

	//Although a cube has 8 vertices, we need to duplicate them for each side (24) because
	//of the normals per vertex
	//+z
	vertices[0].normal = glm::vec3(0, 0, 1);
	vertices[1].normal = glm::vec3(0, 0, 1);
	vertices[2].normal = glm::vec3(0, 0, 1);
	vertices[3].normal = glm::vec3(0, 0, 1);
	vertices[0].pos = glm::vec3(-width / 2, -height / 2, depth / 2);
	vertices[1].pos = glm::vec3(width / 2, -height / 2, depth / 2);
	vertices[2].pos = glm::vec3(width / 2, height / 2, depth / 2);
	vertices[3].pos = glm::vec3(-width / 2, height / 2, depth / 2);
	vertices[0].uv = glm::vec2(0.5, 0.66);
	vertices[1].uv = glm::vec2(0.75, 0.66);
	vertices[2].uv = glm::vec2(0.75, 0.33);
	vertices[3].uv = glm::vec2(0.5, 0.33);

	//-z
	vertices[4].normal = glm::vec3(0, 0, -1);
	vertices[5].normal = glm::vec3(0, 0, -1);
	vertices[6].normal = glm::vec3(0, 0, -1);
	vertices[7].normal = glm::vec3(0, 0, -1);
	vertices[4].pos = glm::vec3(-width / 2, -height / 2, -depth / 2);
	vertices[5].pos = glm::vec3(width / 2, -height / 2, -depth / 2);
	vertices[6].pos = glm::vec3(width / 2, height / 2, -depth / 2);
	vertices[7].pos = glm::vec3(-width / 2, height / 2, -depth / 2);
	vertices[4].uv = glm::vec2(0.25, 0.66);
	vertices[5].uv = glm::vec2(0, 0.66);
	vertices[6].uv = glm::vec2(0, 0.33);
	vertices[7].uv = glm::vec2(0.25, 0.33);

	//+x
	vertices[8].normal = glm::vec3(1, 0, 0);
	vertices[9].normal = glm::vec3(1, 0, 0);
	vertices[10].normal = glm::vec3(1, 0, 0);
	vertices[11].normal = glm::vec3(1, 0, 0);
	vertices[8].pos = glm::vec3(width / 2, -height / 2, depth / 2);
	vertices[9].pos = glm::vec3(width / 2, -height / 2, -depth / 2);
	vertices[10].pos = glm::vec3(width / 2, height / 2, -depth / 2);
	vertices[11].pos = glm::vec3(width / 2, height / 2, depth / 2);
	vertices[8].uv = glm::vec2(0.75, 0.66);
	vertices[9].uv = glm::vec2(1, 0.66);
	vertices[10].uv = glm::vec2(1, 0.33);
	vertices[11].uv = glm::vec2(0.75, 0.33);

	//-x
	vertices[12].normal = glm::vec3(-1, 0, 0);
	vertices[13].normal = glm::vec3(-1, 0, 0);
	vertices[14].normal = glm::vec3(-1, 0, 0);
	vertices[15].normal = glm::vec3(-1, 0, 0);
	vertices[12].pos = glm::vec3(-width / 2, -height / 2, -depth / 2);
	vertices[13].pos = glm::vec3(-width / 2, -height / 2, depth / 2);
	vertices[14].pos = glm::vec3(-width / 2, height / 2, depth / 2);
	vertices[15].pos = glm::vec3(-width / 2, height / 2, -depth / 2);
	vertices[12].uv = glm::vec2(0.25, 0.66);
	vertices[13].uv = glm::vec2(0.5, 0.66);
	vertices[14].uv = glm::vec2(0.5, 0.33);
	vertices[15].uv = glm::vec2(0.25, 0.33);

	//+y
	vertices[16].normal = glm::vec3(0, 1, 0);
	vertices[17].normal = glm::vec3(0, 1, 0);
	vertices[18].normal = glm::vec3(0, 1, 0);
	vertices[19].normal = glm::vec3(0, 1, 0);
	vertices[16].pos = glm::vec3(-width / 2, height / 2, depth / 2);
	vertices[17].pos = glm::vec3(width / 2, height / 2, depth / 2);
	vertices[18].pos = glm::vec3(width / 2, height / 2, -depth / 2);
	vertices[19].pos = glm::vec3(-width / 2, height / 2, -depth / 2);
	vertices[16].uv = glm::vec2(0.5, 0.33);
	vertices[17].uv = glm::vec2(0.5, 0);
	vertices[18].uv = glm::vec2(0.25, 0.0);
	vertices[19].uv = glm::vec2(0.25, 0.33);

	//-y
	vertices[20].normal = glm::vec3(0, -1, 0);
	vertices[21].normal = glm::vec3(0, -1, 0);
	vertices[22].normal = glm::vec3(0, -1, 0);
	vertices[23].normal = glm::vec3(0, -1, 0);
	vertices[20].pos = glm::vec3(-width / 2, -height / 2, -depth / 2);
	vertices[21].pos = glm::vec3(width / 2, -height / 2, -depth / 2);
	vertices[22].pos = glm::vec3(width / 2, -height / 2, depth / 2);
	vertices[23].pos = glm::vec3(-width / 2, -height / 2, depth / 2);
	vertices[20].uv = glm::vec2(0.25, 0.66);
	vertices[21].uv = glm::vec2(0.25, 1);
	vertices[22].uv = glm::vec2(0.5, 1);
	vertices[23].uv = glm::vec2(0.5, 0.66);

	//assign colors
	for (int i = 0; i < vertex_num; i++) {
		vertices[i].color = glm::vec3(1, (float)i / (float)vertex_num, 0.0);
	}

	int arr[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		8, 9, 10,
		10, 11, 8,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		20, 21, 22,
		22, 23, 20,
		// top
		16, 17, 18,
		18, 19, 16
	};

	indices.assign(arr, arr + face_att * face_num);

	// Create geometry
	Geometry* geom = new Geometry();
	geom->size = face_num * face_att;

	glGenVertexArrays(1, &geom->vao);
	glBindVertexArray(geom->vao);

	// Create OpenGL buffer for vertices
	glGenBuffers(1, &geom->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, geom->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertex_num, &vertices[0], GL_STATIC_DRAW);

	// Create OpenGL buffer for faces
	glGenBuffers(1, &geom->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_num * face_att*sizeof(int), &indices[0], GL_STATIC_DRAW);

	// Free data buffers
	vertices.clear();
	indices.clear();

	return geom;
}

Geometry* CreateSquare() {

	vertices.resize(4);
	indices.resize(6);

	vertices[0].pos = glm::vec3(-1, -1, 0);
	vertices[1].pos = glm::vec3(-1,  1, 0);
	vertices[2].pos = glm::vec3( 1,  1, 0);
	vertices[3].pos = glm::vec3( 1, -1, 0);

	vertices[0].normal = glm::vec3(0, 0, 1);
	vertices[1].normal = glm::vec3(0, 0, 1);
	vertices[2].normal = glm::vec3(0, 0, 1);
	vertices[3].normal = glm::vec3(0, 0, 1);

	vertices[0].color = glm::vec3(1, 1, 1);
	vertices[1].color = glm::vec3(0, 0, 1);
	vertices[2].color = glm::vec3(0, 1, 0);
	vertices[3].color = glm::vec3(1, 0, 0);

	vertices[0].uv = glm::vec2(0, 1);
	vertices[1].uv = glm::vec2(0, 0);
	vertices[2].uv = glm::vec2(1, 0);
	vertices[3].uv = glm::vec2(1, 1);

	int arr[] = {
		0, 2, 1,
		0, 3, 2
	};

	indices.assign(arr, arr + 6);

	GLuint face[] = { 0, 2, 1,
					 0, 3, 2 };

	// Create geometry
	Geometry* geom = new Geometry();
	geom->size = 6;

	glGenVertexArrays(1, &geom->vao);
	glBindVertexArray(geom->vao);

	// Create OpenGL buffer for vertices
	glGenBuffers(1, &geom->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, geom->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, &vertices[0], GL_STATIC_DRAW);

	// Create OpenGL buffer for faces
	glGenBuffers(1, &geom->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), &indices[0], GL_STATIC_DRAW);

	// Free data buffers
	vertices.clear();
	indices.clear();

	return geom;
}

Geometry* CreateCylinder(float height=3, float circle_radius=0.5, int num_height_samples=2, int num_circle_samples= 100) {


	// Number of vertices and faces to be created
	const GLuint vertex_num = (num_height_samples+2) * num_circle_samples + 2; // plus two for top and bottom
	const GLuint face_num = num_height_samples * (num_circle_samples - 1) * 2 + 2 * num_circle_samples; // two extra rings worth for top and bottom

	// Number of attributes for faces
	const int face_att = 3; // Vertex indices (3)

	// Allocate memory for buffers
	vertices.resize(vertex_num);
	indices.resize(face_num*face_att);

	// Create vertices 
	float theta; // Angle for circle
	float h; // height
	float s, t; // parameters zero to one
	glm::vec3 loop_center;
	glm::vec3 vertex_position;
	glm::vec3 vertex_normal;
	glm::vec3 vertex_color;

	for (int i = 0; i < num_height_samples; i++) { // along the side

		s = i / (float)num_height_samples; // parameter s (vertical)
		h = (-0.5 + s)*height;
		for (int j = 0; j < num_circle_samples; j++) { // small circle
			t = j / (float)num_circle_samples;
			theta = 2.0*glm::pi<GLfloat>()*t; // circle sample (angle theta)

			// Define position, normal and color of vertex
			vertex_normal = glm::vec3(cos(theta), 0.0f, sin(theta));
			vertex_position = glm::vec3(cos(theta)*circle_radius, h, sin(theta)*circle_radius);

			// Add vectors to the data buffer
			vertices[(i*num_circle_samples + j)].pos = vertex_position;
			vertices[(i*num_circle_samples + j)].normal = vertex_normal;
			vertices[(i*num_circle_samples + j)].color = glm::vec3(1.0 - s, t, s); 
			vertices[(i*num_circle_samples + j)].uv = glm::vec2(s, t);

			//add the bottom vertices
			if (i == 0) {
				vertices[(num_height_samples*num_circle_samples + j)].pos = vertex_position;
				vertices[(num_height_samples*num_circle_samples + j)].normal = glm::vec3(0,-1,0);
				vertices[(num_height_samples*num_circle_samples + j)].color = glm::vec3(1.0 - s, t, s);
				vertices[(num_height_samples*num_circle_samples + j)].uv = glm::vec2(s, t);
			}
			//add the top vertices
			if (i == (num_height_samples-1)) {
				vertices[((num_height_samples + 1)*num_circle_samples + j)].pos = vertex_position;
				vertices[((num_height_samples + 1)*num_circle_samples + j)].normal = glm::vec3(0, 1, 0);
				vertices[((num_height_samples + 1)*num_circle_samples + j)].color = glm::vec3(1.0 - s, t, s);
				vertices[((num_height_samples + 1)*num_circle_samples + j)].uv = glm::vec2(s, t);
			}
		}
	}
	
	int topvertex = num_circle_samples * (num_height_samples+2);
	int bottomvertex = num_circle_samples * (num_height_samples+2) + 1; // indices for top and bottom vertex
	//top
	vertex_position = glm::vec3(0, (-0.5 + (num_height_samples - 1.0f) / num_height_samples)*height, 0);
	vertices[topvertex].pos = vertex_position;
	vertices[topvertex].normal = glm::vec3(0, 1, 0);
	vertices[topvertex].color = glm::vec3(1, 0.6, 0.4);
	vertices[topvertex].uv = glm::vec2(0);
	//bottom
	vertex_position = glm::vec3(0, (-0.5)*height, 0); // location of bottom middle of cylinder
	vertices[bottomvertex].pos = vertex_position;
	vertices[bottomvertex].normal = glm::vec3(0, -1, 0);
	vertices[bottomvertex].color = glm::vec3(1, 0.6, 0.4);
	vertices[bottomvertex].uv = glm::vec2(0);

	// Create triangles
	for (int i = 0; i < num_height_samples - 1; i++) {
		for (int j = 0; j < num_circle_samples; j++) {
			// Two triangles per quad
			glm::vec3 t1(((i + 1) % num_height_samples)*num_circle_samples + j,
				i*num_circle_samples + ((j + 1) % num_circle_samples),
				i*num_circle_samples + j);
			glm::vec3 t2(((i + 1) % num_height_samples)*num_circle_samples + j,
				((i + 1) % num_height_samples)*num_circle_samples + ((j + 1) % num_circle_samples),
				i*num_circle_samples + ((j + 1) % num_circle_samples));
			// Add two triangles to the data buffer
			for (int k = 0; k < 3; k++) {
				indices[(i*num_circle_samples + j)*face_att * 2 + k] = (GLuint)t1[k];
				indices[(i*num_circle_samples + j)*face_att * 2 + k + face_att] = (GLuint)t2[k];
			}
		}
	}

	// amount of array filled so far, start adding from here
	int cylbodysize = num_circle_samples * (num_height_samples - 1) * 2; 
	// triangles for top disc (fan shape)

	int i = num_height_samples;
	for (int j = 0; j < num_circle_samples; j++) {
		// Bunch of wedges pointing to the centre
		glm::vec3 botwedge(
			i*num_circle_samples + j,
			bottomvertex,
			i*num_circle_samples + (j + 1) % num_circle_samples
		);

		// note order reversed so that all triangles point outward
		glm::vec3 topwedge(
			(i + 1)*num_circle_samples + (j + 1) % num_circle_samples,
			topvertex,
			(i+1)*num_circle_samples + j
		);

		// Add the triangles to the data buffer
		for (int k = 0; k < 3; k++) {
			indices[(cylbodysize + j)*face_att + k] = (GLuint)topwedge[k];
			indices[(cylbodysize + j + num_circle_samples)*face_att + k] = (GLuint)botwedge[k];
		}
	}

	// Create geometry
	Geometry* geom = new Geometry();
	geom->size = face_num * face_att;

	glGenVertexArrays(1, &geom->vao);
	glBindVertexArray(geom->vao);

	// Create OpenGL buffer for vertices
	glGenBuffers(1, &geom->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, geom->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertex_num, &vertices[0], GL_STATIC_DRAW);

	// Create OpenGL buffer for faces
	glGenBuffers(1, &geom->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_num * face_att * sizeof(int), &indices[0], GL_STATIC_DRAW);

	// Free data buffers
	vertices.clear();
	indices.clear();

	return geom;
}


Geometry* CreateSphere(float radius=0.5, int num_samples_theta= 100, int num_samples_phi=100) {

	// Number of vertices and faces to be created
	const GLuint vertex_num = num_samples_theta * num_samples_phi;
	const GLuint face_num = num_samples_theta * (num_samples_phi - 1) * 2;

	// Number of attributes each face
	const int face_att = 3;

	// Allocate memory for buffers
	vertices.resize(vertex_num);
	indices.resize(face_num*face_att);

	// Create vertices 
	float theta, phi; // Angles for parametric equation

	for (int i = 0; i < num_samples_theta; i++) {

		theta = 2.0*glm::pi<GLfloat>()*i / (num_samples_theta - 1); // angle theta

		for (int j = 0; j < num_samples_phi; j++) {

			phi = glm::pi<GLfloat>()*j / (num_samples_phi - 1); // angle phi

			// Define position, normal and color of vertex
			glm::vec3 vertex_normal = glm::vec3(cos(theta)*sin(phi), sin(theta)*sin(phi), -cos(phi));
			// We need z = -cos(phi) to make sure that the z coordinate runs from -1 to 1 as phi runs from 0 to pi
			// Otherwise, the normal will be invserted
			vertices[i*num_samples_phi + j].pos = glm::vec3(vertex_normal.x*radius, vertex_normal.y*radius, vertex_normal.z*radius);
			vertices[i*num_samples_phi + j].normal = vertex_normal;
			vertices[i*num_samples_phi + j].color = glm::vec3(((float)i) / ((float)num_samples_theta), 1.0 - ((float)j) / ((float)num_samples_phi), ((float)j) / ((float)num_samples_phi));
			vertices[i*num_samples_phi + j].uv = glm::vec2(((float)i) / ((float)num_samples_theta), 1.0 - ((float)j) / ((float)num_samples_phi));
		}
	}

	// Create faces
	for (int i = 0; i < num_samples_theta; i++) {
		for (int j = 0; j < (num_samples_phi - 1); j++) {
			// Two triangles per quad
			glm::vec3 t1(((i + 1) % num_samples_theta)*num_samples_phi + j,
				i*num_samples_phi + (j + 1),
				i*num_samples_phi + j);
			glm::vec3 t2(((i + 1) % num_samples_theta)*num_samples_phi + j,
				((i + 1) % num_samples_theta)*num_samples_phi + (j + 1),
				i*num_samples_phi + (j + 1));
			// Add two triangles to the data buffer
			for (int k = 0; k < 3; k++) {
				indices[(i*(num_samples_phi - 1) + j)*face_att * 2 + k] = (GLuint)t1[k];
				indices[(i*(num_samples_phi - 1) + j)*face_att * 2 + k + face_att] = (GLuint)t2[k];
			}
		}
	}

	// Create geometry
	Geometry* geom = new Geometry();
	geom->size = face_num * face_att;

	glGenVertexArrays(1, &geom->vao);
	glBindVertexArray(geom->vao);

	// Create OpenGL buffer for vertices
	glGenBuffers(1, &geom->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, geom->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertex_num, &vertices[0], GL_STATIC_DRAW);

	// Create OpenGL buffer for faces
	glGenBuffers(1, &geom->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_num * face_att * sizeof(int), &indices[0], GL_STATIC_DRAW);

	// Free data buffers
	vertices.clear();
	indices.clear();

	return geom;
}

GLuint LoadTexture(std::string filename) {

	// Create texture string name
	std::string texture_name = std::string(DIRECTORY) + "assets/" + filename;

	//Load the texture using the SOIL library, and create a new ID for it
	GLuint texture = SOIL_load_OGL_texture(texture_name.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	
	//error handling in case the texture wasn't loaded
	if (!texture) {
		throw(std::ios_base::failure(std::string("Error loading texture ") + std::string(texture_name) + std::string(": ") + std::string(SOIL_last_result())));
	}

	// Create resource
	return texture;
}


//This function sends the matrix into the uniform in the shader 
//uniforms are attributes that are defined in the shader but can be
//accessed outside it. 
void LoadShaderMatrix(GLuint shader, glm::mat4 matrix, std::string name) {

	//First get the uniform from the shader by specifying its name
	GLint shader_mat = glGetUniformLocation(shader, name.c_str());

	//Now load the matrix using the proper uniform (matrix4fv) function. 
	glUniformMatrix4fv(shader_mat, 1, GL_FALSE, glm::value_ptr(matrix));
}

//print opengl info
void PrintOpenGLInfo(void) {

	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

}

void Render(Geometry* geom, GLuint shader,glm::vec3 translation, glm::vec3 scale, glm::quat orientation, GLuint texture = 0, GLuint texture_2 = 0) {

	//Tell OpenGL to use the specific shader
	glUseProgram(shader);

	//Bind the VAO
	glBindVertexArray(geom->vao);

	//Bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, geom->vbo);

	//Bind the IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom->ibo);

	//Load the vertex attributes from the VBO into the shader

	//First access the "vertex" from the shader
	GLint vertex_att = glGetAttribLocation(shader, "vertex");

	//This function works simiarly to the uniform and doesn't need to be used for verticies. 
	//The 2nd parameter is the number of values being selected, the 2nd last parameter
	//is the size of each attribute, and the last is the memory offset location (In case
	//you don't want to read from the begining). 
	glVertexAttribPointer(vertex_att, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(vertex_att);

	GLint color_att = glGetAttribLocation(shader, "color");
	glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
	glEnableVertexAttribArray(color_att);

	GLint normal_att = glGetAttribLocation(shader, "normal");
	glVertexAttribPointer(normal_att, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)24);
	glEnableVertexAttribArray(normal_att);

	GLint uv_att = glGetAttribLocation(shader, "uv");
	glVertexAttribPointer(uv_att, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)36);
	glEnableVertexAttribArray(uv_att);

	//Create the transform matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rotationMatrix = glm::mat4_cast(orientation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 transf = translationMatrix * rotationMatrix * scaleMatrix;

	//Load all the shader matricies into thier uniforms
	LoadShaderMatrix(shader,transf, "world_mat");

	view_matrix = camera->GetViewMatrix(NULL);
	projection_matrix = camera->GetProjectionMatrix(NULL);

	LoadShaderMatrix(shader,view_matrix, "view_mat");
	LoadShaderMatrix(shader,projection_matrix, "projection_mat");

	//load the time uniform
	GLint timer_var = glGetUniformLocation(shader, "time");
	float time = glfwGetTime();
	glUniform1f(timer_var, time);

	//load the time uniform
	GLint shine_var = glGetUniformLocation(shader, "shine");
	glUniform1f(shine_var, shine);

	//load the time uniform
	GLint uLightPos = glGetUniformLocation(shader, "lightPos");
	//glUniform3fv(uLightPos, 1, glm::value_ptr(light_pos));

	//if the texture exists, need to load it
	if (texture != 0) {

		//Get the texture map uniform from the shader
		GLint tex = glGetUniformLocation(shader, "t1");

		// Assign the first texture to the map
		glUniform1i(tex, 0);
		glActiveTexture(GL_TEXTURE0);

		// First texture we bind
		glBindTexture(GL_TEXTURE_2D, texture);

		// Define texture interpolation
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//define how the texture will be wrapped on the edges
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else {
		glBindTexture(shader, 0);
	}

	if (texture_2 != 0) {

		//Get the texture map uniform from the shader
		GLint tex = glGetUniformLocation(shader, "t2");

		// Assign the first texture to the map
		glUniform1i(tex, 1);
		glActiveTexture(GL_TEXTURE1);

		// First texture we bind
		glBindTexture(GL_TEXTURE_2D, texture_2);

		// Define texture interpolation
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//define how the texture will be wrapped on the edges
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else {
		glBindTexture(shader, 0);
	}

	//Finally, draw using GL_TRIANGLES, which reads 3 at a time until it reaches the total size 
	glDrawElements(GL_TRIANGLES, geom->size, GL_UNSIGNED_INT, 0);

}
// Main function that builds and runs the application
int main(void) {

	try {
		// Initialize the window management library (GLFW)
		if (!glfwInit()) {
			throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
		}

		//Mac uses an older version of OpenGL. So we need tos et it to use
		//the neweer core profile. If you're on windows, remove this. 
		#ifdef __APPLE__
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
		#endif

		// Create a window and its OpenGL context
		window = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), NULL, NULL);
		if (!window) {
			glfwTerminate();
			throw(std::runtime_error(std::string("Could not create window")));
		}

		// Make the window's OpenGL context the current one
		glfwMakeContextCurrent(window);

		// Initialize the GLEW library to access OpenGL extensions
		// Need to do it after initializing an OpenGL context
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glDisable(GL_CULL_FACE);


		camera = new Camera();
		camera->SetCamera(glm::vec3(200, 200, 200), glm::vec3(100, 0, 100), glm::vec3(0, 1, 0));

		//Load the shaders
		GLuint gouraudShader = LoadShaders("shaderGouraud");
		GLuint colorShader = LoadShaders("colorShader");
		GLuint textureShader = LoadShaders("textureShader");

		// Create geometry
		Geometry* cube = CreateCube();
		Geometry* cylinder = CreateCylinder();
		Geometry* sphere = CreateSphere();
		Geometry* square = CreateSquare();
		
		//load textures
		GLuint texture1 = LoadTexture("hello_there.png");
		GLuint texture2 = LoadTexture("ottawa.png");

		//Set the proper callbacks. 
		glfwSetWindowUserPointer(window, (void *)&projection_matrix);
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetFramebufferSizeCallback(window, ResizeCallback);

		//Define the three transformation attributes
		glm::quat orientation = glm::angleAxis(0.0f, glm::vec3(0.0, 1.0, 0.0));
		glm::vec3 scale = glm::vec3(20.0f);
		glm::vec3 translation = glm::vec3(0.0);

		light_pos = glm::vec3(240, 200, 210);

		PrintOpenGLInfo();

		float delta;
		float last_time = glfwGetTime();
		while (!glfwWindowShouldClose(window)) {

			//Delta time makes it so the speed of rotation is constant and not dependent
			//on the framerate 
			float current_time = glfwGetTime();
			delta = current_time - last_time;
			last_time = current_time;
			//Clear the screen
			glClearColor(background[0],background[1],background[2], 0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//rotate the geometry
			//glm::quat rotation = glm::angleAxis(delta*30*glm::pi<float>() / 180.0f, glm::normalize(glm::vec3(0.2, 0.5, 0.7)));
			//render the geometry using the gouraud shader
			Render(sphere, gouraudShader,glm::vec3(100,0,100), scale, orientation, texture1, texture2);
			Render(sphere, gouraudShader, glm::vec3(140, 0, 100), scale, orientation, texture2, texture1);
			Render(sphere, gouraudShader, glm::vec3(140, 0, 100), scale, orientation, texture2, texture1);

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}
	catch (std::exception &e) {
		PrintException(e);
	}

	return 0;
}
