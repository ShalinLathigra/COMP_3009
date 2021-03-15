#include <app.h>


void OpenGLApp::CreateHouse(std::string name, glm::vec2 translation) {

	//First step is to create a new scene node object
	SceneNode* base = new SceneNode(name + "_base");

	//set its geometry/blueprint. This means it will use
	//the square vertex attributes when rendering the shape. 
	base->SetGeometry(manager_->GetGeometry("square"));

	//Translate by 100 to the right, 100 up. 
	base->SetTranslation(translation);

	//always remember to push the object into the nodes array. 
	nodes.push_back(base);

	SceneNode* roof = new SceneNode(name + "_roof");
	roof->SetGeometry(manager_->GetGeometry("triangle"));
	roof->SetTranslation(translation + glm::vec2(0, 100));
	nodes.push_back(roof);
}

void OpenGLApp::CreateStar(std::string name, glm::vec2 translation) 
{

	//First step is to create a new scene node object
	SceneNode* base = new SceneNode(name + "_base");

	//set its geometry/blueprint. This means it will use
	//the square vertex attributes when rendering the shape. 
	base->SetGeometry(manager_->GetGeometry("square"));
	base->SetColor(glm::vec3(1.0));
	base->SetScale(glm::vec2(0.25, 0.25));

	//Translate by <translation>
	base->SetTranslation(translation);

	//always remember to push the object into the nodes array. 
	nodes.push_back(base);

	for (float i = 0.0f; i < 360.0f; i += 90.0f)
	{
		SceneNode* wing = new SceneNode(name + "_roof");
		wing->SetGeometry(manager_->GetGeometry("triangle"));
		wing->SetTranslation(translation + glm::vec2(0, 25));
		wing->SetPivot(glm::vec2(0, -25));
		wing->SetRotation(i);
		wing->SetScale(glm::vec2(0.25, 0.25));
		wing->SetColor(glm::vec3(1.0));
		nodes.push_back(wing);
	}
}

void OpenGLApp::CreateGround(std::string name, glm::vec2 translation = glm::vec2(400, 50))
{
	//First step is to create a new scene node object
	SceneNode* base = new SceneNode(name + "_base");

	//set its geometry/blueprint. This means it will use
	//the square vertex attributes when rendering the shape. 
	base->SetGeometry(manager_->GetGeometry("square"));
	base->SetColor(glm::vec3(0.2, 0.4, 0.2));
	base->SetScale(glm::vec2(300, 1));

	//Translate by 100 to the right, 100 up. 
	base->SetTranslation(translation);

	//always remember to push the object into the nodes array. 
	nodes.push_back(base);
}

void OpenGLApp::CreateNodes(void) {
	CreateGround("ground1");
	std::string name = "house_";
	for (int i = 0; i < 3; i++)
	{
		CreateHouse(name + std::to_string(i), glm::vec2(100 + i * 150, 75));
	}
	name = "star_";
	for (int i = 0; i < 3; i++)
	{
		CreateStar(name + std::to_string(i), glm::vec2(80 + (i * 200), 350));
	}

	for (int i = 0; i < 2; i++)
	{
		CreateStar(name + std::to_string(i + 3), glm::vec2(175 + (i * 200), 250));
	}
}

void OpenGLApp::CreateGeometry() {
	                  
	//DO NOT call the same CreateGeometry (square/triangle) function twice. 
	//If you need to render multiple squares/triangles, create a scene node
	//that points to the same geometry (See RenderHouse() above). 
	manager_->CreateSquare("square");
	manager_->CreateTriangle("triangle");

}
void  RenderCallback() {
	
	//Sets the background color of the buffer to clear to
	glClearColor(
		instance_->background_color[0],
		instance_->background_color[1],
		instance_->background_color[2], 0.0);

	//Clears the buffer/window each frame. input parameters are which buffers to clear. 
	glClear(GL_COLOR_BUFFER_BIT);

	//switch between rendering modes (wireframe or filled polygons)
	switch (instance_->renderType) {
	case FILL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	}
	for (std::vector<SceneNode*>::iterator it = instance_->nodes.begin(); it != instance_->nodes.end(); ++it) {
		(*it)->Render();
	}
	glFlush();
}

void KeyboardInputCallback(unsigned char key, int x, int y) {
	switch (key) {

	//press the '1' key to set the rendering mode to filled. 
	case '1':
		instance_->renderType = FILL;
		break;

	//press the '2' key to set the rendering mode to be a wireframe
	case '2':
		instance_->renderType = WIREFRAME;
		break;
	}

	//reload the display (calls the RenderCallback() function)
	glutPostRedisplay();
}

void ResizeCallback(int width, int height) {

	//specifies how to transform the rendered positions into normalized positions
	//by modifying the viewport to be the full window width and height
	glViewport(0, 0, width, height);

	//specifies which matrix to apply the subsquent transformations to (GL_PROJECTION)
	glMatrixMode(GL_PROJECTION);

	//replaces the projection matrix with the identity matrix. 
	glLoadIdentity();

	//set the projection matrix to be orthographic with the following inputs:
	//left, right, bottom, top
	gluOrtho2D(0.0, instance_->window_width_, 0.0, instance_->window_height_);

	//window_width_ = width;
	//window_height_ = height;
}


void OpenGLApp::PrintOpenGLStats(void) {
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}


int OpenGLApp::Init(int arg, char ** argv) {

	renderType = FILL;

	//initalize the glut library
	glutInit(&arg, argv);

	//declare window attributes

	//set the background to be an rgb value of red (255,0,0)
	background_color = glm::vec3(0, 0, 0);

	//set the title
	std::string title = "COMP 3009: Tutorial 3";

	//set the window width and height
	window_width_ = 600;
	window_height_ = 400;


	//set the window position on the screen
	glutInitWindowPosition(50, 50);

	//set the window size
	glutInitWindowSize(window_width_, window_height_);

	//create the glut window
	window_id_ = glutCreateWindow(title.c_str());


	
	//set the callback function to use
	instance_ = this;
	glutDisplayFunc(RenderCallback);
	glutReshapeFunc(ResizeCallback);
	glutKeyboardFunc(KeyboardInputCallback);

	//set the proper buffers for the window
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	//initialize the glew extension
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Could not initialize glew");
		return (0);
	}

	//define the geometry functions and objects:
	manager_ = new GeometryManager();
	CreateGeometry();
	CreateNodes();

	PrintOpenGLStats();

	//start the event loop
	glutMainLoop();
	return 0;
}