#include <app.h>


void OpenGLApp::CreateNodes(void) {

	//First step is to create a new scene node object
	SceneNode* base = new SceneNode("cube1");

	//set its geometry/blueprint. This means it will use
	//the square vertex attributes when rendering the shape. 
	base->SetGeometry(manager_->GetGeometry("cube"));

	//Translate by 50 forward. 
	base->SetTranslation(glm::vec3(0, 0,50));

	//set the size to be 2x. 
	base->SetScale(glm::vec3(2,2,2));
	
	//always remember to push the object into the nodes array. 
	nodes.push_back(base);

	base = new SceneNode("cube2");
	base->SetGeometry(manager_->GetGeometry("cube"));
	base->SetTranslation(glm::vec3(0, 0, -50));
	base->SetScale(glm::vec3(2, 2, 2));
	nodes.push_back(base);

	base = new SceneNode("cube3");
	base->SetGeometry(manager_->GetGeometry("cube"));
	base->SetTranslation(glm::vec3(50,0,0));
	base->SetScale(glm::vec3(2, 2, 2));
	nodes.push_back(base);

	base = new SceneNode("cube4");
	base->SetGeometry(manager_->GetGeometry("cube"));
	base->SetTranslation(glm::vec3(-50,0,0));
	base->SetScale(glm::vec3(2, 2, 2));
	nodes.push_back(base);
}

void OpenGLApp::CreateGeometry() {
	manager_->CreateCube("cube");
}

void  RenderCallback() {
	
	//Create the  matricies that will store view and projection
	glm::mat4 viewMat, projMat;

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ensure that faces are displayed from every view point
	glDisable(GL_CULL_FACE);		
	
	//First we set the projection matrix. 
	//Set the matrix mode to be projection, then get the 
	//matrix from the camera. Finally, load it into OpenGL. 
	glMatrixMode(GL_PROJECTION);
	projMat = instance_->camera_->GetProjectionMatrix(NULL);

	//use value_ptr to quickly convert glm structures into array format that can 
	//be accessed by pointers. 
	glLoadMatrixf(glm::value_ptr(projMat));  

	//Similarly, load the modal view matrix. 
	glMatrixMode(GL_MODELVIEW);
	viewMat = instance_->camera_->GetViewMatrix(NULL);
	glLoadMatrixf(glm::value_ptr(viewMat));

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

	glutSwapBuffers();
}

void KeyboardInputCallback(unsigned char key, int x, int y) {

	float speed = 5;
	float angle_turn = 20;
	switch (key) {

	case '1':
		instance_->renderType = FILL;
		break;
	case '2':
		instance_->renderType = WIREFRAME;
		break;
	case '3':
		instance_->nodes[0]->Translate(glm::vec3(0,0,-1));
	case 'w':
		instance_->camera_->MoveForward(speed);
		break;
	case 's':
		instance_->camera_->MoveBackward(speed);
		break;
	case 'a':
		instance_->camera_->MoveLeft(speed);
		break;
	case 'd':
		instance_->camera_->MoveRight(speed);
		break;
	case 'z':
		instance_->camera_->MoveDown(speed);
		break;
	case 'x':
		instance_->camera_->MoveUp(speed);
		break;
	case 'n':
		instance_->camera_->Roll(angle_turn);
		break;
	case 'm':
		instance_->camera_->Roll(-angle_turn);
		break;
	case 'c':
		instance_->camera_->ZoomIn(speed);
		break;
	case 'v':
		instance_->camera_->ZoomOut(speed);
		break;
	}

	//reload the display (calls the RenderCallback() function)
	glutPostRedisplay();
}

void specialKeyboardInputCallback(int key, int x, int y) {

	float angle_turn = 45;

	switch (key) {
	case GLUT_KEY_LEFT:
		instance_->camera_->Yaw(-angle_turn);
		break;
	case GLUT_KEY_RIGHT:
		instance_->camera_->Yaw(angle_turn);
		break;
	case GLUT_KEY_UP:
		instance_->camera_->Pitch(-angle_turn);
		break;
	case GLUT_KEY_DOWN:
		instance_->camera_->Pitch(angle_turn);
		break;
	}
	glutPostRedisplay();
}
void ResizeCallback(int width, int height) {
	
	glViewport(0, 0, width, height);
}

void timerCallback(int operation) {

	//Increment the total frames
	numFrames++;

	//Need to recall gluttimerFunc. This function will essentially be called
	//once the timer (10 msec) has been reached. 
	glutTimerFunc(10, timerCallback, 0);

	//Rotate the cube by 10 degrees in the specified axis. 
	SceneNode* cube = instance_->GetSceneNode("cube1");
	//cube->Rotate(10, glm::vec3(30,0,34));

	glutPostRedisplay();
}

SceneNode* OpenGLApp::GetSceneNode(std::string name) {
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i]->GetName() == name) {
			return nodes[i];
		}
	}
	return NULL;
}
void OpenGLApp::PrintOpenGLStats(void) {
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

int OpenGLApp::Init(int arg, char ** argv) {

	glutInit(&arg, argv);
	
	window_width_ = 512;
	window_height_ = 512;

	//Set the display mode. This time we want the depth buffer 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	//Set the windoe size and position
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_width_, window_height_);

	//set the window name
	window_id_ = glutCreateWindow("COMP3009: Tutorial 4");
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//Need to enable depth test in 3D
	glEnable(GL_DEPTH_TEST);

	//View an object from all sides (Cull back side)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, 512, 512);

	//initialize the glew extension
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Could not initialize glew");
		return (0);
	}

	camera_ = new Camera();
	camera_->SetCamera(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	renderType = FILL;

	//define the geometry functions and objects:
	manager_ = new GeometryManager();
	CreateGeometry();
	CreateNodes();

	PrintOpenGLStats();

	instance_ = this;
	numFrames = 0;

	//set the callback function to use
	glutDisplayFunc(RenderCallback);
	glutReshapeFunc(ResizeCallback);
	glutKeyboardFunc(KeyboardInputCallback);
	glutSpecialFunc(specialKeyboardInputCallback);
	glutTimerFunc(10, timerCallback, 0);

	//start the event loop
	glutMainLoop();
	return 0;
}