#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::dot;

class Collisions {
private:
	vec2 A;
	vec2 B;
	vec2 C;
	vec2 D;
public:
	Collisions(vec2 A, vec2 B, vec2 C, vec2 D);

	~Collisions();
	
	bool inRect(vec2 M);
};

vec2 Collide(vec2 pos, vec2 move, Collisions objs[]);

static Collisions objs[2] = {
	Collisions(vec2(9,9), vec2(7.5, 9), vec2(7.5, 10.25), vec2(9, 10.25)),
	Collisions(vec2(14.5, 10.25), vec2(15.75, 10.25), vec2(15.75, 9), vec2(14.5, 9))
};

#endif