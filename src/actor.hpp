#ifndef LOWREZ_ACTOR
#define LOWREZ_ACTOR

#include <vector>
#include <array>
#include <stack>
#include <iostream>
#include "object.hpp"


using std::stack;
using std::array;


class Actor : public Object
{
    protected:
        stack<array<int, 2>> _path;

    public:
        Actor() : Object() {};
        Actor (SDL_FRect in_rect, SDL_Texture* in_texture) :
            Object(in_rect, in_texture) {};
        array<int, 2> pop_path();
        bool path_empty();
        void set_path(stack<array<int, 2>> i_path);
        void move() override;
};


#endif
