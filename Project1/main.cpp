#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

typedef sf::Vector2<float> Vector2;

sf::Font font;

const std::string gameName = "Space Gosciniak";			//Ale gówniana nazwa - serio trzeba wymyślić coś lepszego

class button {
	sf::RectangleShape _box;
	sf::Text _text;
public:
	Vector2 _pos, _size;
	std::string _txt;
	button(Vector2 pos, std::string txt, Vector2 size, std::vector<sf::Drawable*>& vect) : _pos(pos), _size(size), _txt(txt)
	{
		_box = sf::RectangleShape(_size);
		_box.setPosition(pos);
		//_box.setFillColor(sf::Color(170, 170, 170, 255));
		_box.setFillColor(sf::Color(170, 170, 170, 0));
		_text = sf::Text(txt, font, 40);
		_text.setFillColor(sf::Color(0, 0, 0, 255));
		_text.setOutlineColor(sf::Color(255, 255, 255, 255));
		_text.setOutlineThickness(2);
		auto bounds = _text.getLocalBounds();
		_text.setPosition(pos.x + (_size.x - bounds.width - bounds.left) / 2, pos.y + (_size.y - bounds.height - bounds.top) / 2);	//Center
		_text.setFillColor(sf::Color(0, 0, 0, 255));
		vect.push_back(&_box);
		vect.push_back(&_text);
	}
};

class cursor
{
	sf::Texture _texture;
	sf::Sprite _leftSprite, _rightSprite;
public:
	Vector2 _pos;
	cursor(Vector2 pos, std::vector<sf::Drawable*>& vect) : _pos(pos)
	{
		_texture.loadFromFile("../img/cursor.png");
		_leftSprite.setTexture(_texture);
		_leftSprite.setPosition(pos);
		_leftSprite.setScale(2, 2);
		_rightSprite.setTexture(_texture);
		_rightSprite.scale(-2, 2);
		_rightSprite.setPosition(pos.x + 250, pos.y);
		vect.push_back(&_leftSprite);
		vect.push_back(&_rightSprite);
	}

	void render(bool a)
	{
		//Smieszny patent na pokazywanie/chowanie sprite'ów
		_leftSprite.setColor(sf::Color(255, 255, 255, a ? 255 : 0));
		_rightSprite.setColor(sf::Color(255, 255, 255, a ? 255 : 0));
	}

	void setPos(Vector2 pos)
	{
		_pos = pos;
		_leftSprite.setPosition(pos);
		_rightSprite.setPosition(pos.x + 250, pos.y);
	}
};

class ship
{
	sf::Texture _texture;
	sf::Sprite _sprite;		//Zmienić na animated sprite: https://github.com/SFML/SFML/wiki/Source%3A-AnimatedSprite
	Vector2 _pos;

public:
	int _health;
	Vector2 _speed;

	ship(sf::Texture tex, Vector2 pos, std::vector<sf::Drawable*>& vect) : _texture(tex), _pos(pos)
	{
		_sprite.setTexture(_texture);
		_speed = Vector2(0, 0);
		_sprite.setPosition(_pos);
		vect.push_back(&_sprite);
	}

	void render(bool a)
	{
		//Smieszny patent na pokazywanie/chowanie sprite'ów
		_sprite.setColor(sf::Color(255, 255, 255, a ? 255 : 0));
	}

	void updatePos()
	{
		_pos.x += _speed.x;
		_pos.y += _speed.y;
		_sprite.setPosition(_pos);
	}
};

class player : public ship
{
public:
	player(sf::Texture tex, Vector2 pos, std::vector<sf::Drawable*>& vect) : ship(tex, pos, vect)
	{

	}
};

int menu(sf::RenderWindow& window)
{
	std::vector<sf::Drawable*> vect;
	std::vector<button*> bvect;
	sf::Texture bgMenuT;
	bgMenuT.loadFromFile("../img/bg_menu.png");
	sf::Sprite bgMenu;
	bgMenu.setTexture(bgMenuT);
	vect.push_back(&bgMenu);
	button b1(Vector2(300, 225), "Play", Vector2(200, 75), vect);
	button b2(Vector2(300, 350), "Credits", Vector2(200, 75), vect);
	button b3(Vector2(300, 475), "Quit", Vector2(200, 75), vect);
	bvect.push_back(&b1);
	bvect.push_back(&b2);
	bvect.push_back(&b3);
	sf::Text logo;
	logo = sf::Text(gameName, font, 70);
	logo.setStyle(sf::Text::Bold);
	logo.setPosition((window.getSize().x - logo.getLocalBounds().width - logo.getLocalBounds().left) / 2, 75);
	//logo.setFillColor(sf::Color(0, 204, 0, 255));	//Dosyć zielony
	logo.setFillColor(sf::Color(0, 0, 0, 255));
	logo.setOutlineColor(sf::Color(255, 255, 255, 255));
	logo.setOutlineThickness(4);
	vect.push_back(&logo);
	cursor c1 = cursor(Vector2(0, 0), vect);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				for (std::vector<button*>::iterator it = bvect.begin(); it != bvect.end(); it++)
				{
					if (event.mouseButton.x >= (*it)->_pos.x && event.mouseButton.x <= (*it)->_pos.x + (*it)->_size.x && event.mouseButton.y >= (*it)->_pos.y && event.mouseButton.y <= (*it)->_pos.y + (*it)->_size.y)
					{
						if ((*it)->_txt == "Quit")
						{
							window.close();
							return -2;
						}
						if ((*it)->_txt == "Credits")
						{
							return 1;
						}
						if ((*it)->_txt == "Play")
						{
							return 2;
						}
						std::cout << (*it)->_txt << "\n";
					}

				}
			}
		}
		c1.render(false);
		for (std::vector<button*>::iterator it = bvect.begin(); it != bvect.end(); it++)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			Vector2 bPos = (*it)->_pos;
			if (mousePos.x >= bPos.x && mousePos.x <= bPos.x + (*it)->_size.x && mousePos.y >= bPos.y && mousePos.y <= bPos.y + (*it)->_size.y)
			{
				c1.setPos(Vector2(bPos.x - 25, bPos.y + 30));
				c1.render(true);
				break;
			}
		}
		window.clear();
		for (std::vector<sf::Drawable*>::iterator it = vect.begin(); it != vect.end(); it++)
			window.draw(**it);
		window.display();
	}
	return 0;
}

