#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "collisions.h"

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
	void visible(bool a);
	void update(Vector2 p, int hp);
};

class ship : public colidable
{
	std::vector<sf::Texture> _textures;
	std::vector<sf::Texture>::iterator _it;
	std::vector<sf::Drawable*>* _vectPtr;

public:
	int health;
	Vector2 pos, speed;
	Vector2i collision;
	sf::Sprite sprite;

	ship(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect, std::vector<std::pair<Vector2, Vector2>>& colis);
	~ship();
	void visible(bool a);
	void update();
	void changeSpeed(Vector2 s);
	void takeDamage(int amount);
	Vector2* getPos();
};

class player : public ship
{

public:
	hpBar *_bar;
	player(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect, std::vector<sf::Texture> &barTex, std::vector<std::pair<Vector2, Vector2>>& colis);
	void update();
	~player();
};

class enemy : public ship
{
	float _dist, _baseSpeed, _aiVar, _aiVar2;
	int _side, _aiType;
	hpBar *_bar;

public:
	int shot, procType;
	enemy(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect,
		int aiType, int hp, int type, std::vector<sf::Texture> &barTex, std::vector<std::pair<Vector2, Vector2>>& colis);
	~enemy();
	void update();
	void move();
};

class explosion
{
	Vector2 _pos;
	std::vector <sf::Texture> _textures;
	std::vector <sf::Texture>::iterator _it;
	std::vector<sf::Drawable*>* _vectPtr;

public:
	sf::Sprite sprite;

	explosion(Vector2 p, std::vector <sf::Texture>& tex, std::vector<sf::Drawable*>& vect);
	~explosion();
	bool update();
};
class boss : public enemy
{
	hpBar *_bar;
public:
	boss(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect, std::vector<sf::Texture> &barTex, std::vector<std::pair<Vector2, Vector2>>& colis);
	void update();
	void move();
};