#include "Texture.h"

Texture::Texture(GLint textureUnit, unsigned int type) : textureUnit(textureUnit), type(type)
{

}

Texture::Texture(const char* fileName, GLenum type, GLint texture_unit) : textureUnit(texture_unit), type(type) {
	//if (this->id) {
	//	glDeleteTextures(1, &this->id);
	//}
	
	std::vector<unsigned char> image;
	unsigned error = lodepng::decode(image, this->width, this->height, fileName);

	glGenTextures(1, &this->id);
	glBindTexture(type, this->id);

	glTexImage2D(type, 0, 4, this->width, this->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(0);
	glBindTexture(type, 0);
	//image.clear();
}

Texture::~Texture() { glDeleteTextures(1, &this->id); }

void Texture::loadTexture(const char* fileName)
{
	std::vector<unsigned char> image;
	unsigned error = lodepng::decode(image, this->width, this->height, fileName);

	glGenTextures(1, &this->id);
	glBindTexture(type, this->id);

	glTexImage2D(type, 0, 4, this->width, this->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(0);
	glBindTexture(type, 0);
}

void Texture::bind() {
	glActiveTexture(GL_TEXTURE0 + this->textureUnit);
	glBindTexture(this->type, this->id);
}

void Texture::unbind() {
	glActiveTexture(0);
	glBindTexture(this->type, 0);
}

GLint Texture::getTextureUnit() const
{
	return this->textureUnit;
}
