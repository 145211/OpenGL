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

uniform vec3 playerPos;


//Zmienne interpolowane
out vec4 ic;
out vec4 iN;
out vec4 iL;
out vec2 itexCoord;
out vec4 fragPos;

void main(void) {
    //vec4 lp = vec4(0, 0, -6, 1);
    vec4 lp = vec4(playerPos, 1);

    itexCoord = texCoord;
    ic = color;
    iL = normalize(V * lp - V * M * vertex);
    iN = normalize(V * M * normal);

    fragPos = normalize(M * normal);

    gl_Position = P * V * M * vertex;
}
