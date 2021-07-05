#version 330

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
out vec4 iL[3];  
out vec4 iV; //wektor do obserwatora
out vec2 itexCoord;

#define NR_POINT_LIGHTS 3  

void main(void) {
    // pozycje swiatel
    vec4 lp[NR_POINT_LIGHTS] = vec4[NR_POINT_LIGHTS]( 
    vec4(4.3, 2, -0.8, 1), 
    vec4(19.3, 3, -0.8, 1), 
    vec4(10, 1.5, -50, 1) 
    );

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        iL[i] = V * lp[i] - V * M * vertex;
    }

    itexCoord = texCoord;

    iN = V * M * normal;

    iV = vec4(0,0,0,1) - V * M * vertex; 

    gl_Position = P * V * M * vertex;
}
