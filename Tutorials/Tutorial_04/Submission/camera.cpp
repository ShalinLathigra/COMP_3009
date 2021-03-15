#include <iostream>
#include <camera.h>


Camera::Camera(void) : position(0.0, 0.0, 0.0), forwardVector(0.0, 0.0, 100.0), upVector(0.0, 1.0, 0.0)
, nearPlane(1), farPlane(1000), fieldOfView(60), aspectRatio(1.0)
{
	viewMat = glm::lookAt(position, forwardVector, upVector);
	float top = tan((fieldOfView / 2.0)*(glm::pi<float>() / 180.0))*nearPlane;
	float right = top * aspectRatio;
	projMat = glm::perspective(fieldOfView, aspectRatio, nearPlane, farPlane);

	orientation_ = glm::quat();
}

Camera::~Camera(void){
}


//TO DO---------

int Camera::Roll(float angleDeg){

	glm::quat rotation = glm::angleAxis(glm::radians(angleDeg), GetForward());
	orientation_ = glm::normalize(orientation_ * rotation);
	//DO NOT remove the line below:
	viewMat = glm::lookAt(position, position + forwardVector, upVector);
	return 0;
}

int Camera::Pitch(float angleDeg){

	glm::quat rotation = glm::angleAxis(glm::radians(angleDeg), GetSide());
	orientation_ = glm::normalize(orientation_ * rotation);

	//DO NOT remove the line below:
	viewMat = glm::lookAt(position, position + forwardVector, upVector);
	return 0;
}

int Camera::Yaw(float angleDeg){

	glm::quat rotation = glm::angleAxis(glm::radians(angleDeg), GetUp());
	orientation_ = glm::normalize(orientation_ * rotation);
	//DO NOT remove the line below:
	viewMat = glm::lookAt(position, position + forwardVector, upVector);
	return 0;
}

glm::vec3 Camera::MoveForward(float numUnits) {
	position += GetForward() * numUnits;
	//DO NOT remove the line below:
	viewMat = glm::lookAt(position, position + forwardVector, upVector);
	return (position);
}

glm::vec3 Camera::MoveBackward(float numUnits) {
	MoveForward(-numUnits);
	return position;
}

glm::vec3 Camera::MoveUp(float numUnits) {
	position += GetUp() * numUnits;
	//DO NOT remove the line below:
	viewMat = glm::lookAt(position, position + forwardVector, upVector);
	return (position);
}

glm::vec3 Camera::MoveDown(float numUnits) {
	MoveUp(-numUnits);
	return position;
}


glm::vec3 Camera::MoveRight(float numUnits) {
	position += GetSide() * numUnits;
	//DO NOT remove the line below:
	viewMat = glm::lookAt(position, position + forwardVector, upVector);
	return (position);
}

glm::vec3 Camera::MoveLeft(float numUnits) {
	MoveRight(-numUnits);
	return position;
}


void Camera::ZoomIn(float zoom) {

	fieldOfView = glm::max(fieldOfView - zoom, 5.0f);
	//DO NOT remove the line below:
	projMat = glm::perspective(fieldOfView / 2, aspectRatio, nearPlane, farPlane);
}

void Camera::ZoomOut(float zoom) {

	ZoomIn(-zoom);
	//DO NOT remove the line below:
	projMat = glm::perspective(fieldOfView / 2, aspectRatio, nearPlane, farPlane);
}

//---------------------------------

glm::vec3 Camera::GetPosition(void){
	return position;
}


glm::vec3 Camera::GetLookAtPoint(void)	{
	return (position + GetForward());
}

glm::vec3 Camera::GetForward(void) const {
	return forwardVector*orientation_;
}
glm::vec3 Camera::GetSide(void) const {
	glm::vec3 current_side = glm::cross(GetForward(), GetUp());
	current_side = glm::normalize(current_side);
	return current_side;
}
glm::vec3 Camera::GetUp(void) const {
	return upVector* orientation_;
}



glm::mat4 Camera::GetViewMatrix(glm::mat4 *viewMatrix){
	glm::mat4 m = glm::lookAt(position, GetLookAtPoint(), GetUp());
	if (viewMatrix != NULL) *viewMatrix = m;

	return(m);
}


void Camera::SetCamera(glm::vec3 position, glm::vec3 lookAtPoint, glm::vec3 upVector){
	this->position = position;
	this->forwardVector = lookAtPoint - position;
	this->upVector = upVector;
	this->upVector= glm::normalize(this->upVector);
	this->forwardVector = glm::normalize(this->forwardVector);

	viewMat = glm::lookAt(position, position + forwardVector, upVector);

}


int Camera::ChangeForwardVector(float x, float y, float z){
	forwardVector.x = x;
	forwardVector.y = y;
	forwardVector.z = z;

	viewMat = glm::lookAt(position, position + forwardVector, upVector);

	return 0;
}



int Camera::SetPerspectiveView(float fieldOfView, float aspectRatio, float nearPlane, float farPlane){
	this->fieldOfView = fieldOfView;
	this->aspectRatio = aspectRatio;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	
	projMat= glm::perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
	return 0;
}


glm::mat4 Camera::GetProjectionMatrix(glm::mat4 *projMatrix){
	glm::mat4 m= glm::perspective(fieldOfView/2, aspectRatio, nearPlane, farPlane);
	return(m);
}
