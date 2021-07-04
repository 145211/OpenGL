#include "Material.h"

Material::Material(Texture mainTex, Texture diffuseTex, Texture specularTex) :
	mainTex(mainTex), diffuseTex(diffuseTex), specularTex(specularTex)
{

}

void Material::sendToShader(ShaderProgram* sp)
{
	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mainTex.getTextureID());
	//diffuseTex.bindTexture();
	//specularTex.bindTexture();
}
