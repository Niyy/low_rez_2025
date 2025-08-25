#include "actor.hpp"


array<int, 2> Actor::pop_path()
{
    array<int, 2> return_val = _path.top();
    _path.pop();

    return return_val;
}


bool Actor::path_empty()
{
    return _path.empty();
}


void Actor::set_path(stack<array<int, 2>> i_path)
{
    _path = i_path;
}


void Actor::move()
{
    if(path_empty()) 
    {
        return;
    }

    array<int, 2> next_pos = pop_path();
    _rect.x = next_pos[0];
    _rect.y = next_pos[1];
    
    std::cout << "hello actor" << std::endl;

    return;
}
