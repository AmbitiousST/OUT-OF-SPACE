#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

typedef sf::Vector2f Vector2;
typedef sf::Vector2i Vector2i;

class hpBar
{
	std::vector<sf::Texture> _textures;
	sf::Sprite _sprite;
	Vector2 _pos;
	sf::Sprite* _spritePtr;
	std::vector<sf::Drawable*>* _vectPtr;
	int _maxHp;

public:
	float width;

	hpBar(std::vector<sf::Texture> &tex, std::vector<sf::Drawable*>& vect, int hp, Vector2 p);
	~hpBar();
	void update(Vector2 p, int hp);
};

class ship
{
	std::vector<sf::Texture> _textures;
	std::vector<sf::Texture>::iterator _it;
	std::vector<sf::Drawable*>* _vectPtr;

public:
	int health;
	Vector2 pos, speed;
	Vector2i collision;
	sf::Sprite sprite;

	ship(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect);
	~ship();
	void visible(bool a);
	void update();
	void changeSpeed(Vector2 s);
	void takeDamage(int amount);
};

class player : public ship
{
	hpBar *_bar;

public:
	player(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect, std::vector<sf::Texture> &barTex);
	void update();
	~player();
};

class enemy : public ship
{
	float _dist, _baseSpeed, _superSecretVariable;
	int _side, _aiType;
	hpBar *_bar;

public:
	int shot, procType;
	enemy(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect,
		int aiType, int hp, int type, std::vector<sf::Texture> &barTex);
	~enemy();
	void update();
	void move();
};