int credits(sf::RenderWindow& window)
/*
	Trzebaby każdą linijkę dać do osobnego sf::Text.
	Wtedy możnaby to wyśrodkować, bo narazie wygląda kiepsko.
*/
{
	std::vector<sf::Drawable*> vect;
	std::ifstream creditsFile;
	creditsFile.open("../credits.txt", std::ifstream::in);
	std::string creditsStr, temp;
	while (!creditsFile.eof())
	{
		std::getline(creditsFile, temp);
		creditsStr += temp + '\n';
	}
	sf::Text creditsText;
	creditsText = sf::Text(creditsStr, font, 20);
	creditsText.setPosition((window.getSize().x - creditsText.getLocalBounds().width - creditsText.getLocalBounds().left) / 2, 75);
	//creditsText.setFillColor(sf::Color(0, 204, 0, 255));
	creditsText.setFillColor(sf::Color(0, 0, 0, 255));
	creditsText.setOutlineColor(sf::Color(255, 255, 255, 255));
	creditsText.setOutlineThickness(1.5);
	sf::Texture bgMenuT;
	bgMenuT.loadFromFile("../img/bg_menu.png");
	sf::Sprite bgMenu;
	bgMenu.setTexture(bgMenuT);
	vect.push_back(&bgMenu);
	vect.push_back(&creditsText);
	std::vector<button*> bvect;
	button b1(Vector2(300, 475), "Back", Vector2(200, 75), vect);
	bvect.push_back(&b1);
	cursor c1 = cursor(Vector2(0, 0), vect);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				for (std::vector<button*>::iterator it = bvect.begin(); it != bvect.end(); it++)
				{
					if (event.mouseButton.x >= (*it)->_pos.x && event.mouseButton.x <= (*it)->_pos.x + (*it)->_size.x && event.mouseButton.y >= (*it)->_pos.y && event.mouseButton.y <= (*it)->_pos.y + (*it)->_size.y)
					{
						if ((*it)->_txt == "Back")
						{
							return 0;
						}
						std::cout << (*it)->_txt << "\n";
					}
				}
			}
		}
		c1.render(false);
		for (std::vector<button*>::iterator it = bvect.begin(); it != bvect.end(); it++)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			Vector2 bPos = (*it)->_pos;
			if (mousePos.x >= bPos.x && mousePos.x <= bPos.x + (*it)->_size.x && mousePos.y >= bPos.y && mousePos.y <= bPos.y + (*it)->_size.y)
			{
				c1.setPos(Vector2(bPos.x - 25, bPos.y + 30));
				c1.render(true);
				break;
			}
		}
		window.clear();
		window.clear();
		for (std::vector<sf::Drawable*>::iterator it = vect.begin(); it != vect.end(); it++)
			window.draw(**it);
		window.display();
	}
	return 0;
}

int game(sf::RenderWindow& window)
{
	std::vector<sf::Drawable*> vect;
	/*
	std::vector<button*> bvect;
	button b1(Vector2(300, 475), "Return", Vector2(200, 75), vect);
	bvect.push_back(&b1);
	*/
	sf::Texture playerTex;
	playerTex.loadFromFile("../img/player.png");
	player Player(playerTex, Vector2(0, 0), vect);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			/*
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				for (std::vector<button*>::iterator it = bvect.begin(); it != bvect.end(); it++)
				{
					if (event.mouseButton.x >= (*it)->_pos.x && event.mouseButton.x <= (*it)->_pos.x + (*it)->_size.x && event.mouseButton.y >= (*it)->_pos.y && event.mouseButton.y <= (*it)->_pos.y + (*it)->_size.y)
					{
						if ((*it)->_txt == "Return")
						{
							return 0;
						}
						std::cout << (*it)->_txt << "\n";
					}

				}
			}
			*/
		}
		window.clear();
		for (std::vector<sf::Drawable*>::iterator it = vect.begin(); it != vect.end(); it++)
			window.draw(**it);
		window.display();
	}
	return 0;
}

int main()
{
	font.loadFromFile("../arial.ttf");
	int state = 0;																	// 0 - menu, 1 - gra, 2 - credits, -1 błąd, -2 zamknij
	sf::RenderWindow window(sf::VideoMode(800, 600), gameName);
	sf::Clock clock;
	while (state >= 0)
	{
		switch (state)
		{
		case 0:
			state = menu(window);
			break;
		case 1:
			state = credits(window);
			break;
		case 2:
			state = game(window);
			break;
		}
	}
	return 0;
}