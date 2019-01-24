#include "projectiles.h"

projectile::projectile(sf::Texture tex, Vector2 pos, Vector2 speed) : _texture(tex), _pos(pos), _speed(speed)
{
	_sprite.setTexture(_texture);
	_sprite.setPosition(_pos);
}

void projectile::update()
{
	_pos += _speed;
	_sprite.setPosition(_pos);
}

void playerProjectilesContainer::addProjectile(Vector2 pos)
{
	pvect.push_back(new projectile(texture, pos, speed));
}

void playerProjectilesContainer::update(sf::RenderWindow& window, std::list<enemy*>& evect)
{
	int x = 0;	//czemu tu?
	for (auto it = pvect.begin(); it != pvect.end();)
	{
		x = 0;	//skoro mo¿e byæ tu?
		(*it)->update();
		for (auto it2 = evect.begin(); it2 != evect.end();)
		{
			if (Collision::PixelPerfectTest((*it)->_sprite, (*it2)->sprite, 128))
			{
				(*it2)->takeDamage(1);
				if ((*it2)->health == 0)
				{
					delete *it2;
					it2 = evect.erase(it2);
				}
				delete *it;
				it = pvect.erase(it);
				x = 1;
				break;
			}
			else
			{
				it2++;
			}
		}
		if (x)
		{
			if (it == pvect.end())
				break;
		}
		if ((*it)->_pos.y < 0)
		{
			delete *it;
			it = pvect.erase(it);
		}
		else
		{
			window.draw((*it)->_sprite);
			it++;
		}
	}
}

playerProjectilesContainer::~playerProjectilesContainer()
{
	for (auto it = pvect.begin(); it != pvect.end(); it++)
		delete *it;
}

void enemyProjectilesContainer::addProjectile(Vector2 pos)
{
	pvect.push_back(new projectile(texture, pos, speed));
}
void enemyProjectilesContainer::update(sf::RenderWindow& window, player& Player)
{
	for (auto it = pvect.begin(); it != pvect.end();)
	{
		(*it)->update();
		if (Collision::PixelPerfectTest((*it)->_sprite, Player.sprite, 128))
		{
			Player.takeDamage(1);
			delete *it;
			it = pvect.erase(it);
			continue;
		}
		if ((*it)->_pos.y > 600)
		{
			delete *it;
			it = pvect.erase(it);
		}
		else
		{
			window.draw((*it)->_sprite);
			it++;
		}
	}
}
enemyProjectilesContainer::~enemyProjectilesContainer()
{
	for (auto it = pvect.begin(); it != pvect.end(); it++)
		delete *it;
}