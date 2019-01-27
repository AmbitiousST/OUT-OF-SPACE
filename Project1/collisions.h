#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
typedef sf::Vector2f Vector2;
class colidable
{
public:
	Vector2 lower;
	sf::FloatRect BB;
	Vector2* posPTR;
	std::vector<std::pair<Vector2, Vector2>> colisionlines;
	virtual Vector2* getPos();
	void updateBB();
};
bool testColision(colidable* a,colidable* b);