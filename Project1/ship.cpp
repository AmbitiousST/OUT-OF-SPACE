#include "ship.h"

hpBar::hpBar(std::vector<sf::Texture> &tex, std::vector<sf::Drawable*>& vect, int hp, Vector2 p) : _textures(tex), _vectPtr(&vect), _maxHp(hp)
{
	update(p, hp);
	vect.push_back(&_sprite);
	width = _sprite.getGlobalBounds().width;
}

void hpBar::update(Vector2 p, int hp)
{
	_pos = p;
	_spritePtr = &_sprite;
	_sprite.setPosition(_pos);
	_sprite.setTexture(_textures[_maxHp - (hp)]);	//Potencjalne wyj�tki
}

hpBar::~hpBar()
{
	for (auto it = _vectPtr->begin(); it != _vectPtr->end(); it++)
	{
		if (*it == _spritePtr)
		{
			_vectPtr->erase(it);
			break;
		}
	}
}

ship::ship(std::vector<sf::Texture> &tex, Vector2 p, std::vector<sf::Drawable*>& vect) : _textures(tex), pos(p)
{
	_vectPtr = &vect;
	sprite.setTexture(_textures[0]);
	speed = Vector2(0, 0);
	sprite.setPosition(p);
	vect.push_back(&sprite);
	_it = _textures.begin();
	collision = Vector2i(0, 0);
}

void ship::visible(bool a)
{
	sprite.setColor(sf::Color(255, 255, 255, a ? 255 : 0));
}

void ship::update()		//fizyka statk�w
{

	collision = Vector2i(0, 0);
	sf::FloatRect rect = sprite.getGlobalBounds();
	pos += speed;
	if (pos.x + rect.width >= 800)
		pos.x = 800 - rect.width, collision.x = 1;
	if (pos.x <= 0)
		pos.x = 0, collision.x = -1;
	if (pos.y + rect.height >= 600)
		pos.y = 600 - rect.height, collision.y = 1;
	if (pos.y <= 0)
		pos.y = 0, collision.y = -1;
	speed = Vector2(0, 0);
	sprite.setTexture(*_it);
	if (++_it == _textures.end())
		_it = _textures.begin();
	sprite.setPosition(pos);
}

void ship::changeSpeed(Vector2 s)
{
	speed.x += s.x;
	speed.y += s.y;
}

void ship::takeDamage(int amount)	//funkcja roku
{
	health -= amount;
	if (health < 0)
		health = 0;
}
ship::~ship()
{
	for (auto it = _vectPtr->begin(); it != _vectPtr->end(); it++)
	{
		if (*it == &sprite)
		{
			_vectPtr->erase(it);
			break;
		}
	}
}
player::player(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect, std::vector<sf::Texture> &barTex) : ship(tex, pos, vect)
{
	health = 5;
	_bar = new hpBar(barTex, vect, health, Vector2(pos.x, pos.y + 50));
}

player::~player()
{
	ship::~ship();
	delete _bar;
}

void player::update()
{
	ship::update();
	_bar->update(Vector2(pos.x + (sprite.getGlobalBounds().width - _bar->width) / 2, pos.y + 50), health);
}

enemy::enemy(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect,
	int aiType, int hp, int type, std::vector<sf::Texture> &barTex) : ship(tex, pos, vect)
	//okropnie du�o tych argument�w. Chyba przesadzi�em
{
	//ca�y ten blok chyba da si� zrobi� �adniej, ale nie wiem jak

	_side = 1;
	_aiType = aiType;
	if (aiType == -1)
	{
		_side = -1;
		_aiType = 1;
	}
	health = hp;
	_dist = 0;
	shot = rand() % 50;
	_baseSpeed = 1;
	procType = type;
	_superSecretVariable = 0;
	_bar = new hpBar(barTex, vect, health, Vector2(pos.x, pos.y - 10));
}

void enemy::update()
{
	ship::update();
	shot += rand() % 3 + 1;
	if (shot >= 100)
		shot = 1;
	_bar->update(Vector2(pos.x + (sprite.getGlobalBounds().width - _bar->width) / 2, pos.y - 10), health);
}

enemy::~enemy()			//Dok�adzie to samo, co destruktor gracza
{
	ship::~ship();
	delete _bar;
}

void enemy::move()		//ai przeciwnika
{
	switch (_aiType)
	{
	case(1):
		if (collision.x == 1 && _side == 1)
		{
			_side = 0;
		}

		if (collision.x == -1 && _side == -1)
		{
			_side = 0;
		}
		if (_dist > 50)
		{
			_dist = 0;
			if (pos.x < 400)
				_side = 1;
			else
				_side = -1;
		}
		switch (_side)
		{
		case 0:
			changeSpeed(Vector2(0, _baseSpeed));
			_dist += 1;
			break;
		case 1:
			changeSpeed(Vector2(_baseSpeed, 0));
			break;
		case -1:
			changeSpeed(Vector2(_baseSpeed*-1, 0));
			break;
		default:
			break;
		}
		break;

	case 2:
		changeSpeed(Vector2(0, 0));
		break;

	case 3:
		if (collision.x == 1 && _side == 1)
		{
			_side = -1;
		}
		if (collision.x == -1 && _side == -1)
		{
			_side = 1;
		}
		changeSpeed(Vector2(_baseSpeed*_side, sin(_superSecretVariable)));
		_superSecretVariable += 0.01f;
		break;

	default:
		if (collision.x == 1)
		{
			changeSpeed(Vector2(_baseSpeed*-1, 0));
		}

		if (collision.x == -1)
		{
			changeSpeed(Vector2(_baseSpeed*-1, 0));
		}
		break;
	}
}