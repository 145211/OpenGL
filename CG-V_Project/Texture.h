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
	Texture(const char* fileName, GLenum type, GLint texture_unit);
	~Texture();

	GLuint getID() const { return this->id; }
	void bind();
	void unbind();
	inline GLint getTextureUnit() const;
};