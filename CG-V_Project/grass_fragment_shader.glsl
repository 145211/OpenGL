#version 330

out vec4 pixelColor; //Output variable. Almost final pixel color.

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;

//Varying variables
in vec4 n;
in vec4 l;
in vec2 iTexCoord;

in float layer;

void main(void) {
	//Normalized, interpolated vectors
	vec4 ml = normalize(l);
	vec4 mn = normalize(n);

	//Lighting model computation
	float nl = clamp(dot(mn, 0.6*ml), 0, 1);

	vec4 grassCol = texture(textureMap0, iTexCoord);
	vec4 grass = texture(textureMap1, iTexCoord);
	vec4 col = grassCol;

	if (layer != 0) col = vec4(grassCol.rgb, grass.a);

	if (grass.a == 0 && layer != 0) discard;

	pixelColor = col*vec4(nl, nl, nl, 1);
}