#include "actor.hpp"


void Actor::set_path(stack<array<int, 2>> i_path)
{
    _path = i_path;
}


void Actor::move()
{
    array<int, 2> next_point = _path.top();
    _rect.x = next_point[0];
    _rect.y = next_point[1];
    _path.pop();
}
