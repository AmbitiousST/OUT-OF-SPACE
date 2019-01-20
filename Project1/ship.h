#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

typedef sf::Vector2f Vector2;
typedef sf::Vector2i Vector2i;

class ship
{
	std::vector<sf::Texture> _textures;
	std::vector<sf::Texture>::iterator _it;

	friend class hpBar;	//pierwszy raz tego u¿ywam

public:
	int health;
	Vector2 pos, speed;
	Vector2i collision;
	sf::Sprite _sprite;
	//std::vector<sf::Drawable*>::iterator vectorPos;

	ship(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect);
	void visible(bool a);
	void update();
	void changeSpeed(Vector2 s);
	void takeDamage(int amount);
};

class player : public ship
{
public:
	player(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect);
};

class enemy : public ship
{
	float _dist, _baseSpeed;
	int _side, _aiType;

public:
	int shot;
	enemy(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect, int aiType);
	void move();
};

class hpBar
{
	std::vector<sf::Texture> _textures;
	sf::Sprite _sprite;
	Vector2 _pos;
	ship *_ship;

public:
	hpBar(std::vector<sf::Texture> &tex, std::vector<sf::Drawable*>& vect, ship* ship);
	void update();
};
