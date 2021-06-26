#version 330

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};

uniform Material material;

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;

uniform vec3 playerPos;
uniform vec4 ambientLight;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 ic; 
in vec4 iN; 
in vec4 iL; 
in vec2 itexCoord;
in vec4 fragPos;


void main(void) {
	vec4 n = normalize(iN);
	vec4 l = normalize(iL);

	float nl = clamp(dot(n, l), 0, 1);
	
	pixelColor = /*texture(textureMap0, itexCoord)*/ ic * vec4(nl) * ambientLight;
}
