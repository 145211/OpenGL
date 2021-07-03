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
	vec4 kd = texture(textureMap0, itexCoord);

	float constant = 1;
	float linear = 0.09;
	float quadratic = 0.032;

	float dist = length(iL);
	float atten = 1 / (constant + linear * dist + quadratic * (dist*dist));

	vec4 mAmbient = kd;
	vec4 mDiffuse = atten * vec4(0.5) * kd;
	vec4 mSpecular = atten * vec4(0.5) * kd;
	float mShiny = 25;

	vec4 lightColor = vec4(1, 1, 1, 1);

	vec4 ambient = ambientLight * mAmbient;

	vec4 norm = normalize(iN);
	vec4 lightDir = normalize(iL);
	float diff = clamp(dot(norm, lightDir), 0, 1);
	vec4 diffuse = lightColor * (diff * mDiffuse);

    vec4 viewDir = normalize(iV);
    vec4 reflectDir = reflect(-lightDir, norm);
	float spec = pow(clamp(dot(reflectDir, viewDir), 0.5, 1), mShiny);
	vec4 specular = lightColor * (spec * mSpecular);

	//Swiatlo
	vec4 result = ambient + diffuse + specular;

	pixelColor = vec4(kd.rgb * result.rgb, kd.a);
}
