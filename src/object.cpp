#include "object.hpp"


SDL_FRect Object::get_rect()
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
