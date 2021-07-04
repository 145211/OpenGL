#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform float maxLength;
uniform float maxLayer;
uniform vec3 playerPos;

//Attributes
in vec4 vertex; //Vertex coordinates in model space
in vec4 color;
in vec4 normal; //Vertex normal in model space
in vec2 texCoord;

//Varying variables
out vec4 l;
out vec4 n;
out vec2 iTexCoord;

out float layer; // current layer

void main(void) {
    //vec4 lp = vec4(0, 0, -6, 1); //light position, world space
    vec4 lp = vec4(playerPos, 1);
    l = normalize(V * lp - V * M * vertex); //vector towards the light in eye space
    n = normalize(V * M * normal); //normal vector in eye space
    iTexCoord = texCoord;

    layer = gl_InstanceID; // numer warstwy, to numer kopii

    vec4 nv = vertex + (layer * maxLength / maxLayer) * normalize(normal);

    gl_Position = P * V * M * nv;
}