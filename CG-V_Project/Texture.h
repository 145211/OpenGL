#pragma once

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "lodepng.h"

class Texture {
private:
	GLuint id;
	unsigned int width;
	unsigned int height;
	unsigned int type;
	GLint textureUnit;

public:
	Texture(GLint texture_unit, unsigned int type);
	Texture(const char* fileName, GLenum type, GLint texture_unit);
	~Texture();

	void loadTexture(const char* fileName);
	//void setTextureUnit();


	GLuint getTextureID() const { return this->id; }
	void bindTexture();
	void unbindTexture();
	inline GLint getTextureUnit() const;
};