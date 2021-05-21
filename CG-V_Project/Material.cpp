#include "Material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint diffuseTex, GLint specularTex) : 
	ambient(ambient), diffuse(diffuse), specular(specular), diffuseTex(diffuseTex), specularTex(specularTex) 
{
	
}

void Material::sendToShader(ShaderProgram* sp)
{
	glUniform3fv(sp->u("material.ambient"), 1, glm::value_ptr(this->ambient));
	glUniform3fv(sp->u("material.diffuse"), 1, glm::value_ptr(this->diffuse));
	glUniform3fv(sp->u("material.specular"), 1, glm::value_ptr(this->specular));
	glUniform1i(sp->u("material.diffuseTex"), this->diffuseTex);
	glUniform1i(sp->u("material.specularTex"), this->specularTex);
	
}
