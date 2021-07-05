#version 330

struct PointLight {    
    vec4 pos;
    
    vec4 lightColor;

    float constant;
    float linear;
    float quadratic;  
};  

#define noPointLights 5
uniform PointLight pointLights[noPointLights];

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 color; //kolor związany z wierzchołkiem
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord;

uniform vec3 playerPos; //pozycja gracza

//Zmienne interpolowane
out vec4 iN;
out vec4 iL[noPointLights];  
out vec4 iV; //wektor do obserwatora
out vec2 itexCoord;

void main(void) {
    // pozycje swiatel
    for (int i = 0; i < noPointLights - 1; i++)
    {
        iL[i] = V * pointLights[i].pos - V * M * vertex;
    }

    iL[noPointLights - 1] = V * vec4(playerPos, 1) - V * M * vertex;

    itexCoord = texCoord;

    iN = V * M * normal;

    iV = vec4(0,0,0,1) - V * M * vertex; 

    gl_Position = P * V * M * vertex;
}
