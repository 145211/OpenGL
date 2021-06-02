#pragma once

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "Object.h"
#include "Texture.h"
#include "Model.h"
#include "Vertex.h"
#include "shaderprogram.h"

#include "Material.h"

class Entity {
private:
	Texture* texture;
	Model model;

	ShaderProgram* sp;

	GLuint VBO, VAO;
	void deleteModel();	// model powinien jeszcze być usuwany przy setowaniu nowego modelu (co i jak jeszcze nie wiem)
	void initBuffers(); // przy inicjowaniu modelu. Tekstura już powinna do tego czasu
						// posiadać zarezerwowaną i zorganizowaną pamięć
						// initBuffers powinno jeszcze testować, czy VBO oraz VAO już są utworzone
						// i usuwać jeżeli są już wykorzystywane przed ponowną alokacją.
public:
	Entity();
	Entity(Texture* texture, Model& model);

	void setTexture(Texture* texture);
	void setModel(Model& model);
	void setShader(ShaderProgram* sp);

	void drawEntity(glm::mat4 P, glm::mat4 V); // Cała matematyka odnośnie zmiennych jednorodnych oraz opakowanie w przekazania do shadera powinny znajdywać się tutaj. Możliwe rozbicie na kilka pomniejszych funkcji (i jak już to pewnie prywatnych).
	
	Texture* accessTexturePointer();
	Model& accessModel();
};