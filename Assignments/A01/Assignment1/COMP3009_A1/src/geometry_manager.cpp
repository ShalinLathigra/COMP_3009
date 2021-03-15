#include <geometry_manager.h>
# include <random>
# include <iostream>


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
	Geometry* geometry = new Geometry(name);
	geometry->AddSegment(GL_QUADS);

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
	v1->pos_ = glm::vec3(0.5f, 0.5f, -0.5f);

	v2->color_ = glm::vec3(0.0f, 1.0f, 0.0f);
	v2->pos_ = glm::vec3(-0.5f, 0.5f, -0.5f);

	v3->color_ = glm::vec3(0.0f, 1.0f, 0.0f);
	v3->pos_ = glm::vec3(-0.5f, 0.5f, 0.5f);

	v4->color_ = glm::vec3(0.0f, 1.0f, 0.0f);
	v4->pos_ = glm::vec3(0.5f, 0.5f, 0.5f);

	// Bottom face (y = -1.0f)
	v5->color_ = glm::vec3(1.0f, 0.5f, 0.0f);
	v5->pos_ = glm::vec3(0.5f, -0.5f, 0.5f);

	v6->color_ = glm::vec3(1.0f, 0.5f, 0.0f);
	v6->pos_ = glm::vec3(-0.5f, -0.5f, 0.5f);

	v7->color_ = glm::vec3(1.0f, 0.5f, 0.0f);
	v7->pos_ = glm::vec3(-0.5f, -0.5f, -0.5f);

	v8->color_ = glm::vec3(1.0f, 0.5f, 0.0f);
	v8->pos_ = glm::vec3(0.5f, -0.5f, -0.5f);

	// Front face  (z = 1.0f)
	v9->color_ = glm::vec3(1.0f, 0.0f, 0.0f);
	v9->pos_ = glm::vec3(0.5f, 0.5f, 0.5f);

	v10->color_ = glm::vec3(1.0f, 0.0f, 0.0f);
	v10->pos_ = glm::vec3(-0.5f, 0.5f, 0.5f);

	v11->color_ = glm::vec3(1.0f, 0.0f, 0.0f);
	v11->pos_ = glm::vec3(-0.5f, -0.5f, 0.5f);

	v12->color_ = glm::vec3(1.0f, 0.0f, 0.0f);
	v12->pos_ = glm::vec3(0.5f, -0.5f, 0.5f);

	// Back face (z = -1.0f)
	v13->color_ = glm::vec3(1.0f, 1.0f, 0.0f);
	v13->pos_ = glm::vec3(0.5f, -0.5f, -0.5f);

	v14->color_ = glm::vec3(1.0f, 1.0f, 0.0f);
	v14->pos_ = glm::vec3(-0.5f, -0.5f, -0.5f);

	v15->color_ = glm::vec3(1.0f, 1.0f, 0.0f);
	v15->pos_ = glm::vec3(-0.5f, 0.5f, -0.5f);

	v16->color_ = glm::vec3(1.0f, 1.0f, 0.0f);
	v16->pos_ = glm::vec3(0.5f, 0.5f, -0.5f);

	// Left face (x = -1.0f)
	v17->color_ = glm::vec3(0.0f, 0.0f, 1.0f);
	v17->pos_ = glm::vec3(-0.5f, 0.5f, 0.5f);

	v18->color_ = glm::vec3(0.0f, 0.0f, 1.0f);
	v18->pos_ = glm::vec3(-0.5f, 0.5f, -0.5f);

	v19->color_ = glm::vec3(0.0f, 0.0f, 1.0f);
	v19->pos_ = glm::vec3(-0.5f, -0.5f, -0.5f);

	v20->color_ = glm::vec3(0.0f, 0.0f, 1.0f);
	v20->pos_ = glm::vec3(-0.5f, -0.5f, 0.5f);

	// Right face (x = 1.0f)
	v21->color_ = glm::vec3(1.0f, 0.0f, 1.0f);
	v21->pos_ = glm::vec3(0.5f, 0.5f, -0.5f);

	v22->color_ = glm::vec3(1.0f, 0.0f, 1.0f);
	v22->pos_ = glm::vec3(0.5f, 0.5f, 0.5f);

	v23->color_ = glm::vec3(1.0f, 0.0f, 1.0f);
	v23->pos_ = glm::vec3(0.5f, -0.5f, 0.5f);

	v24->color_ = glm::vec3(1.0f, 0.0f, 1.0f);
	v24->pos_ = glm::vec3(0.5f, -0.5f, -0.5f);

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

