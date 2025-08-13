#include "object.h"


Object::~Object()
{
    SDL_DestroyTexture(_texture);
}


SDL_FRect Object::get_rect()
{
    return _rect;
}


SDL_Texture* Object::get_texture()
{
    return _texture;
}
