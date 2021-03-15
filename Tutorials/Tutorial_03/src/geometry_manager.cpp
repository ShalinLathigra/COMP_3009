#include <geometry_manager.h>


Geometry* GeometryManager::GetGeometry(std::string name) {
	//find the geometry using its name
	for (std::vector<Geometry*>::iterator it = geometryList_.begin(); it != geometryList_.end(); ++it) {
		if ((*it)->GetName().compare(name) == 0) {
			return (*it);
		}
	}
	//if not found, return NULL
	return NULL;
}
void GeometryManager::CreateTriangle(std::string name) {

	//define our geometry and set its type for rendering
	Geometry* geometry = new Geometry(name, GL_TRIANGLES);

	//define the three verticies we will use
	Vertex *v1 = new Vertex();
	Vertex *v2 = new Vertex();
	Vertex *v3 = new Vertex();

	//Assign the specific color and position attributes
	v1->color_ = glm::vec3(0, 1, 0);
	v1->pos_ = glm::vec2(-50, -50);

	v2->color_ = glm::vec3(0, 1, 0);
	v2->pos_ = glm::vec2(50, -50);

	v3->color_ = glm::vec3(0, 1, 0);
	v3->pos_ = glm::vec2(0, 50);

	//Add each vertex into the list
	geometry->AddVertex(v1);
	geometry->AddVertex(v2);
	geometry->AddVertex(v3);

	//add the geometry to the list
	geometryList_.push_back(geometry);
}

void GeometryManager::CreateSquare(std::string name) {

	//define our geometry and set its type for rendering
	Geometry* geometry = new Geometry(name, GL_POLYGON);

	//define the four verticies we will use
	Vertex *v1 = new Vertex();
	Vertex *v2 = new Vertex();
	Vertex *v3 = new Vertex();
	Vertex *v4 = new Vertex();

	//Assign the specific color and position attributes
	v1->color_ = glm::vec3(1, 0, 0);
	v1->pos_ = glm::vec2(50, 50);

	v2->color_ = glm::vec3(1, 0, 0);
	v2->pos_ = glm::vec2(-50, 50);

	v3->color_ = glm::vec3(1, 0, 0);
	v3->pos_ = glm::vec2(-50, -50);

	v4->color_ = glm::vec3(1, 0, 0);
	v4->pos_ = glm::vec2(50, -50);

	//Add each vertex into the list
	geometry->AddVertex(v1);
	geometry->AddVertex(v2);
	geometry->AddVertex(v3);
	geometry->AddVertex(v4);

	//add the geometry to the list
	geometryList_.push_back(geometry);
}