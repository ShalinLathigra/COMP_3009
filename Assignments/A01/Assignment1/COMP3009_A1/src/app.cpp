#include <app.h>


void OpenGLApp::CreateNodes(void) {
	float size = 4.0;
	float pi = 3.14159265;
	float square_x = 10;
	float square_z = 5;
	SceneNode* square = new SceneNode("platform_1");
	square->SetGeometry(manager_->GetGeometry("cube"));
	square->SetTranslation(glm::vec3(0, 0, 0));
	square->SetScale(glm::vec3(square_x, size, square_z));
	nodes.push_back(square);

	SceneNode* sphere = new SceneNode("joint_1");
	sphere->SetGeometry(manager_->GetGeometry("sphere"));
	sphere->SetTranslation(glm::vec3(0, size, 0));
	sphere->SetScale(glm::vec3(size));
	square->AddChild(sphere);

	square = new SceneNode("platform_2");
	square->SetGeometry(manager_->GetGeometry("cube"));
	square->SetTranslation(glm::vec3(0, size, 0));
	square->SetScale(glm::vec3(square_x, size, square_z));
	sphere->AddChild(square);

	sphere = new SceneNode("joint_2");
	sphere->SetGeometry(manager_->GetGeometry("sphere"));
	sphere->SetTranslation(glm::vec3(0, size, 0));
	sphere->SetScale(glm::vec3(size));
	square->AddChild(sphere);

	square = new SceneNode("platform_3");
	square->SetGeometry(manager_->GetGeometry("cube"));
	square->SetTranslation(glm::vec3(0, size, 0));
	square->SetScale(glm::vec3(square_x, size, square_z));
	sphere->AddChild(square);

}

void OpenGLApp::CreateGeometry() {
	manager_->CreateCube("cube");
	manager_->CreateCylinder("cylinder");
	manager_->CreateSphere("sphere");
	
	int d = 3;
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
	float trans_speed = 1;
	float angle_turn = 20;
	SceneNode* selected = instance_->GetSceneNode(instance_->selected_);
	float dir = 1;
	switch (key) {

	case '8':
		instance_->renderType = FILL;
		break;
	case '9':
		instance_->renderType = WIREFRAME;
		break;
	case '1':
		instance_->selected_ = "platform_1";
		instance_->axis = glm::vec3(0, 1, 0);
		break;
	case '2':
		instance_->selected_ = "joint_1";
		instance_->axis = glm::vec3(1, 0, 0);
		break;
	case '3':
		instance_->selected_ = "platform_2";
		instance_->axis = glm::vec3(0, 1, 0);
		break;
	case '4':
		instance_->selected_ = "joint_2";
		instance_->axis = glm::vec3(1, 0, 0);
		break;
	case '5':
		instance_->selected_ = "platform_3";
		instance_->axis = glm::vec3(0, 1, 0);
		break;
	case 'x': 
		dir = -1;
	case 'z':
		instance_->GetSceneNode(instance_->selected_)->Rotate(dir, instance_->axis);
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
	numFrames+=10;

	//Need to recall gluttimerFunc. This function will essentially be called
	//once the timer (10 msec) has been reached. 
	glutTimerFunc(10, timerCallback, 0);

	//Rotate the cylinder by 1 degrees in the specified axis, as well as
	//the other shapes.
	/*
	SceneNode* cylinder = instance_->GetSceneNode("cylinder1");
	if (cylinder != NULL)
	{
		cylinder->Rotate(1, glm::vec3(1,0.2,0));
	}
	*/

	// Sphere for comparison at 50, 10, 100
	glm::vec3 axis = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
	
	SceneNode* sphere = instance_->GetSceneNode("sphere1");

	if (sphere != NULL)
	{
		sphere->Rotate(0.1, axis);
	}

	// Actual sphere at 100, 10, 100
	sphere = instance_->GetSceneNode("sphere2");
	float ang = 23.5f * 2.0 * 3.14159265f / 360.0f;
	axis = glm::normalize(glm::vec3(cos(23.5f), sin(23.5f), 0.0f));
	if (sphere != NULL)
	{
		sphere->Rotate(2.5, axis);
	}
	
	SceneNode* child = instance_->GetSceneNode("cube-sphere1");
	if (child != NULL)
	{
		child->Rotate(-0.1, glm::vec3(0,1,0));
	}
	child = instance_->GetSceneNode("cube-sphere2");
	if (child != NULL)
	{
		child->Rotate(0.2, glm::vec3(0,1,0));
	}

	/*
	SceneNode* cube = instance_->GetSceneNode("cube1");
	if (cube != NULL)
	{
		cube->Rotate(1, glm::vec3(0.5, 0.2, 1));
	}
	*/

	glutPostRedisplay();
}

SceneNode* OpenGLApp::GetSceneNode(std::string name) {
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i]->GetName() == name) {
			return nodes[i];
		}
		SceneNode * c = &SceneNode("");
		if (nodes[i]->GetChild(name, &c))
		{
			return c;
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
	camera_->SetCamera(glm::vec3(0, 20, 100), glm::vec3(0, 20, 0 ), glm::vec3(0, 1, 0));
	renderType = FILL;

	//define the geometry functions and objects:
	manager_ = new GeometryManager();
	CreateGeometry();
	CreateNodes();

	PrintOpenGLStats();

	instance_ = this;
	selected_ = "platform_1";
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