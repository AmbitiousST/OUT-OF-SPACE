#include "ship.h"

ship::ship(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect) : _textures(tex), _pos(pos)
{
	_sprite.setTexture(_textures[0]);
	_speed = Vector2(0, 0);
	_sprite.setPosition(_pos);
	vect.push_back(&_sprite);
	_it = _textures.begin();
	_collision = Vector2i(0, 0);
}

void ship::render(bool a)
{
	_sprite.setColor(sf::Color(255, 255, 255, a ? 255 : 0));
}

void ship::update()
{
	_collision = Vector2i(0, 0);
	sf::FloatRect rect = _sprite.getGlobalBounds();
	_pos += _speed;
	if (_pos.x + rect.width >= 800)
		_pos.x = 800 - rect.width, _collision.x = 1;
	if (_pos.x <= 0)
		_pos.x = 0, _collision.x = -1;
	if (_pos.y + rect.height >= 600)
		_pos.y = 600 - rect.height, _collision.y = 1;
	if (_pos.y <= 0)
		_pos.y = 0, _collision.y = -1;
	_speed = Vector2(0, 0);
	_sprite.setTexture(*_it);
	if (++_it == _textures.end())
		_it = _textures.begin();
	_sprite.setPosition(_pos);
}

void ship::changeSpeed(Vector2 s)
{
	_speed.x += s.x;
	_speed.y += s.y;
}

player::player(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect) : ship(tex, pos, vect)
{

}

enemy::enemy(std::vector<sf::Texture> &tex, Vector2 pos, std::vector<sf::Drawable*>& vect, int aiType) : ship(tex, pos, vect)
{
	_dist = 0;
	_side = 1;
	shot = 0;
	_aiType = aiType;	//ca³y ten blok chyba da siê zrobiæ ³adniej, ale nie wiem jak
}
void enemy::move() 
{
	switch (_aiType)
	{
	case(1):
		if (_collision.x == 1 && _side == 1)//(_pos.x + 70 >= 800 && _side == 1)
		{
			_side = 0;
		}

		if (_collision.x == -1 && _side == -1)//(	_pos.x-50<0 && _side == -1)
		{
			_side = 0;
		}

		if (_dist > 50)						//Co to ma na celu?
		{
			_dist = 0;
			if (_pos.x < 400)
				_side = 1;
			else
				_side = -1;
		}
		switch (_side)
		{
		case 0:
			changeSpeed(Vector2(0, 1));
			_dist += 1;
			break;
		case 1:
			changeSpeed(Vector2(1, 0));
			break;
		case -1:
			changeSpeed(Vector2(-1, 0));
			break;
		default:
			break;
		}
		break;

	default:
		if (_collision.x == 1)
		{
			changeSpeed(Vector2(_speed.x*-1, 0));
		}

		if (_collision.x == -1)
		{
		changeSpeed(Vector2(_speed.x*-1, 0));
		}
		break;
	}
}