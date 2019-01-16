#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>

typedef sf::Vector2<float> Vector2;

sf::Font font;

const std::string gameName = "SPACE BATTLE";			//Ale g�wniana nazwa - serio trzeba wymy�li� co� lepszego

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
		_box.setFillColor(sf::Color(170, 170, 170, 255));
		_text = sf::Text(txt, font, 40);
		auto bounds = _text.getLocalBounds();
		_text.setPosition(pos.x + (_size.x - bounds.width - bounds.left) / 2, pos.y + (_size.y - bounds.height - bounds.top) / 2);	//Center
		//std::cout << x.left << "\n" << x.height << "\n" << x.top << "\n" << x.width;
		_text.setFillColor(sf::Color(0, 0, 0, 255));
		vect.push_back(&_box);
		vect.push_back(&_text);
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
	logo = sf::Text(gameName,font,70);
	logo.setStyle(sf::Text::Bold);
	logo.setPosition((window.getSize().x - logo.getLocalBounds().width - logo.getLocalBounds().left) / 2, 75);
	logo.setFillColor(sf::Color(0, 204, 0, 255));	//Dosy� zielony
	vect.push_back(&logo);
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
						std::cout << (*it)->_txt << "\n";
					}

				}
			}
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
	int state = 0;																	// 0 - menu, -1 b��d, -2 zamknij
	sf::RenderWindow window(sf::VideoMode(800, 600), gameName);
	sf::Clock clock;
	while (state >= 0)
	{
		switch (state)
		{
		case 0:
			state = menu(window);
			break;
		}
	}
	return 0;
}