#include "projectiles.h"

projectile::projectile(sf::Texture tex, Vector2 pos, Vector2 speed,std::vector<std::pair<Vector2, Vector2>>& colis) : _texture(tex), _pos(pos), _speed(speed)
{
	colisionlines = colis;
	setBB();
	_sprite.setTexture(_texture);
	_sprite.setPosition(_pos);
}

void projectile::update()
{
	_pos += _speed;
	_sprite.setPosition(_pos);
	updateBB();
}

Vector2* projectile::getPos()
{
	return &_pos;
}

void projectile::setRotation(float ang)
{
	rotation = ang;
	float xo = _sprite.getLocalBounds().width / 2, yo=_sprite.getLocalBounds().height / 2;
	_sprite.setOrigin(xo, yo);
	_sprite.setRotation(ang * 45.0f / atan(1));
	_sprite.setOrigin(0, 0);
	for (auto it = colisionlines.begin(); it != colisionlines.end(); it++)
	{
		it->first = Vector2(cos(ang)*(it->first.x - xo) + (it->first.y - yo)*sin(ang) + xo, cos(ang)*(it->first.y - yo) - sin(ang)*(it->first.x - xo) + yo);
		it->second = Vector2(cos(ang)*(it->second.x - xo) + (it->second.y - yo)*sin(ang) + xo, cos(ang)*(it->second.y - yo) - sin(ang)*(it->second.x - xo) + yo);
	}
}
void projectile::rotate(float ang)
{
	rotation += ang;
	setRotation(rotation);
}
playerProjectilesContainer::playerProjectilesContainer(std::vector <Vector2>* expPos, std::vector<std::pair<Vector2, Vector2>>& _colis) : _expPos(expPos),colis(_colis)
{

}

void playerProjectilesContainer::addProjectile(Vector2 pos)
{
	pvect.push_back(new projectile(texture, pos, speed,colis));
}

void playerProjectilesContainer::clear()
{
	for (auto it = pvect.begin(); it != pvect.end(); it++)
		delete *it;
	pvect.clear();
}

void playerProjectilesContainer::update(sf::RenderWindow& window, std::list<enemy*>& evect)
{
	int x = 0;	//czemu tu?
	for (auto it = pvect.begin(); it != pvect.end();)
	{
		x = 0;	//skoro mo�e by� tu?
		(*it)->update();
		for (auto it2 = evect.begin(); it2 != evect.end();)
		{
			if (testColision(*it,*it2)) //Collision::PixelPerfectTest((*it)->_sprite, (*it2)->sprite, 128)
			{
				(*it2)->takeDamage(1);
				if ((*it2)->health == 0)
				{
					auto bounds = (*it2)->sprite.getGlobalBounds();
					_expPos->push_back(Vector2((*it2)->pos.x + bounds.width / 2 - 44.5f, (*it2)->pos.y + bounds.height / 2 - 44.5f));
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

enemyProjectilesContainer::enemyProjectilesContainer(std::vector<sf::Texture> &tex) : textures(tex)
{

}
void enemyProjectilesContainer::addProjectile(projectile * pointer)
{
	pvect.push_back(pointer);
}
void enemyProjectilesContainer::clear()
{
	for (auto it = pvect.begin(); it != pvect.end(); it++)
		delete *it;
	pvect.clear();
}

void enemyProjectilesContainer::addProjectile(sf::Texture tex, Vector2 pos, Vector2 speed, std::vector<std::pair<Vector2, Vector2>>& colis)
{
	pvect.push_back(new projectile(tex,pos,speed,colis));
}

void enemyProjectilesContainer::update(sf::RenderWindow& window, player& Player)
{
	for (auto it = pvect.begin(); it != pvect.end();)
	{
		(*it)->update();
		if (testColision(&Player,*it)) //Collision::PixelPerfectTest((*it)->_sprite, Player.sprite, 128)
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