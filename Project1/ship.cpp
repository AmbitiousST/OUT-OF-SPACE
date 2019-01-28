#include "ship.h"
#include <iostream>
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
	_sprite.setTexture(_textures[_maxHp - (hp)]);	//Potencjalne wyj¹tki
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
void hpBar::visible(bool a)
{
	_sprite.setColor(sf::Color(255, 255, 255, a ? 255 : 0));
}


ship::ship(std::vector<sf::Texture> &tex, Vector2 p, std::vector<sf::Drawable*>& vect,std::vector<std::pair<Vector2,Vector2>>& colis) : _textures(tex), pos(p)
{
	colisionlines = colis;
	setBB();
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

void ship::update()		//fizyka statków
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
	updateBB();
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
Vector2* ship::getPos()
{
	return &pos;
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
player::player(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect, std::vector<sf::Texture> &barTex, std::vector<std::pair<Vector2, Vector2>>& colis) : ship(tex, pos, vect,colis)
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
	int aiType, int hp, int type, std::vector<sf::Texture> &barTex, std::vector<std::pair<Vector2, Vector2>>& colis) : ship(tex, pos, vect,colis)
	//okropnie du¿o tych argumentów. Chyba przesadzi³em
{
	//ca³y ten blok chyba da siê zrobiæ ³adniej, ale nie wiem jak

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
	_aiVar = 0;
	_aiVar2 = -2;
	_bar = new hpBar(barTex, vect, health, Vector2(pos.x, pos.y - 10));
}

void enemy::update()
{
	ship::update();
	_bar->update(Vector2(pos.x + (sprite.getGlobalBounds().width - _bar->width) / 2, pos.y - 10), health);
}

enemy::~enemy()			//Dok³adzie to samo, co destruktor gracza
{
	ship::~ship();
	delete _bar;
}

void enemy::move()		//ai przeciwnika
{
	shot += rand() % 3 + 1;
	if (shot >= 100)
		shot = 1;
	switch (_aiType)
	{
	case(1):
		//orginal
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
		//static
		changeSpeed(Vector2(0, 0));
		break;

	case 3:
		//sinuœ
		if (collision.x == 1 && _side == 1)
		{
			_side = -1;
		}
		if (collision.x == -1 && _side == -1)
		{
			_side = 1;
		}
		changeSpeed(Vector2(_baseSpeed*_side, sin(_aiVar)));
		_aiVar += 0.01f;
		break;
	case 4:
		//left to right sin speed
		if (collision.x == 1 && _side == 1)
		{
			_side = -1;
		}
		if (collision.x == -1 && _side == -1)
		{
			_side = 1;
		}
		changeSpeed(Vector2(fabs(sin(_aiVar))*_side*1.5f, cos(_aiVar)));
		_aiVar += 0.01f;
		break;
	case 5:
		//circle
		changeSpeed(Vector2(cos(_aiVar), sin(_aiVar)));
		_aiVar += 0.01f;
		break;
	case 6:
		//8
		changeSpeed(Vector2(cos(_aiVar), cos(_aiVar*2)));
		_aiVar += 0.01f;
		break;
	case 7:
		//diagonal variable speed
		changeSpeed(Vector2(cos(_aiVar), cos(_aiVar)));
		_aiVar += 0.02f;
		break;
	case 8:
		//polynomial
		if (collision.x == 1 && _side == 1)
		{
			_side = -1;
		}
		if (collision.x == -1 && _side == -1)
		{
			_side = 1;
		}
		changeSpeed(Vector2((_aiVar + 2)*(_aiVar + 1)*(_aiVar - 0.5)*(_aiVar - 1.5)*_side, cos(_aiVar2)));
		_aiVar += 0.01f;
		_aiVar2 += 0.01f;
		if (_aiVar > 1.9)
			_aiVar = -2.3f;
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

explosion::explosion(Vector2 p, std::vector <sf::Texture>& tex, std::vector<sf::Drawable*>& vect) : _pos(p), _textures(tex)
{
	_it = _textures.begin();
	sprite.setTexture(*_it);
	sprite.setPosition(_pos);
	vect.push_back(&sprite);
	_vectPtr = &vect;
}

bool explosion::update()
{
	sprite.setTexture(*_it);
	_it++;
	if (_it == _textures.end())
	{
		_it = _textures.begin();
		return false;
	}
	return true;
}

explosion::~explosion()
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

void boss::update()
{
	ship::update();
	_bar->update(Vector2(pos.x + (sprite.getGlobalBounds().width - _bar->width) / 2, pos.y + 50), health);
}

boss::boss(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect, std::vector<sf::Texture> &barTex, std::vector<std::pair<Vector2, Vector2>>& colis) : enemy(tex, pos, vect,0,19,0,barTex,colis)
{
}
void boss::move() 
{
	static int dist = 0;
	switch (_aiType)
	{
	case 0:
		goTo(Vector2(0.05f, 75), 2.0f);
		if (collision.x==-1)
		{
			_aiType = 1;
			enemy::procType = 3;
		}
		break;
	case 1:
		changeSpeed(Vector2(4.0f,0));
		shot += 1;
		shot %= 25;
		if (collision.x == 1)
		{
			_aiType = 2;
			enemy::procType = 0;
		}
		break;
	case 2:
		goTo(Vector2(370.5f, 50.0f),2.0f);
		if (fabs(pos.x - 370.5f) < 1.0f && fabs(pos.y - 50.0f) < 1.0f)
		{
			_aiType = 3;
			flags |= 1;
		}
		break;
	case 3:
		if (!(flags & 1))
			_aiType = 0;
		break;
	case 4:
		goTo(Vector2(370.5f, 50.0f), 2.0f);
		if (fabs(pos.x - 370.5f) < 1.0f && fabs(pos.y - 50.0f) < 1.0f)
		{
			_aiType = 3;
			flags |= 2;
		}
		break;
	}
}
void boss::goTo(Vector2 to,float speed)
{
	float ang = atan2(pos.y-to.y,pos.x-to.x);
	changeSpeed(Vector2(speed*cos(ang)*-1, speed*sin(ang)*-1));
}