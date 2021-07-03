#version 330
in vec4 vertex;

out vec3 TexCoords;

uniform mat4 P;
uniform mat4 V;

void main()
{
    TexCoords = vec3(vertex.xyz);
    gl_Position = P * V * vertex;
}  