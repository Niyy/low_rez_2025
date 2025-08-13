#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>


class Object
{
    public:
        Object(SDL_FRect in_rect, const char* in_img_path, SDL_Renderer* in_renderer) :
            _rect(in_rect),
            _texture(IMG_LoadTexture(in_renderer, in_img_path)) {};
        ~Object();

        SDL_FRect get_rect();
        SDL_Texture* get_texture();
    
    private:
        SDL_FRect _rect; // location and dimensions
        SDL_Texture *_texture; // essentially the sprite
};
