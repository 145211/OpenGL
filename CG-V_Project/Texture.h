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
	bool cubemap = false;

public:
	Texture(GLuint type, GLint texture_unit);
	Texture(const char* fileName, GLenum type, GLint texture_unit);
	~Texture();

	void loadTexture(const char* fileName);

	void loadCubemap(std::vector<std::string> faces);
	
	void setTextureUnit(GLint tu);

	bool getType();
	void setType(unsigned int tp);

	void setTextureID(GLuint texID);
	GLuint getTextureID() const { return this->id; }

	void bindTexture();
	void unbindTexture();
	inline GLint getTextureUnit() const;
};