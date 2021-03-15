#include <geometry.h>


Geometry::Geometry(std::string name) : name_(name){}

Geometry::~Geometry() {

	for (std::vector<Segment*>::iterator it = segments_.begin(); it != segments_.end(); it++) {
		delete (*it);
	}
	
	segments_.clear();
}


void Geometry::AddSegment(int renderType) {
	Segment* segment = new Segment();
	segment->RenderType_ = renderType;
	segment->vertexAtt_  = new std::vector < Vertex*>;

	segments_.push_back(segment);
}
void Geometry::AddVertex(Vertex* v, int segmentIndex) {
	segments_.at(segmentIndex)->vertexAtt_->push_back(v);
}

void Geometry::Render(glm::mat4 trans, glm::vec3 color) {

	glm::vec3 renderColor = color;
	for (int i = 0; i < segments_.size(); i++) {

		glBegin(segments_[i]->RenderType_);

		for (int j = 0; j < segments_[i]->vertexAtt_->size(); j++) {
			//check if the color is 'empty' (meaning -1), so use the default vertex attribute color. 
			if (color.x == -1)renderColor = segments_[i]->vertexAtt_->at(j)->color_;
			glColor3fv((GLfloat *)&renderColor);

			//We need to create a 3D vector for position to multiply it by a matrix 
			//to get the final position.
			glm::vec4 position4D = trans * glm::vec4(segments_[i]->vertexAtt_->at(j)->pos_, 1);

			glVertex3fv((GLfloat *)&position4D);
		}
		glEnd();



	}
	
}

std::string Geometry::GetName(void) {
	return name_;
}