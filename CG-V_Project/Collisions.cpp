#include "Collisions.h"

Collisions::Collisions(vec2 A, vec2 B, vec2 C, vec2 D) {
	this->A = A;
	this->B = B;
	this->C = C;
	this->D = D;
};

Collisions::~Collisions() {};

vec2 vectr(vec2 A, vec2 B) {
	return vec2(B.x - A.x, B.y - A.y);
};

bool Collisions::inRect(vec2 M) {
	vec2 AB = vectr(A, B);
	vec2 BC = vectr(B, C);

	float ABAM = dot(AB, vectr(A, M));
	float BCBM = dot(BC, vectr(B, M));

	if ((0 <= ABAM) && (ABAM <= dot(AB, AB)))
	{
		if ((0 <= BCBM) && (BCBM <= dot(BC, BC)))
		{
			return 1;
		}
	}
	return 0; 
};

vec2 Collide(vec2 pos, vec2 move, Collisions objs[]) {
	vec2 destination = pos + move;
	for (int i = 0; i < sizeof(objs)/2; i++)
	{
		//printf("%f, %f, inRect: %i, size: %i\n", destination.x, destination.y, objs[i].inRect(destination), sizeof(objs));
		if (objs[i].inRect(destination) == 1)
		{
			//printf("Collision!\n");
			return vec2(0, 0);
		}
	}
	return move;
};


