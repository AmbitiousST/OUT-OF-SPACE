#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
typedef sf::Vector2f Vector2;
class colidable
{
public:
	std::vector<std::pair<Vector2, Vector2>> colisionlines;
	virtual Vector2 getPos();
};
bool testColision(colidable* a,colidable* b);