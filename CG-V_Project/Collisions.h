#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::dot;
using glm::distance;
using std::vector;

class Collisions {
public:
	vec2 A;
	vec2 B;
	vec2 C;
	vec2 D;

	Collisions(vec2 A, vec2 B, vec2 C, vec2 D);

	~Collisions();

	bool inRect(vec2 M);
};

vec2 Collide(vec2 pos, vec2 move, vector<Collisions> objs);

vector<Collisions> squareArray(vec2 start, vec2 end, float ammount, float side, vector<Collisions> objs);

vector<Collisions> collisionInit(vector<Collisions> objs);

#endif