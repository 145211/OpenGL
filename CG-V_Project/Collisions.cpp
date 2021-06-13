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

//sprawdzenie z którym bokiem nastepuje kolizja
vector<vec2> intersection(Collisions obj, vec2 pos, vec2 des) {
	vector<vec2> sides = { obj.A, obj.B, obj.C, obj.D, obj.A };

	for (int i = 0; i < 4; i++)
	{
		float denom = (des[1] - pos[1]) * (sides[i + 1][0] - sides[i][0]) - (des[0] - pos[0]) * (sides[i + 1][1] - sides[i][1]);
		float numera = (des[0] - pos[0]) * (sides[i][1] - pos[1]) - (des[1] - pos[1]) * (sides[i][0] - pos[0]);
		float numerb = (sides[i + 1][0] - sides[i][0]) * (sides[i][1] - pos[1]) - (sides[i + 1][1] - sides[i][1]) * (sides[i][0] - pos[0]);

		if (denom == 0)
			continue;

		float mua = numera / denom;
		float mub = numerb / denom;

		if (mua < 0 || mua > 1 || mub < 0 || mub > 1)
			continue;

		return { sides[i], sides[i + 1] };
	}
};

//obliczanie kolizji
vec2 Collide(vec2 pos, vec2 move, vector<Collisions> objs) {
	vec2 des = pos + move;
	for (int i = 0; i < objs.size(); i++)
	{
		//printf("%f, %f, inRect: %i, size: %i\n", destination.x, destination.y, objs[i].inRect(destination), sizeof(objs));
		if (objs[i].inRect(des) == 1)
		{
			vector<vec2> wall = intersection(objs[i], pos, des);

			vec2 norm = normalize(-vec2(-(wall[1].y - wall[0].y), (wall[1].x - wall[0].x)));

			move = move - dot(norm, move) * norm;

			return move;
		}
	}
	return move;
};

//wspolrzedne rogow kwadratu
vector<vec2> corners(vec2 centre, float a) {
	float a2 = a / 2;
	return vector<vec2> {
		centre - vec2(a2, a2),
			centre + vec2(a2, -a2),
			centre + vec2(a2, a2),
			centre - vec2(a2, -a2)};
};

//obszary zabronione w linii w regularnych odstepach
vector<Collisions> squareArray(vec2 start, vec2 end, float ammount, float side, vector<Collisions> objs) {
	vec2 dist = (end - start) / (ammount - 1);

	vector<vec2> corns;

	for (float i = 0; i < (ammount); i++) {
		corns = corners(start + i * dist, side);
		objs.push_back(Collisions(corns[0], corns[1], corns[2], corns[3]));
	}
	return objs;
};

//procedura tworzaca wektor obiektow kolizji
vector<Collisions> collisionInit(vector<Collisions> objs) {
	objs = squareArray(vec2(23, 0), vec2(23, 46.5), 11, 2.75, objs);
	objs = squareArray(vec2(0, 0), vec2(0, 46.5), 11, 2.75, objs);

	objs = squareArray(vec2(15.75, 0), vec2(7.75, 0), 2, 2.75, objs);
	objs = squareArray(vec2(8.25, 39), vec2(8.25, 9.65), 4, 1.4, objs);
	objs = squareArray(vec2(15.15, 39), vec2(15.15, 9.65), 4, 1.4, objs);

	return objs;
};

