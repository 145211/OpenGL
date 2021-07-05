#version 330

struct DirLight {
    vec4 ambientColor;
    float ambientPwr;

    float pwr;
    vec4 direction;
  	vec4 lightColor;
};  

uniform DirLight dirLight;

struct PointLight {    
    vec4 lightColor;

    float constant;
    float linear;
    float quadratic;  
};  

#define NR_POINT_LIGHTS 3  
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct Flashlight {    
    vec4 lightColor;

    float cutoff;
};  

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;

uniform vec3 playerPos;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 iN; 
in vec4 iL[NR_POINT_LIGHTS]; 
in vec4 iV; 
in vec2 itexCoord;

vec4 CalcDirLight(DirLight light, vec4 norm, vec4 viewDir, vec4 DT, vec4 ST, float shine)
{
    //ambient
    vec4 ambient = light.ambientColor * light.ambientPwr * DT;

    //diffuse
    vec4 dir = normalize(-light.direction);
    float diff = clamp(dot(norm, dir), 0, 1);
    vec4 diffuse = vec4(light.pwr) * diff * light.lightColor;
    
    //specular
    vec4 refDir = reflect(-light.direction, norm);
    float spec = pow(max(dot(viewDir, refDir), 0), shine);
    vec4 specular = vec4(light.pwr) * spec * light.lightColor;

    return (ambient + diffuse);
}  

vec4 point(PointLight PL, vec4 il, vec4 DT, vec4 ST, vec4 norm, float mShiny)
{
	//Attenuation
	float dist = length(il);
	float atten = 1 / (PL.constant + PL.linear * dist + PL.quadratic * (dist*dist));

	//Rodzaje swiatla
	vec4 mDiffuse = atten * vec4(0.5) * DT;
	vec4 mSpecular = atten * vec4(0.5) * ST;
           
	//diffuse
	vec4 lp = normalize(il);
	float diff = clamp(dot(norm, lp), 0, 1);
	vec4 diffuse = (diff * mDiffuse);
    
	//Specular
    vec4 viewDir = normalize(iV);
    vec4 reflectDir = reflect(-lp, norm);
	float spec = pow(clamp(dot(reflectDir, viewDir), 0.5, 1), mShiny);
	vec4 specular = (spec * mSpecular);

    return PL.lightColor * (diffuse + specular);
}

//vec4 spot(Flashlight FL, vec4 lightPos, vec4 lightDir, vec4 DT, vec4 ST, vec4 norm, float mShiny)
//{
//    float theta = dot(lightDir, normalize(-lightDir));
//
//
//    return vec4(0);
//}

void main(void) {
	//Powierzchnia
	vec4 diffTex = texture(textureMap0, itexCoord);
	vec4 specTex = texture(textureMap1, itexCoord);
    float mShiny = 25;

    //kierunek patrzenia
    vec4 viewDir = normalize(iV);
    vec4 normal = normalize(iN);
   
    //Swiatlo
    vec4 result = vec4(0);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += point(pointLights[i], iL[i], diffTex, specTex, normal, mShiny);

    result += CalcDirLight(dirLight, iN, viewDir, diffTex, specTex, mShiny);

	pixelColor = vec4(diffTex.rgb * result.rgb, diffTex.a);
}
