#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 color; //kolor związany z wierzchołkiem
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec4 texCoord;


//Zmienne interpolowane
out vec4 ic;
out vec4 iN;
out vec4 iL;

void main(void) {
    vec4 lp = vec4(0, 0, -6, 1);


    ic = color;
    iL = normalize(V * lp - V * M * vertex);
    iN = normalize(V * M * normal);

    gl_Position = P * V * M * vertex;
}
