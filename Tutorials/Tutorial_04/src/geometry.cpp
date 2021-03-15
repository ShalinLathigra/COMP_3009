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

void Geometry::Render(glm::mat4 trans, glm::vec3 color) {

	glBegin(type_);

	glm::vec3 renderColor = color;
	for (int i = 0; i < vertexAtt_.size(); i++) {

		//check if the color is 'empty' (meaning -1), so use the default vertex attribute color. 
		if (color.x == -1)renderColor = vertexAtt_[i]->color_;
		glColor3fv((GLfloat *)&renderColor);

		//We need to create a 3D vector for position to multiply it by a matrix 
		//to get the final position.
		glm::vec4 position4D = trans*glm::vec4(vertexAtt_[i]->pos_,1);

		

		glVertex3fv((GLfloat *)&position4D);
	}
	glEnd();
	
}

std::string Geometry::GetName(void) {
	return name_;
}