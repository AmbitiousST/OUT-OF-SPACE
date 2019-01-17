#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

typedef sf::Vector2<float> Vector2;

class ship
{
	std::vector<sf::Texture> _textures;
	sf::Sprite _sprite;
	std::vector<sf::Texture>::iterator _it;

public:
	int _health;
	Vector2 _pos;
	Vector2 _speed;

	ship(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect);

	void render(bool a);

	void update();

	void changeSpeed(Vector2 s);
};

class player : public ship
{
public:
	player(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect);// : ship(tex, pos, vect) {}
};

class enemy : public ship
{
	float dist;
	int side=1;
public:
	int shot = 0;
	enemy(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect);
	void move();
};
