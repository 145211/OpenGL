#include "Entity.h"

void Entity::initBuffers()
{
	// Generate and bind VAO buffer
	if (VAO) glDeleteVertexArrays(1, &this->VAO);
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	// Generate and bind VBO buffer
	if (VBO) glDeleteBuffers(1, &this->VBO);
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	// Send data to the buffer
	glBufferData(GL_ARRAY_BUFFER, this->model.arraySize() * sizeof(Vertex), this->model.vertexData(), GL_STATIC_DRAW);

	// Set pointers to attributes
	// Position
	glVertexAttribPointer(this->sp->a("vertex"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	// Color
	glVertexAttribPointer(this->sp->a("color"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	// Normal
	glVertexAttribPointer(this->sp->a("normal"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	// Texcoord
	glVertexAttribPointer(this->sp->a("texCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));

	// Disable current attribute buffer
	glBindVertexArray(0);
}

void Entity::setTexture(Texture* texture)
{
	this->texture = texture;
}

void Entity::setModel(Model& model)
{
	this->model = model;
}

void Entity::setShader(ShaderProgram* sp)
{
	this->sp = sp;
}

Texture* Entity::accessTexturePointer()
{
	return this->texture;
}

Model& Entity::accessModel()
{
	return this->model;
}
