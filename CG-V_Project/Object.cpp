#include "Object.h"

void Object::updateModelMatrix()
{
	this->modelMatrix = glm::mat4(1.0f);
	this->modelMatrix = glm::translate(this->modelMatrix, this->position);
	this->modelMatrix = glm::rotate(this->modelMatrix, this->rotation.x, glm::vec3(1.f, 0.f, 0.f));
	this->modelMatrix = glm::rotate(this->modelMatrix, this->rotation.y, glm::vec3(0.f, 1.f, 0.f));
	this->modelMatrix = glm::rotate(this->modelMatrix, this->rotation.z, glm::vec3(0.f, 0.f, 1.f));
	this->modelMatrix = glm::translate(this->modelMatrix, this->position - this->origin);
	this->modelMatrix = glm::scale(this->modelMatrix, this->scaling);
}

void Object::sendModelMatrix(ShaderProgram* sp)
{
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(this->modelMatrix));
}

void Object::initVAO()
{
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);

	//Position
	glVertexAttribPointer(this->spPtr->a("vertex"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(this->spPtr->a("vertex"));
	//Color
	glVertexAttribPointer(this->spPtr->a("color"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(this->spPtr->a("color"));
	//Texcoord
	glVertexAttribPointer(this->spPtr->a("texCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(this->spPtr->a("texCoord"));
	//Normal
	glVertexAttribPointer(this->spPtr->a("normal"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(this->spPtr->a("normal"));
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	//glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

Object::Object(ShaderProgram* spPtr, Vertex* vertices, GLuint vertexCount, glm::vec3 position, glm::vec3 rotation, glm::vec3 scaling) : spPtr(spPtr)
{
	this->position = position;
	this->rotation = rotation;
	this->scaling = scaling;
	this->origin = glm::vec3(0.0f);

	this->vertexCount = vertexCount;
	this->vertexArray = new Vertex[vertexCount];
	for (size_t i = 0; i < this->vertexCount; i++)
	{
		this->vertexArray[i] = vertices[i];
	}
	//std::copy(vertices, vertices + vertexCount, this->vertexArray);

	this->initVAO();
	this->updateModelMatrix();
}

Object::~Object()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);

	delete[] this->vertexArray;
}

void Object::activateShader()
{
	this->spPtr->use();
}

void Object::setShader(ShaderProgram* sp)
{
	this->spPtr = sp;
}

void Object::setPosition(glm::vec3 position)
{
	this->position = position;
}

void Object::setRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}

void Object::setScaling(glm::vec3 scaling)
{
	this->scaling = scaling;
}

void Object::move(glm::vec3 vector)
{
	this->position += position;
}

void Object::rotate(glm::vec3 rotation)
{
	this->rotation += rotation;
}

void Object::scale(glm::vec3 rotation)
{
	this->scaling += scaling;
}

void Object::render()
{
	this->updateModelMatrix();
	this->sendModelMatrix(spPtr);

	//sp->use();

	glBindVertexArray(this->VAO);

	glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	resetModelMatrix();
}

void Object::resetModelMatrix()
{
	this->modelMatrix = glm::mat4(1.0f);
}

GLuint Object::getVertexCount() const
{
	return vertexCount;
}
