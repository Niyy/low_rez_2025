#ifndef LOWREZ_OBJECT
#define LOWREZ_OBJECT

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>


class Object
{

    protected:
        unsigned int _id;
        SDL_FRect _rect; // location and dimensions
        SDL_Texture *_texture; // essentially the sprite
        static unsigned int id_file;

    public:
        Object() :
            _id(id_file++),
            _rect(),
            _texture(nullptr) {};
        Object(SDL_FRect in_rect, const char* in_img_path, SDL_Renderer* in_renderer) :
            _id(id_file++),
            _rect(in_rect),
            _texture(IMG_LoadTexture(in_renderer, in_img_path)) {};
        Object(SDL_FRect in_rect, SDL_Texture* in_texture) :
            _id(id_file),
            _rect(in_rect),
            _texture(in_texture) {};
        
        // getter & setters
        int get_id();
        const SDL_FRect& rect() const;
        void set_rect(SDL_FRect in_rect);
        SDL_Texture* get_texture();
        void set_texture(SDL_Texture* in_texture);
};

#endif
