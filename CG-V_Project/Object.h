#pragma once

#include "Vertex.h"
#include "shaderprogram.h"

#include <GL/glew.h>
#include <algorithm>
#include <iterator>

class Object {
private:
	Vertex* vertexArray;
	GLuint vertexCount;
	ShaderProgram* spPtr;

	GLuint VAO;
	GLuint VBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scaling;
	glm::vec3 origin;

	glm::mat4 modelMatrix;

	void updateModelMatrix();
	void sendModelMatrix(ShaderProgram* sp);
	void initVAO();
public:
	Object(ShaderProgram* spPtr, Vertex* vertices, GLuint vertexCount, glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scaling = glm::vec3(1.0f));
	Object(const Object& object);
	~Object();

	void activateShader();
	void setShader(ShaderProgram* sp);

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScaling(glm::vec3 scaling);

	void move(glm::vec3 vector);
	void rotate(glm::vec3 rotation);
	void scale(glm::vec3 rotation);

	void render();
	void resetModelMatrix();
	
	GLuint getVertexCount() const;

	void enableVertex();
	void enableColor();
	void enableNormal();

	void disableAll();
	void activateObject();
};