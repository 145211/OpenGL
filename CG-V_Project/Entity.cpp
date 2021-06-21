#include "Entity.h"

glm::vec3* Entity::playerPos;

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

	//glGenBuffers(1, &this->EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * model.indicesSize(), model.indicesData(), GL_STATIC_DRAW);

	// Set pointers to attributes
	// Position
	glVertexAttribPointer(this->sp->a("vertex"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	// Color
	glVertexAttribPointer(this->sp->a("color"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	// Normal
	glVertexAttribPointer(this->sp->a("normal"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	// Texcoord
	glVertexAttribPointer(this->sp->a("texCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));

	// Disable buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Entity::Entity(Texture* texture, Model& model, ShaderProgram* sp) : texture(texture), sp(sp)
{
	this->model = model;
	initBuffers();
}

Entity::~Entity()
{
	// Delete buffers
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	// The rest should be automatically destructed
}

void Entity::setTexture(Texture* texture)
{
	this->texture = texture;
	initBuffers(); // Would work better if used flags
}

void Entity::setModel(Model& model)
{
	this->model = model;
	initBuffers(); // Would work better if used flags
}

void Entity::setShader(ShaderProgram* sp)
{
	this->sp = sp;
}

void Entity::drawEntity(glm::mat4 P, glm::mat4 V)
{
	// Enable shader program
	this->sp->use();

	// Bind buffers
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

	// Bind texture
	this->texture->bindTexture();

	// Prepare model matrix
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, this->model.getPosition());
	M = glm::rotate(M, this->model.getRotation().x, glm::vec3(1.f, 0.f, 0.f));
	M = glm::rotate(M, this->model.getRotation().y, glm::vec3(0.f, 1.f, 0.f));
	M = glm::rotate(M, this->model.getRotation().z, glm::vec3(0.f, 0.f, 1.f));
	M = glm::translate(M, this->model.getPosition() - this->model.getOrigin());
	M = glm::scale(M, this->model.getScaling());

	// Send uniforms
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glUniform3fv(sp->u("playerPos"), 1, glm::value_ptr(*playerPos));

	// Enable vertex attributes
	glEnableVertexAttribArray(this->sp->a("vertex"));
	glEnableVertexAttribArray(this->sp->a("color"));
	glEnableVertexAttribArray(this->sp->a("texCoord"));
	glEnableVertexAttribArray(this->sp->a("normal"));

	// Draw arrays
	glDrawArrays(GL_TRIANGLES, 0, this->model.arraySize());
	//glDrawElements(GL_TRIANGLES, model.indicesSize(), GL_UNSIGNED_INT, 0);

	// Cleanup
	this->texture->unbindTexture();
	glDisableVertexAttribArray(this->sp->a("vertex"));
	glDisableVertexAttribArray(this->sp->a("color"));
	glDisableVertexAttribArray(this->sp->a("texCoord"));
	glDisableVertexAttribArray(this->sp->a("normal"));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// Disable shader program
	this->sp->unuse();
}

Texture* Entity::accessTexturePointer()
{
	return this->texture;
}

Model& Entity::accessModel()
{
	return this->model;
}
