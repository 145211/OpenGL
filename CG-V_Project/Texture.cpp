#include "Texture.h"

Texture::Texture(GLuint type, GLint textureUnit) : textureUnit(textureUnit), type(type)
{

}

Texture::Texture(const char* fileName, GLenum type, GLint texture_unit) : textureUnit(texture_unit), type(type) {
	//if (this->id) {
	//	glDeleteTextures(1, &this->id);
	//}
	
	loadTexture(fileName);
	//std::vector<unsigned char> image;
	//unsigned error = lodepng::decode(image, this->width, this->height, fileName);

	//glGenTextures(1, &this->id);
	//glBindTexture(type, this->id);

	//glTexImage2D(type, 0, 4, this->width, this->height, 0,
	//	GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	//glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glActiveTexture(0);
	//glBindTexture(type, 0);
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

void Texture::loadCubemap(std::vector<std::string> faces)
{
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		std::vector<unsigned char> image;
		unsigned error = lodepng::decode(image, this->width, this->height, faces[i].c_str());
		if (image.size())
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
			image.clear();
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			image.clear();
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glActiveTexture(0);
	glBindTexture(type, 0);
}

void Texture::setTextureUnit(GLint tu)
{
	textureUnit = tu;
}

void Texture::setTextureID(GLuint texID)
{
	this->id = texID;
}

bool Texture::getType()
{
	return type;
}

void Texture::setType(unsigned int tp)
{
	type = tp;
}

void Texture::bindTexture() {
	glActiveTexture(GL_TEXTURE0 + this->textureUnit);
	glBindTexture(this->type, this->id);
}

void Texture::unbindTexture() {
	glActiveTexture(0);
	glBindTexture(this->type, 0);
}

GLint Texture::getTextureUnit() const
{
	return this->textureUnit;
}
