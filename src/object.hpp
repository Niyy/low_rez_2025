#ifndef LOWREZ_OBJECT
#define LOWREZ_OBJECT

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <array>
#include <iostream>

using std::string;

class Object
{
    protected:
        unsigned int _id;
        string _type;
        SDL_FRect _rect; // location and dimensions
        SDL_Texture *_texture; // essentially the sprite
        static unsigned int id_file;

    public:
        Object() :
            _id(id_file++),
            _rect(),
            _texture(nullptr) {};
        Object(SDL_FRect in_rect, SDL_Texture* in_texture) :
            _id(id_file++),
            _rect(in_rect),
            _texture(in_texture) {};
        Object(int x, int y); 

        void update();

        // getter & setters
        string type();
        int x();
        int x(int i_x);
        int y();
        int y(int i_y);
        std::array<int, 2> get_pos();
        std::array<int, 2> set_pos(std::array<int, 2> new_pos);
        int get_id();
        const SDL_FRect& rect() const;
        SDL_FRect& rect(SDL_FRect in_rect);
        SDL_Texture* get_texture();
        void set_texture(SDL_Texture* in_texture);
        virtual void move();
};

#endif
