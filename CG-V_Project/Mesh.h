#pragma once


#include <iostream>
#include <vector>
#include "Vertex.h"
#include "shaderprogram.h"
#include "Texture.h"
#include "Material.h"


class Mesh {
private:
	Vertex* vertexArray;
	unsigned vertexCount;
	GLuint* indexArray;
	unsigned indexCount;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 ModelMatrix;

	void initVAO();
	void updateModelMatrix();
	void updateUniforms(ShaderProgram* sp);

public:
	Mesh(Vertex* vertexArray, const unsigned& vertexCount, GLuint* indexArray, const unsigned& indexCount, glm::vec3 position = glm::vec3(0.f), glm::vec3 origin = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f));
	Mesh(const Mesh& obj);
	~Mesh();

	void setPosition(const glm::vec3& position);
	void Move(const glm::vec3& vector);
	void setRotation(const glm::vec3& rotation);
	void Rotate(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);
	void Scale(const glm::vec3& scale);

	void update();
	void render(ShaderProgram* sp);
};