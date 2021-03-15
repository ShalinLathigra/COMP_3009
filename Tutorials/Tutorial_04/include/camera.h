/*
Description:
	Contains a camera class
Copyright (c):
	2021 Sharjeel Ali <sharjeelali@cmail.carleton.ca>
	2020 David Mould <mould@scs.carleton.ca>
	2016 Doran Nussbaum <nussbaum@scs.carleton.ca>
*/
#ifndef CAMERA_H_
#define CAMERA_H_

#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>
#include <vector>
#include <string>

#pragma once
class Camera
{
public:
	Camera(void);
	~Camera(void);

	//TO DO: Complete the following functions------
	int Roll(float angleDeg);
	int Pitch(float angleDeg);
	int Yaw(float angleDeg);

	glm::vec3 MoveForward(float numUnits);
	glm::vec3 MoveBackward(float numUnits);
	glm::vec3 MoveRight(float numUnits);
	glm::vec3 MoveLeft(float numUnits);
	glm::vec3 MoveUp(float numUnits);
	glm::vec3 MoveDown(float numUnits);

	void ZoomIn(float zoom);
	void ZoomOut(float zoom);
	//---------------------------------------------

	glm::vec3 GetPosition(void);		

	//Get the point the camera's looking at. 
	glm::vec3 GetLookAtPoint(void);	

	//Get the side vector direction
	glm::vec3 GetSide(void) const;

	//Get the forward pointing vector direction.
	glm::vec3 GetForward(void) const;

	//Get the up  pointing vector direction. 
	glm::vec3 GetUp(void) const;

	//Recreate and return the view matrix. 
	glm::mat4 GetViewMatrix(glm::mat4 * viewMatrix);

	//Recreate and return the projection matrix. 
	glm::mat4 GetProjectionMatrix(glm::mat4 *projMatrix);

	//Reset the camera
	void SetCamera(glm::vec3 position, glm::vec3 lookAtPoint, glm::vec3 upVector);

	int ChangeForwardVector(float x, float y, float z);

	int SetPerspectiveView(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

private:
	glm::mat4 viewMat;
	glm::mat4 projMat;
	glm::vec3 position;		
	glm::vec3 upVector;		
	glm::vec3 forwardVector;	

	glm::quat orientation_;
	
	float fieldOfView;
	float aspectRatio;
	float nearPlane;
	float farPlane;
};

#endif