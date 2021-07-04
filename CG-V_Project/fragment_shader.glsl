#version 330

struct DirLight {
    vec4 direction;
  	   
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};  

uniform DirLight dirLight;

struct PointLight {    
    vec4 lightColor;

    float constant;
    float linear;
    float quadratic;  
};  

#define NR_POINT_LIGHTS 2  
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;

uniform vec3 playerPos;
uniform vec4 ambientLight;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 iN; 
in vec4 iL[2]; 
in vec4 iV; 
in vec2 itexCoord;

vec4 CalcDirLight(DirLight light, vec4 normal, vec4 viewDir, float shine, vec4 DT, vec4 ST)
{
    vec4 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec4 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shine);
    // combine results
    vec4 ambient  = light.ambient  * DT;
    vec4 diffuse  = light.diffuse  * diff * DT;
    vec4 specular = light.specular * spec * ST;
    return (ambient + diffuse + specular);
}  

vec4 point(PointLight PL, vec4 il, vec4 DT, vec4 ST, float mShiny)
{
	//Attenuation
	float dist = length(il);
	float atten = 1 / (PL.constant + PL.linear * dist + PL.quadratic * (dist*dist));

	//Rodzaje swiatla
	vec4 mAmbient = DT;
	vec4 mDiffuse = atten * vec4(0.5) * DT;
	vec4 mSpecular = atten * vec4(0.5) * ST;
       
    //ambient
	vec4 ambient = ambientLight * mAmbient;
    
	//diffuse
	vec4 norm = normalize(iN);
	vec4 lp = normalize(il);
	float diff = clamp(dot(norm, lp), 0, 1);
	vec4 diffuse = PL.lightColor * (diff * mDiffuse);
    
	//Specular
    vec4 viewDir = normalize(iV);
    vec4 reflectDir = reflect(-lp, norm);
	float spec = pow(clamp(dot(reflectDir, viewDir), 0.5, 1), mShiny);
	vec4 specular = PL.lightColor * (spec * mSpecular);

    return (ambient + diffuse + specular);
}

void main(void) {
	//Powierzchnia
	vec4 diffTex = texture(textureMap0, itexCoord);
	vec4 specTex = texture(textureMap1, itexCoord);
    float mShiny = 25;

    //kierunek patrzenia
    vec4 viewDir = normalize(iV);
   
    //Swiatlo
    vec4 result = vec4(0);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += point(pointLights[i], iL[i], diffTex, specTex, mShiny);

	pixelColor = vec4(diffTex.rgb * result.rgb, diffTex.a);
}
