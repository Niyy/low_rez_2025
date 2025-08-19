#include "map.hpp"
#include <iostream>

bool low_rez::Map::place(Object obj)
{
    int h_index = 0,
        w_index = 0;
     array<int, 2> start_id = {(int)(obj.rect().x), (int)(obj.rect().y)};

    if(_objects.find(start_id) == _objects.end())
    {
        bool hit = false;

        for(h_index = 0; h_index < obj.rect().h; h_index++)
        {
            for(w_index = 0; w_index < obj.rect().w; w_index++)
            {
                array<int, 2> id = {(int)(obj.rect().x + w_index), (int)(obj.rect().y + h_index)};

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
                    array<int, 2> id = {(int)(obj.rect().x + w_index), (int)(obj.rect().y + h_index)};
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
    array<int, 2> id = {(int)(obj.rect().x), (int)(obj.rect().y)};

    if(_objects.find(id) != _objects.end())
    {
        for(int h_index = 0; h_index < obj.rect().h; h_index++)
        {
            for(int w_index = 0; w_index < obj.rect().w; w_index++)
            {
                array<int, 2> erase_id = {(int)(obj.rect().x + w_index), (int)(obj.rect().y + h_index)};

                _objects.erase(erase_id);
            }
        }

        return true;
    }

    return false;
}


std::map<array<int, 2>, Object> low_rez::Map::get_objects()
{
    return _objects;
}


Object* low_rez::Map::query(SDL_FRect start, std::string what)
{
    unsigned int min = 0 - 1;
    Object* found_obj = nullptr;
    std::vector<array<int, 2>> contents;
    std::map<std::string, std::vector<array<int, 2>>>::iterator iter_find;

    if((iter_find = _map_bound_groups.find(what)) == _map_bound_groups.end())
    {
        return nullptr;
    }

    contents = iter_find->second;
    
    for(unsigned int iter = 0; iter < contents.size(); iter++)
    {
        Object cur_obj = _objects[contents[iter]];
        unsigned int x_comp = (start.x - cur_obj.x());
        unsigned int y_comp = (start.y - cur_obj.y());
        unsigned int dist = 0;

        x_comp *= x_comp;
        y_comp *= y_comp;
        dist = x_comp + y_comp;

        if(dist < min)
        {
            found_obj = &cur_obj;
        }
    }

    return found_obj;
}


//std::vector<array<int, 2>>* low_rez::Map::make_path(int bound_x, int bound_y, Object obj, array<int, 2> dst)
//{
//    array<int, 2> start(obj.x(), obj.y());
//    queue<int, 2> queued_tiles; 
//}
