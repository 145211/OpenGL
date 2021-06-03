#ifndef MODEL_H
#define MODEL_H

#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "GL/glew.h"
#include <fstream>
#include <sstream>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model {
private:
	std::vector<Vertex> vertexArray;
	
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 rotation = glm::vec3(0.f);
	glm::vec3 scaling = glm::vec3(1.f);
	glm::vec3 origin = glm::vec3(0.f);
public:
	Model();
	Model(const char* fileName);
	Model(const Model& m);

	void loadModel(const char* fileName);
	void assimpLoadModel(std::string fileName);

	glm::vec4* getVerticesArray();
	glm::vec2* getTexCoordsArray();
	glm::vec4* getNormalsArray();
	//glm::vec4* getFacesArray();

	size_t arraySize();
	Vertex* vertexData();

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScaling(glm::vec3 scaling);

	glm::vec3 getPosition() const;
	glm::vec3 getRotation() const;
	glm::vec3 getScaling() const;
	glm::vec3 getOrigin() const;
};

#endif