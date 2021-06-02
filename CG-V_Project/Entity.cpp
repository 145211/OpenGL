#include "Entity.h"

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