void GeometryManager::CreateCylinder(std::string name) {
	//Verticies taken from here:
	//https://gist.github.com/nikAizuddin/5ea402e9073f1ef76ba6

	Geometry* geometry = new Geometry(name);

	float x = 0.0;
	float y = 0.0;
	float angle = 0.0;
	float angle_stepsize = 3.14159265 * 0.5;
	float radius = 0.5;
	float height = 1;

	float pi = 3.14159;

	//create the sides
	geometry->AddSegment(GL_QUAD_STRIP);
	while (angle < 2 * pi) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		
		for (int i = 1; i > -2; i-=2) {
			Vertex* v = new Vertex();
			v->pos_ = glm::vec3(x, i*height / 2,y);
			v->color_ = glm::vec3(((float)i)/2+1, 0.5, 0.5);
			geometry->AddVertex(v);
		}
		angle = angle + angle_stepsize;
	}

	for (int i = 1; i > -2; i -= 2) {
		Vertex* v = new Vertex();
		v->pos_ = glm::vec3(radius, i*height / 2,0.0);
		v->color_ = glm::vec3(1, 0, 0);
		geometry->AddVertex(v);
	}
	
	//create the top and bottom
	for (int i = 0; i < 2; i++) {
		geometry->AddSegment(GL_POLYGON);
		angle = 0.0;

		while (angle < 2 * pi) {
			x = radius * cos(angle);
			y = radius * sin(angle);

			Vertex* v = new Vertex();
			v->pos_ = glm::vec3(x, height / 2 * (i * 2 - 1), y);
			v->color_ = glm::vec3((float)i/2, (float)angle / 2*pi, 0.5);
			geometry->AddVertex(v, i+1);

			angle = angle + angle_stepsize;
		}

		Vertex* v = new Vertex();
		v->pos_ = glm::vec3(radius, height / 2 * (i * 2 - 1),0.0);
		v->color_ = glm::vec3(1, 1, 1);
		geometry->AddVertex(v, i + 1);
	}

	geometryList_.push_back(geometry);
}

void GeometryManager::CreateSphere(std::string name) {
	//Verticies taken from here:
	//https://stackoverflow.com/questions/7687148/drawing-sphere-in-opengl-without-using-glusphere

	Geometry* geometry = new Geometry(name);
	float radius = 0.5;
	int lats = 15;
	int longs = 15;

	float pi = 3.14159;
	int i, j;
	for (i = 0; i <= lats; i++) {
		double lat0 = pi * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = pi * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		geometry->AddSegment(GL_QUAD_STRIP);
		for (j = 0; j <= longs; j++) {
			double lng = 2 * pi * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			int ci = rand() % 3;

			glm::vec3 c1 = glm::vec3(1.0f, 0.0f, 0.0f);
			glm::vec3 c2 = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::vec3 c3 = glm::vec3(0.0f, 0.0f, 1.0f);
			
			glm::vec3 color = (ci == 0) ? c1 : (ci == 1) ? c2 : c3;//glm::vec3(1, (float)i / (float)lats, (float)j / (float)longs);

			Vertex* v1 = new Vertex();
			Vertex* v2 = new Vertex();

			v1->pos_ = glm::vec3(radius * x * zr0, radius * y * zr0, radius * z0);

			v2->pos_ = glm::vec3(radius * x * zr1, radius * y * zr1, radius * z1);
			
			v1->color_ = color;
			v2->color_ = color;
			geometry->AddVertex(v1, i);
			geometry->AddVertex(v2, i);
		}
	}

	geometryList_.push_back(geometry);
}

void GeometryManager::CreateTriangle(std::string name) {

	//define our geometry and set its type for rendering
	Geometry* geometry = new Geometry(name);
	geometry->AddSegment(GL_TRIANGLES);
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
	Geometry* geometry = new Geometry(name);
	geometry->AddSegment(GL_POLYGON);
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