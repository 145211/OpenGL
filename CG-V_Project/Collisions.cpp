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

bool ccw(vec2 A, vec2 B, vec2 C) {
	return ((C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x));
}

//sprawdzenie z kt?rym bokiem nastepuje kolizja
vector<vec2> intersection(Collisions obj, vec2 pos, vec2 des) {
	vector<vec2> sides = { obj.A, obj.B, obj.C, obj.D, obj.A };

	for (int i = 0; i < 4; i++)
	{
		if (ccw(sides[i], pos, des) != ccw(sides[i + 1], pos, des) and ccw(sides[i], sides[i + 1], pos) != ccw(sides[i], sides[i + 1], des)) {
			//printf("side %i\n", i + 1);
			return { sides[i], sides[i + 1] };
		}
	}

	return { vec2(0, 0) };
};

//obliczanie kolizji
vec2 Collide(vec2 pos, vec2 move, vector<Collisions> objs) {
	vec2 des = pos + move;
	for (int i = 0; i < objs.size(); i++)
	{
		if (objs[i].inRect(des) == 1)
		{
			//printf("%f, %f, inRect: %i\n", des.x, des.y, objs[i].inRect(des));

			vector<vec2> wall = intersection(objs[i], pos, des);

			if (wall.size() == 1) {
				//return -move;
				continue;
			}

			vec2 norm = normalize(-vec2(-(wall[1].y - wall[0].y), (wall[1].x - wall[0].x)));

			//printf("norm: %f, %f\n", norm.x, norm.y);

			move = move - dot(norm, move) * norm;

			//printf("move: %f, %f\n", move.x, move.y);
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

vector<Collisions> square(vec2 pos, float side, vector<Collisions> objs) {
	vector<vec2> corns;

	corns = corners(pos, side);
	objs.push_back(Collisions(corns[0], corns[1], corns[2], corns[3]));

	return objs;
};

vector<Collisions> wall(vec2 A, vec2 B, vector<Collisions> objs) {
	objs.push_back(Collisions(A, B, A, B));

	return objs;
};

Collisions diagonal(vec2 A, vec2 B, vec2 C) {
	return Collisions(A, B, C, (A + C) - B);
}

//procedura tworzaca wektor obiektow kolizji
vector<Collisions> collisionInit(vector<Collisions> objs) {
	//big collumns
	objs = squareArray(vec2(23, 0), vec2(23, 46.5), 11, 2.75, objs);
	objs = squareArray(vec2(0, 0), vec2(0, 46.5), 11, 2.75, objs);
	objs = squareArray(vec2(15.75, 0), vec2(7.75, 0), 2, 2.75, objs);
	//small collumns
	objs = squareArray(vec2(8.25, 39), vec2(8.25, 9.65), 4, 1.5, objs);
	objs = squareArray(vec2(15.15, 39), vec2(15.15, 9.65), 4, 1.5, objs);

	//venus
	objs = square(vec2(12, 24), 4, objs);

	//donut
	objs = square(vec2(11.15, -52.9), 4, objs);

	//czary
	objs = square(vec2(4.3, -0.8), 2, objs);
	objs = square(vec2(19.3, -0.8), 2, objs);

	objs = square(vec2(4.3, 24.2), 2, objs);
	objs = square(vec2(18.5, 24.2), 2, objs);

	//vase
	objs = square(vec2(4.3, 19.5), 2, objs);
	objs = square(vec2(19.2, 19.5), 2, objs);

	// border
	objs = wall(vec2(44, -69), vec2(44, 69), objs); // L
	objs = wall(vec2(-44, 69), vec2(-44, -69), objs); // R
	objs = wall(vec2(45, 68), vec2(-45, 68), objs); // T
	objs = wall(vec2(45, -68), vec2(-45, -68), objs); // D


	//fence
	objs.push_back(Collisions(vec2(7.75, -43.5), vec2(6.25, -43.5), vec2(6.25, -5.2), vec2(7.75, -5.2)));
	objs.push_back(Collisions(vec2(16, -5.2), vec2(14.5, -5.2), vec2(14.5, -43.5), vec2(16, -43.5)));
	
	return objs;
};

