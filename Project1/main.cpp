#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include "ship.h"
#include "utility.h"
#include "projectiles.h"


typedef sf::Vector2<float> Vector2;

sf::Font font;

const std::string gameName = "Space Gosciniak";

std::vector<sf::Text*> loadText(std::string filename, float offset, float windowSizeX, int size, sf::Color fill, sf::Color outline, float thickness, sf::Text::Style style = sf::Text::Regular)
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
	std::vector<sf::Text*> Tvect = loadText("../credits.txt", 75.0f, window.getSize().x, 20, sf::Color(0, 0, 0, 255), sf::Color(255, 255, 255, 255), 3.0f);
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
	const int levelNum = 10;
	sf::Clock clock;
	std::vector<sf::Drawable*> vect;

	//Colisions
	std::vector<std::pair<Vector2, Vector2>> playerColis;
	std::vector<std::pair<Vector2, Vector2>> playerProjectilesColis;
	std::vector<std::pair<Vector2, Vector2>> enemyColis[6];
	std::vector<std::pair<Vector2, Vector2>> enemyProjectileColis[4];
	{
		auto lamb = [](float a, float b, float c, float d) {return std::make_pair(Vector2(a, b), Vector2(c, d)); };
		playerColis.push_back(lamb(20, 0, 0, 46));
		playerColis.push_back(lamb(40, 46, 0, 46));
		playerColis.push_back(lamb(20, 0, 40, 46));

		playerProjectilesColis.push_back(lamb(6, 7, 6, 25));
		playerProjectilesColis.push_back(lamb(10, 7, 10, 25));
		playerProjectilesColis.push_back(lamb(6, 7, 10, 7));
		playerProjectilesColis.push_back(lamb(6, 25, 10, 25));

		enemyColis[0].push_back(lamb(0, 14, 34, 14));
		enemyColis[0].push_back(lamb(17, 47, 0, 14));
		enemyColis[0].push_back(lamb(17, 47, 34, 14));

		enemyColis[1].push_back(lamb(0, 9, 34, 9));
		enemyColis[1].push_back(lamb(17, 49, 0, 9));
		enemyColis[1].push_back(lamb(17, 49, 34, 9));

		enemyColis[2].push_back(lamb(0, 15, 34, 15));
		enemyColis[2].push_back(lamb(17, 44, 34, 15));
		enemyColis[2].push_back(lamb(0, 15, 17, 44));

		enemyColis[3].push_back(lamb(0, 12, 58, 12));
		enemyColis[3].push_back(lamb(24, 53, 34, 53));
		enemyColis[3].push_back(lamb(24, 53, 0, 12));
		enemyColis[3].push_back(lamb(58, 12, 34, 53));

		enemyColis[4].push_back(lamb(0, 4, 63, 5));
		enemyColis[4].push_back(lamb(32, 74, 0, 4));
		enemyColis[4].push_back(lamb(32, 74, 63, 5));

		enemyColis[5].push_back(lamb(10, 0, 84, 0));
		enemyColis[5].push_back(lamb(94, 25, 84, 0));
		enemyColis[5].push_back(lamb(94, 25, 68, 75));
		enemyColis[5].push_back(lamb(26, 75, 68, 75));
		enemyColis[5].push_back(lamb(26, 75, 0, 25));
		enemyColis[5].push_back(lamb(0, 25, 10, 0));

		enemyProjectileColis[0].push_back(lamb(6, 7, 6, 25));
		enemyProjectileColis[0].push_back(lamb(10, 7, 10, 25));
		enemyProjectileColis[0].push_back(lamb(6, 7, 10, 7));
		enemyProjectileColis[0].push_back(lamb(6, 25, 10, 25));

		enemyProjectileColis[1].push_back(lamb(4, 8, 9, 8));
		enemyProjectileColis[1].push_back(lamb(4, 8, 4, 18));
		enemyProjectileColis[1].push_back(lamb(9, 8, 9, 18));
		enemyProjectileColis[1].push_back(lamb(4, 18, 9, 18));

		enemyProjectileColis[2].push_back(lamb(27, 26, 34, 26));
		enemyProjectileColis[2].push_back(lamb(27, 26, 27, 84));
		enemyProjectileColis[2].push_back(lamb(34, 26, 34, 84));
		enemyProjectileColis[2].push_back(lamb(27, 84, 34, 84));

		enemyProjectileColis[3].push_back(lamb(21, 69, 33, 69));
		enemyProjectileColis[3].push_back(lamb(21, 69, 21, 81));
		enemyProjectileColis[3].push_back(lamb(21, 81, 33, 81));
		enemyProjectileColis[3].push_back(lamb(33, 81, 33, 69));

	}

	//Jump
	std::vector <sf::Texture> jumpTex;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sf::Texture a;
			a.loadFromFile("../img/jump_animation.png", sf::IntRect(128 * j, 128 * i, 128, 128));
			jumpTex.push_back(a);
		}
	}

	//Explosions
	std::vector <Vector2> explosionPositions;
	std::vector <explosion*> explosions;
	std::vector <sf::Texture> exp1;
	//std::vector <sf::Texture> exp2;
	for (int i = 0; i < 10; i++)
	{
		sf::Texture a;
		a.loadFromFile("../img/explosion1.png", sf::IntRect(1 + 92 * i, 1, 89, 89));
		exp1.push_back(a);
	}

	//Sound and music
	sf::SoundBuffer bigBoom;
	bigBoom.loadFromFile("../sound/DeathFlash.flac");

	sf::SoundBuffer jumpSound;
	jumpSound.loadFromFile("../sound/jump.wav");

	sf::SoundBuffer shootSoundVector[5];
	shootSoundVector[0].loadFromFile("../sound/laser3.wav");
	shootSoundVector[1].loadFromFile("../sound/laser5.wav");
	shootSoundVector[3].loadFromFile("../sound/space laser.wav");
	shootSoundVector[4].loadFromFile("../sound/boss_laser.wav");
	std::list<sf::Sound> SoundVector;
	std::vector<sf::Music*> MusicVector;
	{
		std::vector<std::string> names;
		names.push_back("../sound/Cyberpunk Moonlight Sonata.wav");
		names.push_back("../sound/through space.ogg");
		names.push_back("../sound/high tech lab.flac");
		for (auto it = names.begin(); it != names.end(); it++)
		{
			sf::Music* tmp = new sf::Music;
			tmp->openFromFile(*it);
			tmp->setVolume(5);
			MusicVector.push_back(tmp);
		}
	}
	auto musicIterator = MusicVector.begin();

	//Player
	std::vector<sf::Texture> playerTextures;
	playerProjectilesContainer ppc(&explosionPositions, playerProjectilesColis);
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
	player Player(playerTextures, Vector2(400, 500), vect, playerHpBarTextures, playerColis);
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

	std::vector<sf::Texture> hpBarBossTexture;
	for (int i = 0; i < 19; i++)
	{
		sf::Texture tex;
		tex.loadFromFile("../img/hp_bar_boss.png", sf::IntRect(0, i * 10, 96, 10));
		hpBarBossTexture.push_back(tex);
	}

	//Background
	sf::Texture bgGameTex[10];
	for (int i = 0; i < 10; i++)
		bgGameTex[i].loadFromFile("../img/bg_game" + std::to_string(i + 1) + ".png");

	//Portal
	std::vector<sf::Texture> portalTex;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			sf::Texture a;
			a.loadFromFile("../img/portal.png", sf::IntRect(128 * j, 128 * i, 128, 128));
			portalTex.push_back(a);
		}
	}

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
	temp.loadFromFile("../img/enemy_proc4.png");
	enemyProcTextures.push_back(temp);
	enemyProjectilesContainer epc(enemyProcTextures);
	std::vector<sf::Texture> enemyTextures[6];
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
	tmp.clear();
	for (int i = 0; i < 5; i++)
	{
		sf::Texture tex;
		tex.loadFromFile("../img/enemy5.png", sf::IntRect(i * 65, 0, 65, 75));
		tmp.push_back(tex);
	}
	enemyTextures[4] = tmp;
	tmp.clear();
	for (int i = 0; i < 5; i++)
	{
		sf::Texture tex;
		tex.loadFromFile("../img/enemy6.png", sf::IntRect(i * 95, 0, 95, 76));
		tmp.push_back(tex);
	}
	enemyTextures[5] = tmp;
	sf::Sprite bgGame;
	std::vector<sf::Text*> Tvictory = loadText("../victory.txt", 75, window.getSize().x, 70, sf::Color(0, 0, 0, 255), sf::Color(255, 255, 255, 255), 4, sf::Text::Bold);
	std::vector<sf::Text*> Tfailure = loadText("../failure.txt", 75, window.getSize().x, 70, sf::Color(0, 0, 0, 255), sf::Color(255, 255, 255, 255), 4, sf::Text::Bold);

	(*musicIterator)->play();

	//Boss
	Vector2 bossLastPos = Vector2(-INFINITY, -INFINITY);
	boss* Boss=new boss();
	std::pair<explosion*, explosion*> portal;
	for (int level = 1; level <= levelNum; level++)
	{
		switch (level)
		{
		case 1:
			for (int i = 0; i < 5; i++)
			{
				enemy *e = new enemy(enemyTextures[0], Vector2(50.0f*i, 100.0f), vect, 1, 2, 0, hpBarsTextures[0], enemyColis[0]);
				evect.push_back(e);
			}
			break;
		case 2:
			for (int i = 0; i < 5; i++)
			{
				enemy *e = new enemy(enemyTextures[0], Vector2(100.0f*i, 150.0f), vect, 1, 2, 0, hpBarsTextures[0], enemyColis[0]);
				evect.push_back(e);
			}
			{
				enemy *e = new enemy(enemyTextures[1], Vector2(200.0f, 70.0f), vect, 0, 3, 1, hpBarsTextures[1], enemyColis[1]);
				evect.push_back(e);
				enemy *e2 = new enemy(enemyTextures[1], Vector2(565.0f, 70.0f), vect, 0, 3, 1, hpBarsTextures[1], enemyColis[1]);
				evect.push_back(e2);
			}
			break;
		case 3:
			for (int i = 0; i < 3; i++)
			{

				enemy* e = new enemy(enemyTextures[1], Vector2(272.5f + 85.0f*i, 70.0f), vect, 2, 3, 1, hpBarsTextures[1], enemyColis[1]);
				evect.push_back(e);
			}
			for (int i = 0; i < 2; i++)
			{
				enemy* e2 = new enemy(enemyTextures[0], Vector2(100 + 100.0f*i, 150.0f), vect, -1, 2, 0, hpBarsTextures[0], enemyColis[0]);
				evect.push_back(e2);
				enemy* e3 = new enemy(enemyTextures[0], Vector2(565 + 100.0f*i, 150.0f), vect, 1, 2, 0, hpBarsTextures[0], enemyColis[0]);
				evect.push_back(e3);
			}
			break;
		case 4:
			for (int i = 0; i < 5; i++)
			{
				enemy* e = new enemy(enemyTextures[2], Vector2(62.5f + 40.0f + 135.0f*i, 50.0f), vect, 6, 4, 0, hpBarsTextures[2], enemyColis[2]);
				evect.push_back(e);
			}
			for (int i = 0; i < 3; i++)
			{

				enemy* e = new enemy(enemyTextures[1], Vector2(272.5f + 85.0f*i, 70.0f), vect, 2, 3, 1, hpBarsTextures[1], enemyColis[1]);
				evect.push_back(e);
			}
			break;
		case 5:
		{
			for (int i = 0; i < 5; i++)
			{
				enemy* e = new enemy(enemyTextures[2], Vector2(62.5f + 40.0f + 135.0f*i, 50.0f), vect, 5, 4, 0, hpBarsTextures[2], enemyColis[2]);
				evect.push_back(e);
			}
			for (int i = 0; i < 2; i++)
			{
				enemy* e2 = new enemy(enemyTextures[0], Vector2(100 + 100.0f*i, 150.0f), vect, -1, 2, 0, hpBarsTextures[0], enemyColis[0]);
				evect.push_back(e2);
				enemy* e3 = new enemy(enemyTextures[0], Vector2(565 + 100.0f*i, 150.0f), vect, 1, 2, 0, hpBarsTextures[0], enemyColis[0]);
				evect.push_back(e3);
			}
			break;
		}
		case 6:
		{
			for (int i = 0; i < 5; i++)
			{
				enemy* e = new enemy(enemyTextures[2], Vector2(62.5f + 40.0f + 135.0f*i, 50.0f), vect, 7, 4, 0, hpBarsTextures[2], enemyColis[2]);
				evect.push_back(e);
			}
			for (int i = 0; i < 5; i++)
			{
				enemy *e = new enemy(enemyTextures[0], Vector2(50.0f*i, 100.0f), vect, 1, 2, 0, hpBarsTextures[0], enemyColis[0]);
				evect.push_back(e);
			}
			break;
		}
		case 7:
		{
			enemy* e = new enemy(enemyTextures[3], Vector2(370.5f, 50.0f), vect, 4, 5, 3, hpBarsTextures[3], enemyColis[3]);
			evect.push_back(e);
		}
		break;
		case 8:
		{
			enemy* e = new enemy(enemyTextures[4], Vector2(370.5f, 50.0f), vect, 8, 5, 3, hpBarsTextures[3], enemyColis[4]);
			evect.push_back(e);
		}
		break;
		case 9:
		{
			for (int i = 0; i < 6; i++)
			{
				enemy* e2 = new enemy(enemyTextures[0], Vector2(100 + 50.0f*i, 75.0f), vect, -1, 2, 0, hpBarsTextures[0], enemyColis[0]);
				evect.push_back(e2);
				enemy* e3 = new enemy(enemyTextures[0], Vector2(400 + 50.0f*i, 75.0f), vect, 1, 2, 0, hpBarsTextures[0], enemyColis[0]);
				evect.push_back(e3);
			}
		}
		break;
		case 10:
		{
			Boss = new boss(enemyTextures[5], Vector2(370.5f, 50.0f), vect, hpBarBossTexture, enemyColis[5], &bossLastPos);;
			evect.push_back(Boss);
		}
		break;
		}

		bgGame.setTexture(bgGameTex[level - 1]);
		epc.clear();
		ppc.clear();
		explosionPositions.clear();
		for (auto it = explosions.begin(); it != explosions.end(); it++)
			delete *it;
		explosions.clear();

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

			if (level == 10)	//Boss' logic
			{
				if (evect.front() != Boss)
				{
					for (auto it = evect.begin(); it != evect.end(); it++)
						delete *it;
					evect.clear();
				}
				if (Boss->flags & 1)
				{
					enemy* e2 = new enemy(enemyTextures[0], Vector2(150.0f, 85.0f), vect, -1, 2, 0, hpBarsTextures[0], enemyColis[0]);
					evect.push_back(e2);
					enemy* e3 = new enemy(enemyTextures[0], Vector2(612.0f, 85.0f), vect, 1, 2, 0, hpBarsTextures[0], enemyColis[0]);
					evect.push_back(e3);
					Boss->flags &= ~1;
					explosion* e = new explosion(Vector2(100.0f, 25.0f), jumpTex, vect);
					explosions.push_back(e);
					explosion* e1 = new explosion(Vector2(572.0f, 25.0f), jumpTex, vect);
					explosions.push_back(e1);
					SoundVector.push_back(sf::Sound(jumpSound));
					SoundVector.back().setVolume(25);
					SoundVector.back().play();
				}
				if (Boss->flags & 2)
				{
					if (Boss->portalState == 0)
					{
						portal.first = new explosion(Vector2(-50.0f, 30.0f), portalTex, vect);
						portal.second = new explosion(Vector2(860.0f, -103.0f), portalTex, vect);
						portal.first->sprite.setRotation(270.0f);
						portal.second->sprite.setRotation(90.0f);
						explosion* e = new explosion(Vector2(-20.0f, -90.0f), jumpTex, vect);
						explosions.push_back(e);
						explosion* e1 = new explosion(Vector2(692.0f, -90.0f), jumpTex, vect);
						explosions.push_back(e1);
						SoundVector.push_back(sf::Sound(jumpSound));
						SoundVector.back().setVolume(25);
						SoundVector.back().play();
						Boss->portalState = 1;
					}
					if (Boss->portalState == 1)
					{
						portal.first->move(Vector2(0.0f, 3.0f));
						portal.second->move(Vector2(0.0f, 3.0f));
						portal.first->update();
						portal.second->update();
						if (Player.pos.y < portal.first->_pos.y - 90 && Player.pos.y > portal.first->_pos.y - 140)
							Boss->portalState = 2;
					}
					if (Boss->portalState > 1 && Boss->portalState < 10)
					{
						portal.first->update();
						portal.second->update();
						Boss->portalState++;
					}
					if (Boss->portalState == 10)
					{
						SoundVector.push_back(sf::Sound(shootSoundVector[3]));
						SoundVector.back().setVolume(12);
						SoundVector.back().play();
						epc.addProjectile(epc.textures[2], Vector2(portal.first->_pos.x - 16.5f, portal.first->_pos.y - 120.0f), Vector2(15.0f, 0.0f), enemyProjectileColis[2]);
						epc.addProjectile(epc.textures[2], Vector2(portal.second->_pos.x, portal.second->_pos.y + 10.0f), Vector2(-15.0f, 0.0f), enemyProjectileColis[2]);
						Boss->portalState = 11;
					}
					if (Boss->portalState > 10 && Boss->portalState < 80)
					{
						portal.first->update();
						portal.second->update();
						Boss->portalState++;
					}
					if (Boss->portalState == 80)
					{
						delete portal.first;
						delete portal.second;
						Boss->flags &= ~2;
						Boss->portalState = -1;
					}
				}
			}

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
				SoundVector.push_back(sf::Sound(shootSoundVector[0]));
				SoundVector.back().setVolume(12);
				SoundVector.back().play();
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
				(*musicIterator)->stop();
				for (auto it = Tfailure.begin(); it != Tfailure.end(); it++)
					delete *it;
				for (auto it = Tvictory.begin(); it != Tvictory.end(); it++)
					delete *it;
				for (auto it = evect.begin(); it != evect.end(); it++)
					delete *it;
				explosionPositions.clear();
				for (auto it = MusicVector.begin(); it != MusicVector.end(); it++)
					delete *it;
				for (auto it = explosions.begin(); it != explosions.end(); it++)
					delete *it;
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
					SoundVector.push_back(sf::Sound(shootSoundVector[(*it)->procType]));
					SoundVector.back().setVolume(12);
					SoundVector.back().play();
					switch ((*it)->procType)
					{
					case 0:
						epc.addProjectile(epc.textures[0], Vector2((*it)->pos.x + 12, (*it)->pos.y), Vector2(0.0f, 7.0f), enemyProjectileColis[0]);
						break;
					case 1:
						epc.addProjectile(epc.textures[1], Vector2((*it)->pos.x + 12, (*it)->pos.y), Vector2(0.0f, 9.0f), enemyProjectileColis[1]);
						break;
						//gdzie jest 2
					case 3:
						epc.addProjectile(epc.textures[2], Vector2((*it)->pos.x - 16.5f, (*it)->pos.y), Vector2(0.0f, 9.0f), enemyProjectileColis[2]);
						epc.addProjectile(epc.textures[2], Vector2((*it)->pos.x + 13.5f, (*it)->pos.y), Vector2(0.0f, 9.0f), enemyProjectileColis[2]);
						break;
					}
				}
			}
			while (!explosionPositions.empty())
			{
				std::vector<Vector2>::iterator it = explosionPositions.begin();
				explosion* e = new explosion(*it, exp1, vect);
				SoundVector.push_back(sf::Sound(bigBoom));
				SoundVector.back().setVolume(50);
				SoundVector.back().play();
				explosionPositions.erase(it);
				explosions.push_back(e);
			}
			for (auto it = explosions.begin(); it != explosions.end();)
			{
				if (!(*it)->update())
				{
					delete *it;
					if (explosions.size() == 1)
					{
						explosions.clear();
						break;
					}
					it = explosions.erase(it);
				}
				else
					it++;
			}
			epc.update(window, Player);
			if (Player.health < 1)
			{
				(*musicIterator)->stop();
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
				explosionPositions.clear();
				for (auto it = MusicVector.begin(); it != MusicVector.end(); it++)
					delete *it;
				for (auto it = explosions.begin(); it != explosions.end(); it++)
					delete *it;
				return 0;
			}
			SoundVector.remove_if([](sf::Sound &sound) {return sound.getStatus() == sf::Sound::Stopped; });
			for (std::vector<sf::Drawable*>::iterator it = vect.begin(); it != vect.end(); it++)
				window.draw(**it);
			window.display();
			if ((*musicIterator)->getStatus() == sf::Music::Stopped)
			{
				if (++musicIterator == MusicVector.end())
					musicIterator = MusicVector.begin();
				(*musicIterator)->play();
			}
			while (clock.getElapsedTime().asMilliseconds() < 1000 / fps);	//Fps limiter
		}
		if (level != 10)
		{
			Player._bar->visible(false);
			explosion* jump;
			auto bounds = Player.sprite.getGlobalBounds();
			Vector2 x = Vector2(Player.pos.x + bounds.width / 2 - 64, Player.pos.y + bounds.height / 2 - 100);
			jump = new explosion(x, jumpTex, vect);
			explosions.push_back(jump);
			Player.visible(false);
			SoundVector.push_back(sf::Sound(jumpSound));
			SoundVector.back().setVolume(25);
			SoundVector.back().play();
			while (!explosions.empty())
			{
				clock.restart();
				window.clear();
				window.draw(bgGame);
				for (auto it = explosions.begin(); it != explosions.end(); it++)
				{
					if (!(*it)->update())
					{
						delete *it;
						if (explosions.size() == 1)
						{
							explosions.clear();
							break;
						}
						it = explosions.erase(it);
					}
				}
				for (std::vector<sf::Drawable*>::iterator it = vect.begin(); it != vect.end(); it++)
					window.draw(**it);
				window.display();
				while (clock.getElapsedTime().asMilliseconds() < 1000 / 45);
			}
			Player.visible(true);
			Player._bar->visible(true);
		}
		else
		{
			explosion* e = new explosion(Vector2(bossLastPos.x + 10, bossLastPos.y + 10), exp1, vect);
			SoundVector.push_back(sf::Sound(bigBoom));
			SoundVector.back().setVolume(50);
			SoundVector.back().play();
			explosions.push_back(e);
			int uglyTimer = 0;
			while (!explosions.empty())
			{
				switch (uglyTimer++)
				{
				case 5:
				{
					explosion* e = new explosion(Vector2(bossLastPos.x + 20, bossLastPos.y + 10), exp1, vect);
					SoundVector.push_back(sf::Sound(bigBoom));
					SoundVector.back().setVolume(50);
					SoundVector.back().play();
					explosions.push_back(e);
				}
				break;
				case 10:
				{
					explosion* e = new explosion(Vector2(bossLastPos.x + 10, bossLastPos.y + 7), exp1, vect);
					SoundVector.push_back(sf::Sound(bigBoom));
					SoundVector.back().setVolume(50);
					SoundVector.back().play();
					explosions.push_back(e);
				}
				break;
				case 15:
				{
					explosion* e = new explosion(Vector2(bossLastPos.x, bossLastPos.y + 45), exp1, vect);
					SoundVector.push_back(sf::Sound(bigBoom));
					SoundVector.back().setVolume(50);
					SoundVector.back().play();
					explosions.push_back(e);
				}
				break;
				case 20:
				{
					explosion* e = new explosion(Vector2(bossLastPos.x + 40, bossLastPos.y), exp1, vect);
					SoundVector.push_back(sf::Sound(bigBoom));
					SoundVector.back().setVolume(50);
					SoundVector.back().play();
					explosions.push_back(e);
				}
				break;
				}
				clock.restart();
				window.clear();
				window.draw(bgGame);
				for (auto it = explosions.begin(); it != explosions.end(); it++)
				{
					if (!(*it)->update())
					{
						delete *it;
						if (explosions.size() == 1)
						{
							explosions.clear();
							break;
						}
						it = explosions.erase(it);
					}
				}
				for (std::vector<sf::Drawable*>::iterator it = vect.begin(); it != vect.end(); it++)
					window.draw(**it);
				window.display();
				while (clock.getElapsedTime().asMilliseconds() < 1000 / 45);
			}
		}
	}
	bgGame.setTexture(bgGameTex[levelNum - 1]);
	(*musicIterator)->stop();
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
	for (auto it = MusicVector.begin(); it != MusicVector.end(); it++)
		delete *it;
	return 1;
}

int main()
{
	sf::Music music;
	music.openFromFile("../sound/ObservingTheStar.ogg");
	music.setVolume(12);
	music.setLoop(1);
	font.loadFromFile("../arial.ttf");
	int state = 0;								// 0 - menu, 1 - gra, 2 - credits, -1 błąd, -2 zamknij
	sf::RenderWindow window(sf::VideoMode(800, 600), gameName);
	srand(time(NULL));
	music.play();
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
			music.stop();
			state = game(window);
			music.play();
			break;
		}
	}
	return 0;
}
