#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "ship.h"
#include "utility.h"
#include "collision.h"
#include "projectiles.h"

typedef sf::Vector2<float> Vector2;

sf::Font font;

const std::string gameName = "Space Gosciniak";

std::vector<sf::Text*> loadText(std::string filename, float offset, float windowSizeX, int size, sf::Color fill, sf::Color outline, float thickness, sf::Text::Style style)
{
	std::ifstream creditsFile;
	creditsFile.open(filename, std::ifstream::in);
	std::string creditsStr, temp;
	std::vector<sf::Text*> Tvect;
	std::vector<std::string> Svect;
	while (!creditsFile.eof())
	{
		std::getline(creditsFile, temp);
		Svect.push_back(temp);
	}
	for (auto it = Svect.begin(); it != Svect.end(); it++)
	{
		sf::Text* tmp = new sf::Text;
		tmp->setStyle(style);
		tmp->setString(*it);
		tmp->setFont(font);
		tmp->setCharacterSize(size);
		tmp->setFillColor(fill);
		tmp->setOutlineColor(outline);
		tmp->setOutlineThickness(thickness);
		tmp->setPosition((windowSizeX - tmp->getLocalBounds().width - tmp->getLocalBounds().left) / 2, offset);
		offset += tmp->getLocalBounds().height + tmp->getLocalBounds().top + 5;
		Tvect.push_back(tmp);
	}
	return Tvect;
}
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
			{
				window.close();
				return -2;
			}
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
					}

				}
			}
		}
		c1.visible(false);
		for (std::vector<button*>::iterator it = bvect.begin(); it != bvect.end(); it++)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			Vector2 bPos = (*it)->_pos;
			if (mousePos.x >= bPos.x && mousePos.x <= bPos.x + (*it)->_size.x && mousePos.y >= bPos.y && mousePos.y <= bPos.y + (*it)->_size.y)
			{
				c1.setPos(Vector2(bPos.x - 25, bPos.y + 30));
				c1.visible(true);
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
{
	std::vector<sf::Drawable*> vect;

	sf::Texture bgMenuT;
	bgMenuT.loadFromFile("../img/bg_menu.png");
	sf::Sprite bgMenu;
	bgMenu.setTexture(bgMenuT);
	vect.push_back(&bgMenu);
	std::vector<sf::Text*> Tvect = loadText("../credits.txt", 75.0f, window.getSize().x, 20, sf::Color(0, 0, 0, 255), sf::Color(255, 255, 255, 255), 3.0f, sf::Text::Regular);
	vect.insert(vect.end(), Tvect.begin(), Tvect.end());
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
			{
				window.close();
				return -2;
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				for (std::vector<button*>::iterator it = bvect.begin(); it != bvect.end(); it++)
				{
					if (event.mouseButton.x >= (*it)->_pos.x && event.mouseButton.x <= (*it)->_pos.x + (*it)->_size.x && event.mouseButton.y >= (*it)->_pos.y && event.mouseButton.y <= (*it)->_pos.y + (*it)->_size.y)
					{
						if ((*it)->_txt == "Back")
						{
							for (auto it = Tvect.begin(); it != Tvect.end(); it++)
								delete *it;
							return 0;
						}
						std::cout << (*it)->_txt << "\n";
					}
				}
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			return 0;
		}
		c1.visible(false);
		for (std::vector<button*>::iterator it = bvect.begin(); it != bvect.end(); it++)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			Vector2 bPos = (*it)->_pos;
			if (mousePos.x >= bPos.x && mousePos.x <= bPos.x + (*it)->_size.x && mousePos.y >= bPos.y && mousePos.y <= bPos.y + (*it)->_size.y)
			{
				c1.setPos(Vector2(bPos.x - 25, bPos.y + 30));
				c1.visible(true);
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

int game(sf::RenderWindow& window)
{
	const int fps = 60;
	const float speedChange = 3;
	const int levelNum = 5;
	sf::Clock clock;
	std::vector<sf::Drawable*> vect;

	//Player
	std::vector<sf::Texture> playerTextures;
	playerProjectilesContainer ppc;
	ppc.texture.loadFromFile("../img/player_proc.png");
	ppc.speed = Vector2(0, -8);
	for (int i = 0; i < 5; i++)
	{
		sf::Texture tex;
		tex.loadFromFile("../img/player_spritesheet.png", sf::IntRect(i * 41, 0, 41, 52));
		playerTextures.push_back(tex);
	}
	int shot = 0;
	std::vector<sf::Texture> hpBarsTextures[4];
	std::vector<sf::Texture> playerHpBarTextures;
	for (int i = 0; i < 5; i++)
	{
		sf::Texture tex;
		tex.loadFromFile("../img/hp_bar5.png", sf::IntRect(i * 27, 0, 27, 10));
		playerHpBarTextures.push_back(tex);
	}
	player Player(playerTextures, Vector2(400, 500), vect, playerHpBarTextures);
	for (int j = 2; j < 5; j++)
	{
		std::vector<sf::Texture> hpBarTexture;
		for (int i = 0; i < j; i++)
		{
			sf::Texture tex;
			int w = j * 4 + j + 1;	//szerokość jednej części paska
			tex.loadFromFile("../img/hp_bar" + std::to_string(j) + ".png", sf::IntRect(i * w, 0, w, 10));
			hpBarTexture.push_back(tex);
		}
		hpBarsTextures[j - 2] = hpBarTexture;
	}
	hpBarsTextures[3] = playerHpBarTextures;

	//Background
	sf::Texture bgGameTex[5];
	bgGameTex[0].loadFromFile("../img/bg_game1.jpg");
	bgGameTex[1].loadFromFile("../img/bg_game2.png");
	bgGameTex[2].loadFromFile("../img/bg_game3.jpg");
	bgGameTex[3].loadFromFile("../img/bg_game4.jpg");
	bgGameTex[4].loadFromFile("../img/bg_menu.png");

	//Enemy
	std::list<enemy*> evect;
	std::vector<sf::Texture> enemyProcTextures;
	sf::Texture temp;
	temp.loadFromFile("../img/enemy_proc.png");
	enemyProcTextures.push_back(temp);
	temp.loadFromFile("../img/enemy_proc2.png");
	enemyProcTextures.push_back(temp);
	temp.loadFromFile("../img/enemy_proc3.png");
	enemyProcTextures.push_back(temp);
	enemyProjectilesContainer epc(enemyProcTextures);
	std::vector<sf::Texture> enemyTextures[4];
	std::vector<sf::Texture> tmp;
	for (int i = 0; i < 5; i++)
	{
		sf::Texture tex;
		tex.loadFromFile("../img/enemy1.png", sf::IntRect(i * 35, 0, 35, 48));
		tmp.push_back(tex);
	}
	enemyTextures[0] = tmp;
	tmp.clear();
	for (int i = 0; i < 4; i++)
	{
		sf::Texture tex;
		tex.loadFromFile("../img/enemy2.png", sf::IntRect(i * 35, 0, 35, 50));
		tmp.push_back(tex);
	}
	enemyTextures[1] = tmp;
	tmp.clear();
	for (int i = 0; i < 5; i++)
	{
		sf::Texture tex;
		tex.loadFromFile("../img/enemy3.png", sf::IntRect(i * 35, 0, 35, 45));
		tmp.push_back(tex);
	}
	enemyTextures[2] = tmp;
	tmp.clear();
	for (int i = 0; i < 5; i++)
	{
		sf::Texture tex;
		tex.loadFromFile("../img/enemy4.png", sf::IntRect(i * 59, 0, 59, 54));
		tmp.push_back(tex);
	}
	enemyTextures[3] = tmp;
	sf::Sprite bgGame;
	std::vector<sf::Text*> Tvictory = loadText("../victory.txt", 75, window.getSize().x, 70, sf::Color(0, 0, 0, 255), sf::Color(255, 255, 255, 255), 4, sf::Text::Bold);
	std::vector<sf::Text*> Tfailure = loadText("../failure.txt", 75, window.getSize().x, 70, sf::Color(0, 0, 0, 255), sf::Color(255, 255, 255, 255), 4, sf::Text::Bold);
	for (int level = 5; level <= levelNum; level++)
	{
		switch (level)
		{
		case 1:
			for (int i = 0; i < 5; i++)
			{
				enemy *e = new enemy(enemyTextures[0], Vector2(50.0f*i, 100.0f), vect, 1, 2, 0, hpBarsTextures[0]);
				evect.push_back(e);
			}
			break;
		case 2:
			for (int i = 0; i < 5; i++)
			{
				enemy *e = new enemy(enemyTextures[0], Vector2(100.0f*i, 150.0f), vect, 1, 2, 0, hpBarsTextures[0]);
				evect.push_back(e);
			}
			{
				enemy *e = new enemy(enemyTextures[1], Vector2(200.0f, 70.0f), vect, 0, 3, 1, hpBarsTextures[1]);
				evect.push_back(e);
				enemy *e2 = new enemy(enemyTextures[1], Vector2(565.0f, 70.0f), vect, 0, 3, 1, hpBarsTextures[1]);
				evect.push_back(e2);
			}
			break;
		case 3:
			for (int i = 0; i < 3; i++)
			{

				enemy* e = new enemy(enemyTextures[1], Vector2(272.5f + 85.0f*i, 70.0f), vect, 2, 3, 1, hpBarsTextures[1]);
				evect.push_back(e);
			}
			for (int i = 0; i < 2; i++)
			{
				enemy* e2 = new enemy(enemyTextures[0], Vector2(100 + 100.0f*i, 150.0f), vect, -1, 2, 0, hpBarsTextures[0]);
				evect.push_back(e2);
				enemy* e3 = new enemy(enemyTextures[0], Vector2(565 + 100.0f*i, 150.0f), vect, 1, 2, 0, hpBarsTextures[0]);
				evect.push_back(e3);
			}
			break;
		case 4:
			for (int i = 0; i < 5; i++)
			{
				enemy* e = new enemy(enemyTextures[2], Vector2(62.5f + 135.0f*i, 50.0f), vect, 3, 4, 0, hpBarsTextures[2]);
				evect.push_back(e);
			}
			break;
		case 5:
		{
			enemy* e = new enemy(enemyTextures[3], Vector2(370.5f, 50.0f), vect, 4, 5, 3, hpBarsTextures[3]);
			evect.push_back(e);
		}
		break;
		}
		bgGame.setTexture(bgGameTex[level - 1]);

		//Level text
		sf::Text* levelText = new sf::Text;
		const std::string levelStr = "Level " + std::to_string(level);
		*levelText = sf::Text(levelStr, font, 70);
		levelText->setStyle(sf::Text::Bold);
		levelText->setPosition((window.getSize().x - levelText->getLocalBounds().width - levelText->getLocalBounds().left) / 2, 75);
		levelText->setFillColor(sf::Color(0, 0, 0, 255));
		levelText->setOutlineColor(sf::Color(255, 255, 255, 255));
		levelText->setOutlineThickness(4);
		window.clear();
		window.draw(bgGame);
		//Jedna runda update'u wszystkiego, żeby paski hp wyglądały po ludzku
		Player.update();
		ppc.update(window, evect);
		for (auto it = evect.begin(); it != evect.end(); it++) {
			(*it)->update();
		}
		epc.update(window, Player);
		clock.restart();
		for (std::vector<sf::Drawable*>::iterator it = vect.begin(); it != vect.end(); it++)
			window.draw(**it);
		window.draw(*levelText);
		window.display();
		while (clock.getElapsedTime().asMilliseconds() < 500);
		delete levelText;

		//Loop
		while (evect.size() > 0 && window.isOpen())
		{
			clock.restart();
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
					return -2;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				Player.changeSpeed(Vector2(speedChange*-1, 0));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				Player.changeSpeed(Vector2(speedChange, 0));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				Player.changeSpeed(Vector2(0, speedChange*-1));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				Player.changeSpeed(Vector2(0, speedChange));
			}
			if (shot == 1)
			{
				ppc.addProjectile(Vector2(Player.pos.x + 12, Player.pos.y));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				shot++;
				shot %= 32;
			}
			else if (shot > 0)
			{
				shot++;
				shot %= 32;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				return 0;
			}
			window.clear();

			//Render
			window.draw(bgGame);
			Player.update();
			ppc.update(window, evect);
			for (auto it = evect.begin(); it != evect.end(); it++)
			{
				(*it)->move();
				(*it)->update();
				if ((*it)->shot == 1)
				{
					switch ((*it)->procType)
					{
					case 0:
						epc.addProjectile(epc.textures[0], Vector2((*it)->pos.x + 12, (*it)->pos.y), Vector2(0.0f, 7.0f));
						break;
					case 1:
						epc.addProjectile(epc.textures[1], Vector2((*it)->pos.x + 12, (*it)->pos.y), Vector2(0.0f, 9.0f));
						break;
					case 3:
						epc.addProjectile(epc.textures[2], Vector2((*it)->pos.x - 16.5f, (*it)->pos.y), Vector2(0.0f, 9.0f));
						epc.addProjectile(epc.textures[2], Vector2((*it)->pos.x + 13.5f, (*it)->pos.y), Vector2(0.0f, 9.0f));
						break;
					}
				}
			}
			epc.update(window, Player);
			if (Player.health < 1)
			{
				bgGame.setTexture(bgGameTex[level - 1]);

				window.clear();
				window.draw(bgGame);
				clock.restart();
				for (auto it = Tfailure.begin(); it != Tfailure.end(); it++)
					window.draw(**it);
				window.display();
				while (clock.getElapsedTime().asMilliseconds() < 1000);
				for (auto it = Tfailure.begin(); it != Tfailure.end(); it++)
					delete *it;
				for (auto it = Tvictory.begin(); it != Tvictory.end(); it++)
					delete *it;
				for (auto it = evect.begin(); it != evect.end(); it++)
					delete *it;
				return 0;
			}
			for (std::vector<sf::Drawable*>::iterator it = vect.begin(); it != vect.end(); it++)
				window.draw(**it);
			window.display();
			while (clock.getElapsedTime().asMilliseconds() < 1000 / fps);	//Fps limiter
		}
	}
	bgGame.setTexture(bgGameTex[levelNum - 1]);

	window.clear();
	window.draw(bgGame);
	for (auto it = Tvictory.begin(); it != Tvictory.end(); it++)
		window.draw(**it);
	clock.restart();
	window.display();
	while (clock.getElapsedTime().asMilliseconds() < 1000);

	for (auto it = Tfailure.begin(); it != Tfailure.end(); it++)
		delete *it;
	for (auto it = Tvictory.begin(); it != Tvictory.end(); it++)
		delete *it;
	return 0;
}

int main()
{
	font.loadFromFile("../arial.ttf");
	int state = 0;								// 0 - menu, 1 - gra, 2 - credits, -1 błąd, -2 zamknij
	sf::RenderWindow window(sf::VideoMode(800, 600), gameName);
	srand(time(NULL));
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
