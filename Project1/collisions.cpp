#include "collisions.h"

bool testColision(colidable* a, colidable* b) 
{
	Vector2 apos = a->getPos();
	Vector2 bpos = b->getPos();
	auto wekt = [](Vector2& p1, Vector2& p2, Vector2& p3) {return (p2.x - p1.x)*(p3.y - p1.y) - (p3.x - p1.x)*(p2.y - p1.y); };
	for (auto it1 = a->colisionlines.begin(); it1 != a->colisionlines.end(); it1++)
	{
		for (auto it2 = b->colisionlines.begin(); it2 != b->colisionlines.end(); it2++)
		{
			Vector2 p1 = it1->first  + apos;
			Vector2 p2 = it1->second + apos;
			Vector2 p3 = it2->first  + bpos;
			Vector2 p4 = it2->second + bpos;
			float x1 = std::min(p1.x, p2.x);
			float y1 = std::min(p1.y, p2.y);
			float x2 = std::max(p1.x, p2.x);
			float y2 = std::max(p1.y, p2.y);
			float x3 = std::min(p3.x, p4.x);
			float y3 = std::min(p3.y, p4.y);
			float x4 = std::max(p3.x, p4.x);
			float y4 = std::max(p3.y, p4.y);
			if (x2<x3 || x1>x4 || y2<y3 || y1>y4)
				continue;
			if (wekt(p1, p3, p2)*wekt(p1, p4, p2) <= 0 && wekt(p3, p1, p4)*wekt(p3, p2, p4) <= 0)
				return 1;
		}
	}
	return 0;
}
Vector2 colidable::getPos()
{
	return Vector2(0, 0);
}