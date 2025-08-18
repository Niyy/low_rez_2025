#include "map.hpp"
#include <iostream>

bool low_rez::Map::place(Object obj)
{
    int h_index = 0,
        w_index = 0;
    std::array<int, 2> start_id = {(int)(obj.rect().x), (int)(obj.rect().y)};

    if(_objects.find(start_id) == _objects.end())
    {
        bool hit = false;

        for(h_index = 0; h_index < obj.rect().h; h_index++)
        {
            for(w_index = 0; w_index < obj.rect().w; w_index++)
            {
                std::array<int, 2> id = {(int)(obj.rect().x + w_index), (int)(obj.rect().y + h_index)};

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
                for(w_index = 0; w_index < obj.rect().w; w_index++)
                {
                    std::array<int, 2> id = {(int)(obj.rect().x + w_index), (int)(obj.rect().y + h_index)};
                    int another_obj_id = _objects.find(id)->second.get_id();

                    if(another_obj_id != obj.get_id())
                    { 
                        continue; 
                    }

                    std::cout << "hey" << obj.rect().x + w_index << "," << obj.rect().y + h_index << std::endl;
                    std::cout << "deleted" << _objects.erase(id) << std::endl;
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
    std::array<int, 2> id = {(int)(obj.rect().x), (int)(obj.rect().y)};

    if(_objects.find(id) != _objects.end())
    {
        for(int h_index = 0; h_index < obj.rect().h; h_index++)
        {
            for(int w_index = 0; w_index < obj.rect().w; w_index++)
            {
                std::array<int, 2> erase_id = {(int)(obj.rect().x + w_index), (int)(obj.rect().y + h_index)};

                _objects.erase(erase_id);
            }
        }

        return true;
    }

    return false;
}


std::map<std::array<int, 2>, Object> low_rez::Map::get_objects()
{
    return _objects;
}


//Object* low_rez::Map::query(SDL_FRect start, low_rez::Type what)
//{
//    std::vector<array<int, 2>> contents;
//
//    if((contents =_map_bound_groups[what]) != contents.end())
//    {
//        return &_objects[contents];
//    }
//
//    return nullptr;
//}
