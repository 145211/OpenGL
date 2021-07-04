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
out vec4 iL[2];  
out vec4 iV; //wektor do obserwatora
out vec2 itexCoord;

void main(void) {
    vec4 lp[2] = vec4[2]( vec4(4.3, 2, -0.8, 1), vec4(19.3, 3, -0.8, 1) );

    itexCoord = texCoord;

    iL[0] = V * lp[0] - V * M * vertex;
    iL[1] = V * lp[1] - V * M * vertex;

    iN = V * M * normal; 

    iV = vec4(0,0,0,1) - V * M * vertex; 

    gl_Position = P * V * M * vertex;
}
