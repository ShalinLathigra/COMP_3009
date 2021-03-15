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

void GeometryManager::CreateCube(std::string name) {
	//Source for cube verticies can be found here:
	//https://www3.ntu.edu.sg/home/ehchua/programming/opengl/CG_Examples.html)

	//define our geometry and set its type for rendering
	Geometry* geometry = new Geometry(name, GL_QUADS);

	//The simple method would be to store eight verticies for eight
	//points, then reference those. However, since we want each side
	//to be a different color, we need to set create unique verticies. 

	//face 1
	Vertex *v1 = new Vertex();
	Vertex *v2 = new Vertex();
	Vertex *v3 = new Vertex();
	Vertex *v4 = new Vertex();

	//face 2
	Vertex *v5 = new Vertex();
	Vertex *v6 = new Vertex();
	Vertex *v7 = new Vertex();
	Vertex *v8 = new Vertex();

	//face 3
	Vertex *v9 = new Vertex();
	Vertex *v10 = new Vertex();
	Vertex *v11 = new Vertex();
	Vertex *v12 = new Vertex();

	//face 4
	Vertex *v13 = new Vertex();
	Vertex *v14 = new Vertex();
	Vertex *v15 = new Vertex();
	Vertex *v16 = new Vertex();

	//face 5
	Vertex *v17 = new Vertex();
	Vertex *v18 = new Vertex();
	Vertex *v19 = new Vertex();
	Vertex *v20 = new Vertex();

	//face 6
	Vertex *v21 = new Vertex();
	Vertex *v22 = new Vertex();
	Vertex *v23 = new Vertex();
	Vertex *v24 = new Vertex();

	// Top face (y = 1.0f)
	v1->color_ = glm::vec3(0.0f, 1.0f, 0.0f);    
	v1->pos_ = glm::vec3(1.0f, 1.0f, -1.0f);

	v2->color_ = glm::vec3(0.0f, 1.0f, 0.0f);
	v2->pos_ = glm::vec3(-1.0f, 1.0f, -1.0f);

	v3->color_ = glm::vec3(0.0f, 1.0f, 0.0f);
	v3->pos_ = glm::vec3(-1.0f, 1.0f, 1.0f);

	v4->color_ = glm::vec3(0.0f, 1.0f, 0.0f);
	v4->pos_ = glm::vec3(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	v5->color_ = glm::vec3(1.0f, 0.5f, 0.0f);
	v5->pos_ = glm::vec3(1.0f, -1.0f, 1.0f);

	v6->color_ = glm::vec3(1.0f, 0.5f, 0.0f);
	v6->pos_ = glm::vec3(-1.0f, -1.0f, 1.0f);

	v7->color_ = glm::vec3(1.0f, 0.5f, 0.0f);
	v7->pos_ = glm::vec3(-1.0f, -1.0f, -1.0f);

	v8->color_ = glm::vec3(1.0f, 0.5f, 0.0f);
	v8->pos_ = glm::vec3(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	v9->color_ = glm::vec3(1.0f, 0.0f, 0.0f);
	v9->pos_ = glm::vec3(1.0f, 1.0f, 1.0f);

	v10->color_ = glm::vec3(1.0f, 0.0f, 0.0f);
	v10->pos_ = glm::vec3(-1.0f, 1.0f, 1.0f);

	v11->color_ = glm::vec3(1.0f, 0.0f, 0.0f);
	v11->pos_ = glm::vec3(-1.0f, -1.0f, 1.0f);

	v12->color_ = glm::vec3(1.0f, 0.0f, 0.0f);
	v12->pos_ = glm::vec3(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	v13->color_ = glm::vec3(1.0f, 1.0f, 0.0f);
	v13->pos_ = glm::vec3(1.0f, -1.0f, -1.0f);

	v14->color_ = glm::vec3(1.0f, 1.0f, 0.0f);
	v14->pos_ = glm::vec3(-1.0f, -1.0f, -1.0f);

	v15->color_ = glm::vec3(1.0f, 1.0f, 0.0f);
	v15->pos_ = glm::vec3(-1.0f, 1.0f, -1.0f);

	v16->color_ = glm::vec3(1.0f, 1.0f, 0.0f);
	v16->pos_ = glm::vec3(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	v17->color_ = glm::vec3(0.0f, 0.0f, 1.0f);
	v17->pos_ = glm::vec3(-1.0f, 1.0f, 1.0f);

	v18->color_ = glm::vec3(0.0f, 0.0f, 1.0f);
	v18->pos_ = glm::vec3(-1.0f, 1.0f, -1.0f);

	v19->color_ = glm::vec3(0.0f, 0.0f, 1.0f);
	v19->pos_ = glm::vec3(-1.0f, -1.0f, -1.0f);

	v20->color_ = glm::vec3(0.0f, 0.0f, 1.0f);
	v20->pos_ = glm::vec3(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	v21->color_ = glm::vec3(1.0f, 0.0f, 1.0f);
	v21->pos_ = glm::vec3(1.0f, 1.0f, -1.0f);

	v22->color_ = glm::vec3(1.0f, 0.0f, 1.0f);
	v22->pos_ = glm::vec3(1.0f, 1.0f, 1.0f);

	v23->color_ = glm::vec3(1.0f, 0.0f, 1.0f);
	v23->pos_ = glm::vec3(1.0f, -1.0f, 1.0f);

	v24->color_ = glm::vec3(1.0f, 0.0f, 1.0f);
	v24->pos_ = glm::vec3(1.0f, -1.0f, -1.0f);

	//Add each vertex into the list
	geometry->AddVertex(v1);
	geometry->AddVertex(v2);
	geometry->AddVertex(v3);
	geometry->AddVertex(v4);

	geometry->AddVertex(v5);
	geometry->AddVertex(v6);
	geometry->AddVertex(v7);
	geometry->AddVertex(v8);

	geometry->AddVertex(v9);
	geometry->AddVertex(v10);
	geometry->AddVertex(v11);
	geometry->AddVertex(v12);

	geometry->AddVertex(v13);
	geometry->AddVertex(v14);
	geometry->AddVertex(v15);
	geometry->AddVertex(v16);
	
	geometry->AddVertex(v17);
	geometry->AddVertex(v18);
	geometry->AddVertex(v19);
	geometry->AddVertex(v20);
	
	geometry->AddVertex(v21);
	geometry->AddVertex(v22);
	geometry->AddVertex(v23);
	geometry->AddVertex(v24);

	//add the geometry to the list
	geometryList_.push_back(geometry);
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
	v1->pos_ = glm::vec3(-0.5, -0.5,0);

	v2->color_ = glm::vec3(0, 1, 0);
	v2->pos_ = glm::vec3(0.5, -0.5,0);

	v3->color_ = glm::vec3(0, 1, 0);
	v3->pos_ = glm::vec3(0, 0.366,0);

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
	v1->pos_ = glm::vec3(50, 50,0);

	v2->color_ = glm::vec3(1, 0, 0);
	v2->pos_ = glm::vec3(-50, 50,0);

	v3->color_ = glm::vec3(1, 0, 0);
	v3->pos_ = glm::vec3(-50, -50,0);

	v4->color_ = glm::vec3(1, 0, 0);
	v4->pos_ = glm::vec3(50, -50,0);

	//Add each vertex into the list
	geometry->AddVertex(v1);
	geometry->AddVertex(v2);
	geometry->AddVertex(v3);
	geometry->AddVertex(v4);

	//add the geometry to the list
	geometryList_.push_back(geometry);
}