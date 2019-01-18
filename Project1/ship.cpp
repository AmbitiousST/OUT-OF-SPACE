#include "ship.h"

hpBar::hpBar(std::vector<sf::Texture> &tex, std::vector<sf::Drawable*>& vect, ship* ship) : _textures(tex), _ship(ship)
{
	update();
	vect.push_back(&_sprite);
}

void hpBar::update()
{
	sf::FloatRect b = _ship->_sprite.getGlobalBounds();
	_pos.x = _ship->pos.x+((b.width-26)/2);		//hpBar width = 26
	_pos.y = _ship->pos.y + 50;
	_sprite.setPosition(_pos);
	_sprite.setTexture(_textures[5-(_ship->health)]);	//Potencjalne wyj¹tki
}

ship::ship(std::vector<sf::Texture> &tex, Vector2 p, std::vector<sf::Drawable*>& vect) : _textures(tex), pos(p)
{
	_sprite.setTexture(_textures[0]);
	speed = Vector2(0, 0);
	_sprite.setPosition(p);
	vect.push_back(&_sprite);
	_it = _textures.begin();
	collision = Vector2i(0, 0);
}

void ship::visible(bool a)
{
	_sprite.setColor(sf::Color(255, 255, 255, a ? 255 : 0));
}

void ship::update()		//fizyka statków
{
	collision = Vector2i(0, 0);
	sf::FloatRect rect = _sprite.getGlobalBounds();
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
	_sprite.setTexture(*_it);
	if (++_it == _textures.end())
		_it = _textures.begin();
	_sprite.setPosition(pos);
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

player::player(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect) : ship(tex, pos, vect)
{
	health = 5;
}

enemy::enemy(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect, int aiType) : ship(tex, pos, vect)
{
	//ca³y ten blok chyba da siê zrobiæ ³adniej, ale nie wiem jak
	_dist = 0;
	_side = 1;
	shot = 0;
	_aiType = aiType;
	_baseSpeed = 1;
}

void enemy::move()		//ai przeciwnika
{
	switch (_aiType)
	{
	case(1):
		if (collision.x == 1 && _side == 1)//(pos.x + 70 >= 800 && _side == 1) zostawi³em twój kod gdybyœ chcia³
		{
			_side = 0;
		}

		if (collision.x == -1 && _side == -1)//(	pos.x-50<0 && _side == -1)
		{
			_side = 0;
		}

		if (_dist > 50)//Co to ma na celu?
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