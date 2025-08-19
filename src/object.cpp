#include "object.hpp"


unsigned int Object::id_file = 0;


Object::Object(int i_x, int i_y)
{
    _id = id_file++;
    x(i_x);
    y(i_y);
    _texture = nullptr;
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


int Object::get_id()
{
    return _id;
}


const SDL_FRect& Object::rect() const
{
    return _rect;
}


void Object::set_rect(SDL_FRect in_rect)
{
    _rect = in_rect;
}


SDL_Texture* Object::get_texture()
{
    return _texture;
}


void Object::set_texture(SDL_Texture* in_texture)
{
    _texture = in_texture;
}
