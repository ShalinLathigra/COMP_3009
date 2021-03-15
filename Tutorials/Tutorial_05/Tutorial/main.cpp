/*
Description:
	Program that creates a triangle using shaders
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
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtx/string_cast.hpp>

#include <fstream>
#include <sstream>
#include <vector>

//Change this file path to point to the resource/ directory containing the shaders. 
#define SHADER_DIRECTORY "D:/Winter2021/3009/Tutorials/Tutorial_05/Tutorial/resource/"

// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "COMP3009: Tutorial 5";
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

//Shader program
GLuint shader;


//Vertex structure which stores positions
struct Vertex {
	glm::vec3 pos;
};

//triangle array of vertices
std::vector<Vertex>  vertices;

//the vbo (vertex buffer object) is used to load vertex data into OpenGL
GLuint vbo;

//(MAC ONLY): We need to declare the VAO (Vertex array object) manually. 
GLuint vao;

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
void LoadShaders(void) {

	// Create a shader from vertex program source code
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	
	//Read in the shaders into strings
	std::string fragment_shader = LoadTextFile(std::string(SHADER_DIRECTORY) + "shader.frag");
	std::string vertex_shader = LoadTextFile(std::string(SHADER_DIRECTORY) + "shader.vert");

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
	shader = glCreateProgram();
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

}

// Callback for when a key is pressed
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	// Quit the program when pressing 'q'
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
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

//This creates a triangle using the vertex structure
void CreateTriangle()
{
	//Need to declare the size of the array first
	vertices.resize(3);

	//Add the positions
	vertices[0].pos = glm::vec3(-1, -1, 0);
	vertices[1].pos = glm::vec3(0, 1, 0);
	vertices[2].pos = glm::vec3(1, -1, 0);

	//Generate the VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Generate the buffers for the vbo
	glGenBuffers(1, &vbo);

	//Bind the vbo to the target (GL_ARRAY_BUFFER)
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Specify the vertex attributes to store in the buffer. 
	//the 2nd parameter specifies the total memory size of the data (Size of a vertex struct 
	//multiplied by the number of vertices). 
	//The 3rd parameter specifies where to start reading in memory. Because this is a vector 
	//container, we need to specify the first entry [0]. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*3, &vertices[0], GL_STATIC_DRAW);

}

//This function sends the matrix into the uniform in the shader 
//uniforms are attributes that are defined in the shader but can be
//accessed outside it. 
void LoadShaderMatrix(glm::mat4 matrix, std::string name) {

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
		GLFWwindow* window;
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

		//Generate the view matrix. 
		glm::mat4 view_matrix = glm::lookAt(camera_position_g, camera_look_at_g, camera_up_g);

		//Generate the projection matrix
		glm::mat4 projection_matrix =  glm::perspective(glm::radians(camera_fov_g)/2, (float)window_width_g/(float)window_height_g, camera_near_clip_distance_g, camera_far_clip_distance_g);
	
		
		PrintOpenGLInfo();
		//Load the shaders
		LoadShaders();

		// Create geometry
		CreateTriangle();

		//Load the vertex attributes from the VBO into the shader
		//First access the "vertex" from the shader
		GLint vertex_att = glGetAttribLocation(shader, "vertex");

		//This function works simiarly to the uniform and doesn't need to be used for verticies. 
		//The 2nd parameter is the number of values being selected, the 2nd last parameter
		//is the size of each attribute, and the last is the memory offset location (In case
		//you don't want to read from the begining). 
		glVertexAttribPointer(vertex_att, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		glEnableVertexAttribArray(vertex_att);

		//Set the proper callbacks. 
		glfwSetWindowUserPointer(window, (void *)&projection_matrix);
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetFramebufferSizeCallback(window, ResizeCallback);

		//Define the three transformation attributes
		glm::quat orientation = glm::angleAxis(0.0f, glm::vec3(0.0, 1.0, 0.0));
		glm::vec3 scale = glm::vec3(0.3);
		glm::vec3 translation = glm::vec3(0.0);

		glUseProgram(shader);

		//returns the current time
		float time = glfwGetTime();


		while (!glfwWindowShouldClose(window)) {

			//Clear the screen
			glClearColor(background[0],background[1],background[2], 0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//Create the transform matrix
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
			glm::mat4 rotationMatrix = glm::mat4_cast(orientation);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
			glm::mat4 transf = translationMatrix * rotationMatrix * scaleMatrix;

			//Load all the shader matricies into thier uniforms
			LoadShaderMatrix(transf, "world_mat");
			LoadShaderMatrix(view_matrix, "view_mat");
			LoadShaderMatrix(projection_matrix, "projection_mat");

			//load the time uniform
			GLint timer_var = glGetUniformLocation(shader, "time");
			time = glfwGetTime();
			glUniform1f(timer_var, time);

			//Finally, draw. 
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}
	catch (std::exception &e) {
		PrintException(e);
	}

	return 0;
}
