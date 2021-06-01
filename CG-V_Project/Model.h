#ifndef MODEL_H
#define MODEL_H

#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "GL/glew.h"
#include <fstream>
#include <sstream>


class Model {
private:
	std::vector<Vertex> vertexArray;
	
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scaling;
	glm::vec3 origin;
public:
	Model();
	Model(const char* fileName);

	void loadModel(const char* fileName);

	glm::vec4* getVerticesArray();
	glm::vec2* getTexCoordsArray();
	glm::vec4* getNormalsArray();
	//glm::vec4* getFacesArray();

	size_t size();
	Vertex* data();

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScaling(glm::vec3 scaling);
};

#endif