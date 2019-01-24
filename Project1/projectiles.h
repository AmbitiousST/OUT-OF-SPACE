#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "collision.h"
#include "ship.h"

typedef sf::Vector2f Vector2;
typedef sf::Vector2i Vector2i;

class projectile
{
	sf::Texture _texture;
	Vector2 _speed;

public:
	Vector2 _pos;
	sf::Sprite _sprite;

	projectile(sf::Texture tex, Vector2 pos, Vector2 speed);
	void update();
};
class playerProjectilesContainer
{
	std::list<projectile*> pvect;

public:
	sf::Texture texture;
	Vector2 speed;

	void addProjectile(Vector2 pos);
	void update(sf::RenderWindow& window, std::list<enemy*>& evect);
	~playerProjectilesContainer();
};

class enemyProjectilesContainer
{
	std::list<projectile*> pvect;
public:
	sf::Texture texture;
	Vector2 speed;
	void addProjectile(Vector2 pos);
	void update(sf::RenderWindow& window, player& Player);
	~enemyProjectilesContainer();
};
