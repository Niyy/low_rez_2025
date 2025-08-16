#include "map.hpp"
#include <iostream>

bool low_rez::Map::place(Object obj)
{
    int h_index = 0,
        w_index = 0;
    std::tuple<int, int> id(obj.rect().x, obj.rect().y);

    if(_objects.find(id) == _objects.end())
    {
        bool hit = false;

        for(h_index = 0; h_index < obj.rect().h; h_index++)
        {
            for(w_index = 0; w_index < obj.rect().w; w_index++)
            {
                std::tuple<int, int> id(obj.rect().x + w_index, obj.rect().y + h_index);                

                if(_objects.find(id) != _objects.end())
                {
                    hit = true;
                    break;
                }

                _objects[id] = obj;
            }
        }

        if(hit)
        {
            for(; h_index >= 0; h_index--)
            {
                for(; w_index > 0; w_index--)
                {
                    std::tuple<int, int> id(obj.rect().x + w_index, obj.rect().y + h_index);
                    _objects.erase(id);
                }
            }

            return false;
        }

        return true;
    }

    return false;
}


bool low_rez::Map::remove(Object obj)
{
    std::tuple<int, int> id(obj.rect().x, obj.rect().y);

    if(_objects.find(id) != _objects.end())
    {
        for(int h_index = 0; h_index < obj.rect().h; h_index++)
        {
            for(int w_index = 0; w_index < obj.rect().w; w_index++)
            {
                _objects.erase(std::tuple<int, int>(obj.rect().x + w_index, obj.rect().y + h_index));
            }
        }

        return true;
    }

    return false;
}


std::map<std::tuple<int, int>, Object>& low_rez::Map::objects()
{
    return _objects;
}
