#include "Mesh.h"

//void Mesh::initVertices(Vertex* vertexArray, const unsigned& vertexCount, GLuint* indexArray, const unsigned& indexCount)
//{
//	for (size_t i = 0; i < vertexCount; ++i) {
//		this->vertices.push_back(vertexArray[i]);
//	}	
//	
//	for (size_t i = 0; i < indexCount; ++i) {
//		this->indices.push_back(indexArray[i]);
//	}
//}

void Mesh::initVAO()
{
	//Create VAO
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//GEN VBO AND BIND AND SEND DATA
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);

	//GEN EBO AND BIND AND SEND DATA
	if (this->indexCount > 0)
	{
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexCount * sizeof(GLuint), this->indexArray, GL_STATIC_DRAW);
	}

	//SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	//Texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);
	//Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	//BIND VAO 0
	glBindVertexArray(0);
}

void Mesh::updateModelMatrix()
{
	this->ModelMatrix = glm::mat4(1.f);
	this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin);
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
	this->ModelMatrix = glm::translate(this->ModelMatrix, this->position - this->origin);
	this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
}

void Mesh::updateUniforms(ShaderProgram* sp)
{
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(this->ModelMatrix));
}

Mesh::Mesh(Vertex* vertexArray, const unsigned& vertexCount, GLuint* indexArray, const unsigned& indexCount, glm::vec3 position, glm::vec3 origin, glm::vec3 rotation, glm::vec3 scale)
{
	this->position = position;
	this->origin = origin;
	this->rotation = rotation;
	this->scale = scale;

	this->vertexCount = vertexCount;
	this->indexCount = indexCount;

	this->vertexArray = new Vertex[this->vertexCount];
	for (size_t i = 0; i < vertexCount; i++)
	{
		this->vertexArray[i] = vertexArray[i];
	}

	this->indexArray = new GLuint[this->indexCount];
	for (size_t i = 0; i < indexCount; i++)
	{
		this->indexArray[i] = indexArray[i];
	}

	this->initVAO();
	this->updateModelMatrix();
}

Mesh::Mesh(const Mesh& obj)
{
	this->position = obj.position;
	this->origin = obj.origin;
	this->rotation = obj.rotation;
	this->scale = obj.scale;

	this->vertexCount = obj.vertexCount;
	this->indexCount = obj.indexCount;

	this->vertexArray = new Vertex[this->vertexCount];
	for (size_t i = 0; i < this->vertexCount; i++)
	{
		this->vertexArray[i] = obj.vertexArray[i];
	}

	this->indexArray = new GLuint[this->indexCount];
	for (size_t i = 0; i < this->indexCount; i++)
	{
		this->indexArray[i] = obj.indexArray[i];
	}

	this->initVAO();
	this->updateModelMatrix();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);

	if (this->indexCount > 0)
	{
		glDeleteBuffers(1, &this->EBO);
	}

	delete[] this->vertexArray;
	delete[] this->indexArray;
}

void Mesh::setPosition(const glm::vec3& position)
{
	this->position = position;
}

void Mesh::Move(const glm::vec3& vector)
{
	this->position += vector;
}

void Mesh::setRotation(const glm::vec3& rotation)
{
	this->rotation = rotation;
}

void Mesh::Rotate(const glm::vec3& rotation)
{
	this->rotation += rotation;

}

void Mesh::setScale(const glm::vec3& scale)
{
	this->scale = scale;
}

void Mesh::Scale(const glm::vec3& scale)
{
	this->scale += scale;
}

void Mesh::render(ShaderProgram* sp)
{
	//Update uniforms
	this->updateModelMatrix();
	this->updateUniforms(sp);

	sp->use();

	//Bind VAO
	glBindVertexArray(this->VAO);

	//RENDER
	if (this->indexCount == 0)
		glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
	else
		glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);

	//Cleanup
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
