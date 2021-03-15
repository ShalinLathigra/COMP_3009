#include <scene_node.h>
#include <iostream>

SceneNode::SceneNode(std::string name) {
	name_ = name;
	geometry_ = NULL;

	//Make sure the transformation attributes are all set to 0 or 1.
	//If they aren't, the node won't properly render. 
	translation_ = glm::vec3(0);
	orientation_ = glm::quat();
	scale_ = glm::vec3(1);
	color_ = glm::vec3(-1);

	children_ = std::vector<SceneNode*>();
}

void SceneNode::AddChild(SceneNode* child)
{
	children_.push_back(child);
}

bool SceneNode::GetChild(std::string name, SceneNode** c)
{
	for (std::vector<SceneNode*>::iterator iter = children_.begin(); iter != children_.end(); iter++)
	{
		if ((*iter)->GetName().compare(name) == 0)
		{
			*c = (*iter);
			return true;
		}
		if ((*iter)->HasChildren())
		{
			if ((*iter)->GetChild(name, c))
			{
				return true;
			}
		}
	}
	return false;
}

bool SceneNode::HasChildren()
{
	return children_.size() > 0;
}


void SceneNode::Render() {
	
	//We first define our three transformation matricies 
	//(translation, rotation, scale)
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation_);
	glm::mat4 rotationMatrix = glm::mat4_cast(orientation_);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale_);

	//Create the final transformation. Remember: Last matrix gets applied first,
	//so order is scale -> rotation -> translation. 
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	
	//Render the geometry
	geometry_->Render(transformationMatrix, color_);
	for (std::vector<SceneNode*>::iterator iter = children_.begin(); iter != children_.end(); iter++)
	{
		std::cout << (*iter)->GetName() << std::endl;
		(*iter)->RenderAsChild(translationMatrix * rotationMatrix);
	}
}

void SceneNode::RenderAsChild(glm::mat4 parentMatrix)
{
	//We first define our three transformation matricies 
	//(translation, rotation, scale)
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation_);
	glm::mat4 rotationMatrix = glm::mat4_cast(orientation_);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale_);

	//Create the final transformation. Remember: Last matrix gets applied first,
	//so order is scale -> rotation -> translation. 
	glm::mat4 transformationMatrix = parentMatrix * translationMatrix * rotationMatrix * scaleMatrix;

	//Render the geometry
	geometry_->Render(transformationMatrix, color_);
	for (std::vector<SceneNode*>::iterator iter = children_.begin(); iter != children_.end(); iter++)
	{
		(*iter)->RenderAsChild(parentMatrix * translationMatrix * rotationMatrix);
	}
}

void SceneNode::SetGeometry(Geometry* geom) {
	geometry_ = geom;
}

void SceneNode::SetColor(glm::vec3 color) {
	color_ = color;
}
void SceneNode::SetTranslation(glm::vec3 trans) {
	translation_ = trans;
}

void SceneNode::SetRotation(float angle, glm::vec3 axis) {

	//Firt we need to normalize the axis, then we can rotate
	//by calling angleAxis which creates a quaternion. 
	axis = glm::normalize(axis);
	orientation_ = glm::angleAxis(angle, axis);
	
	//make sure to always normalize. 
	orientation_ = glm::normalize(orientation_);
}

void SceneNode::SetScale(glm::vec3 scale) {
	scale_ = scale;
}

void SceneNode::Translate(glm::vec3 trans) {
	translation_ += trans;
}

void SceneNode::Rotate(float angle, glm::vec3 axis) {

	//Firt we need to normalize the axis, then we can rotate
	//by calling angleAxis which creates a quaternion. 
	axis = glm::normalize(axis);
	glm::quat rotation = glm::angleAxis(angle, axis);

	//unlike float rotations that are added, for quaternions, 
	//you multiply them. 
	orientation_ *= rotation;

	//make sure to always normalize. 
	orientation_ = glm::normalize(orientation_);

}

std::string SceneNode::GetName(void) {
	return name_;
}