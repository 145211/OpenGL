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
out vec4 iL; //wektor do światla
out vec4 iV; //wektor do obserwatora
out vec2 itexCoord;

void main(void) {
	vec4 lp = vec4(12, 5, 5, 1);

    //vec4 lp = normalize(-vec4( -0.2f, -1.0f, -0.3f, 0));

    itexCoord = texCoord;

    //iL = V * lp;

    iL = V * lp - V * M * vertex;

    iN = V * M * normal; 

    iV = vec4(0,0,0,1) - V * M * vertex; 

    gl_Position = P * V * M * vertex;
}
