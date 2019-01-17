#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

typedef sf::Vector2<float> Vector2;

extern sf::Font font;

class button {
	sf::RectangleShape _box;
	sf::Text _text;
public:
	Vector2 _pos, _size;
	std::string _txt;
	button(Vector2 pos, std::string txt, Vector2 size, std::vector<sf::Drawable*>& vect);
};

class cursor
{
	sf::Texture _texture;
	sf::Sprite _leftSprite, _rightSprite;
public:
	Vector2 _pos;
	cursor(Vector2 pos, std::vector<sf::Drawable*>& vect);
	void render(bool a);
	void setPos(Vector2 pos);
};