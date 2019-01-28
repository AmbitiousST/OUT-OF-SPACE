#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "ship.h"
#include "collisions.h"

typedef sf::Vector2f Vector2;
typedef sf::Vector2i Vector2i;

class projectile : public colidable
{
	sf::Texture _texture;
	Vector2 _speed;

public:
	Vector2 _pos;
	sf::Sprite _sprite;

	projectile(sf::Texture tex, Vector2 pos, Vector2 speed, std::vector<std::pair<Vector2, Vector2>>& colis);
	void update();
	Vector2* getPos();
};
class playerProjectilesContainer
{
	std::list<projectile*> pvect;
	std::vector <Vector2>* _expPos;

public:
	sf::Texture texture;
	Vector2 speed;
	std::vector<std::pair<Vector2, Vector2>> colis;
	playerProjectilesContainer(std::vector <Vector2>* expPos, std::vector<std::pair<Vector2, Vector2>>& colis);
	void clear();
	void addProjectile(Vector2 pos);
	void update(sf::RenderWindow& window, std::list<enemy*>& evect);
	~playerProjectilesContainer();
};

class enemyProjectilesContainer
{
	std::list<projectile*> pvect;
public:
	std::vector<sf::Texture> textures;
	enemyProjectilesContainer(std::vector<sf::Texture> &tex);
	void addProjectile(sf::Texture tex, Vector2 pos, Vector2 speed, std::vector<std::pair<Vector2, Vector2>>& colis);
	void update(sf::RenderWindow& window, player& Player);
	void clear();
	~enemyProjectilesContainer();
};
