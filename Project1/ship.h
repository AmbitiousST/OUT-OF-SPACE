#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

typedef sf::Vector2f Vector2;
typedef sf::Vector2i Vector2i;

class ship
{
	std::vector<sf::Texture> _textures;
	sf::Sprite _sprite;
	std::vector<sf::Texture>::iterator _it;

public:
	int _health;
	Vector2 _pos;
	Vector2 _speed;
	Vector2i _collision;

	ship(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect);

	void visible(bool a);

	void update();

	void changeSpeed(Vector2 s);
};

class player : public ship
{
public:
	player(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect);
};

class enemy : public ship
{
	float _dist;
	int _side;
	int _aiType;
	float _baseSpeed;

public:
	int shot;

	enemy(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect, int aiType);

	void move();
};
