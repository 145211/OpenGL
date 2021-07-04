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

in vec4 iN; 
in vec4 iL; 
in vec4 iV; 
in vec2 itexCoord;

void main(void) {
	//Powierzchnia
	vec4 diffTex = texture(textureMap0, itexCoord);
	vec4 specTex = texture(textureMap1, itexCoord);

	//Attenuation
	float constant = 1;
	float linear = 0.09;
	float quadratic = 0.032;
	float dist = length(iL);
	float atten = 1 / (constant + linear * dist + quadratic * (dist*dist));

	//Rodzaje swiatla
	vec4 mAmbient = diffTex;
	vec4 mDiffuse = vec4(0.5) * diffTex;
	vec4 mSpecular = vec4(0.5) * specTex;
	float mShiny = 25;

	vec4 lightColor = vec4(1, 1, 1, 1);

	vec4 ambient = ambientLight * mAmbient;

	//diffuse
	vec4 norm = normalize(iN);
	vec4 lightDir = normalize(iL);
	float diff = clamp(dot(norm, lightDir), 0, 1);
	vec4 diffuse = lightColor * (diff * mDiffuse);

	//Specular
    vec4 viewDir = normalize(iV);
    vec4 reflectDir = reflect(-lightDir, norm);
	float spec = pow(clamp(dot(reflectDir, viewDir), 0.5, 1), mShiny);
	vec4 specular = lightColor * (spec * mSpecular);

	//Swiatlo
	vec4 result = ambient + diffuse + specular;

	pixelColor = vec4(diffTex.rgb * result.rgb, diffTex.a);
}
