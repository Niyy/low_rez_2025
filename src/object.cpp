#include "object.hpp"


unsigned int Object::id_file = 0;


Object::Object(int i_x, int i_y)
{
    _id = id_file++;
    x(i_x);
    y(i_y);
    _texture = nullptr;
}


string Object::type()
{
    return _type;
}


int Object::x()
{
    return (int)_rect.x;
}


int Object::x(int i_x)
{
    return _rect.x = i_x;
}


int Object::y()
{
    return (int)_rect.y;
}


int Object::y(int i_y)
{
    return _rect.y = i_y;
}


std::array<int, 2> Object::get_pos()
{
    std::array<int, 2> pos = {_rect.x, _rect.y};
    return pos;
}


std::array<int, 2> Object::set_pos(std::array<int, 2> new_pos)
{
    _rect.x = new_pos[0];
    _rect.y = new_pos[1];

    return new_pos;
}


int Object::get_id()
{
    return _id;
}


const SDL_FRect& Object::rect() const
{
    return _rect;
}


SDL_FRect& Object::rect(SDL_FRect in_rect)
{
    _rect = in_rect;
    return _rect;
}


SDL_Texture* Object::get_texture()
{
    return _texture;
}


void Object::set_texture(SDL_Texture* in_texture)
{
    _texture = in_texture;
}


void Object::move()
{
    std::array<int, 2> return_pos {_rect.x, _rect.y};
    return;
}
