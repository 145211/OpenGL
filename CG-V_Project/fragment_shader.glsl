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
    vec4 pos;
    
    vec4 lightColor;

    float constant;
    float linear;
    float quadratic;   
};  

#define noPointLights 5
uniform PointLight pointLights[noPointLights];

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;

uniform float cutoff;
uniform float outerCutoff;

uniform vec3 playerPos;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 iN; 
in vec4 iL[noPointLights]; 
in vec4 iV; 
in vec2 itexCoord;

in vec4 camera;

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

vec4 point(PointLight PL, vec4 norm, vec4 view, vec4 il, vec4 DT, vec4 ST, float mShiny)
{
	//Attenuation
	float dist = length(il);
	float atten = 1 / (PL.constant + PL.linear * dist + PL.quadratic * (dist*dist));

	//Rodzaje swiatla
	vec4 mDiffuse = atten * vec4(0.5) * DT;
	vec4 mSpecular = atten * vec4(0.25) * (1 - ST);
           
	//diffuse
	vec4 lp = normalize(il);
	float diff = clamp(dot(norm, lp), 0, 1);
	vec4 diffuse = (diff * mDiffuse);
    
	//Specular
    vec4 reflectDir = reflect(-lp, norm);
	float spec = pow(clamp(dot(reflectDir, view), 0.5, 1), mShiny);
	vec4 specular = (spec * mSpecular);

    return PL.lightColor * (specular + diffuse);
}

vec4 torch(vec4 res, float cutoff, float outerCutoff, vec4 frag, vec4 cam)
{
    float theta = dot(cam, -frag);

    float epsilon   = cutoff - outerCutoff;
    float intensity = clamp((theta - outerCutoff) / epsilon, 0.0, 1.0); 

    if(theta > cutoff) 
    {       
        return res;
    }
    else
        return vec4(0);
}


void main(void) {
	//Powierzchnia
	vec4 diffTex = texture(textureMap0, itexCoord);
	vec4 specTex = texture(textureMap1, itexCoord);
    float mShiny = 100;

    //kierunek patrzenia
    vec4 viewDir = normalize(iV);
    vec4 normal = normalize(iN);
   
    //Swiatlo
    vec4 result = vec4(0);

    for (int i = 0; i < noPointLights - 1; i++)
        result += point(pointLights[i], normal, viewDir, iL[i], diffTex, specTex,  mShiny);

    result += CalcDirLight(dirLight, normal, viewDir, diffTex, specTex, mShiny);

    vec4 flashlight = point(pointLights[noPointLights - 1], normal, viewDir, iL[noPointLights - 1], diffTex, specTex,  mShiny);

    result = torch(flashlight, cutoff, 0, viewDir, camera);

	pixelColor = vec4(diffTex.rgb * result.rgb, diffTex.a);
}
