#include <geometry.h>


Geometry::Geometry(std::string name, int type) : name_(name), type_(type){}

Geometry::~Geometry() {

	for (std::vector<Vertex*>::iterator it = vertexAtt_.begin(); it != vertexAtt_.end(); ++it) {
		delete (*it);
	}
	vertexAtt_.clear();
}

void Geometry::AddVertex(Vertex* v) {
	vertexAtt_.push_back(v);
}

void Geometry::Render(glm::mat3 trans, glm::vec3 color) {

	glBegin(type_);

	for (int i = 0; i < vertexAtt_.size(); i++) {

		//check if the color is 'empty' (meaning -1), so use the default vertex attribute color. 
		if (color.x == -1)color = vertexAtt_[i]->color_;
		glColor3fv((GLfloat *)&color);

		//We need to create a 3D vector for position to multiply it by a matrix 
		//to get the final position.
		glm::vec3 position3D = trans*glm::vec3(vertexAtt_[i]->pos_,1);

		//Convert the 3D vector into 2D to get the final transformed position
		glm::vec2 pos = glm::vec2(position3D.x, position3D.y);

		glVertex2fv((GLfloat *)&pos);
	}
	glEnd();
	
}

std::string Geometry::GetName(void) {
	return name_;
}