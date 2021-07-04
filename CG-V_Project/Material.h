#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "shaderprogram.h"
#include "texture.h"

class Material {
private:
	Texture mainTex;
	Texture diffuseTex;
	Texture specularTex;

public:
	Material(Texture mainTex, Texture diffuseTex, Texture specularTex);
	~Material();

	void sendToShader(ShaderProgram* sp);

};