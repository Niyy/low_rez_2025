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
        Object(SDL_FRect in_rect, SDL_Texture* in_texture) :
            _id(id_file),
            _rect(in_rect),
            _texture(in_texture) {};
        Object(int x, int y); 
        
        // getter & setters
        int x();
        int x(int i_x);
        int y();
        int y(int i_y);
        int get_id();
        const SDL_FRect& rect() const;
        void set_rect(SDL_FRect in_rect);
        SDL_Texture* get_texture();
        void set_texture(SDL_Texture* in_texture);
};

#endif
