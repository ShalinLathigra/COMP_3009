#include <scene_node.h>

SceneNode::SceneNode(std::string name) {
	name_ = name;
	geometry_ = NULL;

	//Make sure the transformation attributes are all set to 0 or 1.
	//If they aren't, the node won't properly render. 
	translation_ = glm::vec2(0);
	pivot_ = glm::vec2(0,0);
	rotation_ = 0;
	scale_ = glm::vec2(1);
	color_ = glm::vec3(-1);
}

void SceneNode::Render() {
	
	//We first define our three transformation matricies 
	//(translation, rotation, scale)
	glm::mat3 translationMatrix = glm::translate(glm::mat3(1.0f), translation_);
	glm::mat3 pivotMatrix = glm::translate(glm::mat3(1.0f), -pivot_);
	glm::mat3 rotationMatrix = glm::rotate(glm::mat3(1.0f), rotation_);
	glm::mat3 scaleMatrix = glm::scale(glm::mat3(1.0f), scale_);

	//Create the final transformation. Remember: Last matrix gets applied first,
	//so order is scale -> rotation -> translation. 
	glm::mat3 transformationMatrix = translationMatrix * glm::inverse(pivotMatrix) * rotationMatrix * pivotMatrix * scaleMatrix;
	
	//Render the geometry
	geometry_->Render(transformationMatrix, color_);

}

void SceneNode::SetGeometry(Geometry* geom) {
	geometry_ = geom;
}

void SceneNode::SetColor(glm::vec3 color) {
	color_ = color;
}
void SceneNode::SetTranslation(glm::vec2 trans) {
	translation_ = trans;
}
void SceneNode::SetPivot(glm::vec2 pivot) {
	pivot_ = pivot;
}
void SceneNode::SetRotation(float angle) {
	rotation_ = angle;
}

void SceneNode::SetScale(glm::vec2 scale) {
	scale_ = scale;
}

void SceneNode::Translate(glm::vec2 trans) {
	translation_ += trans;
}

void SceneNode::Rotate(float angle) {
	rotation_ += angle;

}
