#include "map.hpp"
#include "min_heap.hpp"

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


stack<array<int, 2>> low_rez::Map::make_path(int bound_x, int bound_y, Object obj, array<int, 2> dst)
{
    array<int, 2> current;
    array<int, 2> start = {obj.x(), obj.y()};
    Min_Heap<array<int, 2>> queued_tiles;
    map<array<int, 2>, array<int, 2>> hit_tiles;
    stack<array<int, 2>> path;

    hit_tiles[start] = start;
    add_to_path(bound_x, bound_y, dst, start, get_north(start), hit_tiles, queued_tiles);
    add_to_path(bound_x, bound_y, dst, start, get_south(start), hit_tiles, queued_tiles);
    add_to_path(bound_x, bound_y, dst, start, get_west(start), hit_tiles, queued_tiles);
    add_to_path(bound_x, bound_y, dst, start, get_east(start), hit_tiles, queued_tiles);
    add_to_path(bound_x, bound_y, dst, start, get_north_east(start), hit_tiles, queued_tiles);
    add_to_path(bound_x, bound_y, dst, start, get_south_east(start), hit_tiles, queued_tiles);
    add_to_path(bound_x, bound_y, dst, start, get_north_west(start), hit_tiles, queued_tiles);
    add_to_path(bound_x, bound_y, dst, start, get_south_west(start), hit_tiles, queued_tiles);

    while(!queued_tiles.empty())
    {
        current = queued_tiles.pop();
    
        if(current == dst)
        {
            break;
        }

        add_to_path(bound_x, bound_y, dst, current, get_north(current), hit_tiles, queued_tiles);
        add_to_path(bound_x, bound_y, dst, current, get_south(current), hit_tiles, queued_tiles);
        add_to_path(bound_x, bound_y, dst, current, get_west(current), hit_tiles, queued_tiles);
        add_to_path(bound_x, bound_y, dst, current, get_east(current), hit_tiles, queued_tiles);
        add_to_path(bound_x, bound_y, dst, current, get_north_east(current), hit_tiles, queued_tiles);
        add_to_path(bound_x, bound_y, dst, current, get_south_east(current), hit_tiles, queued_tiles);
        add_to_path(bound_x, bound_y, dst, current, get_north_west(current), hit_tiles, queued_tiles);
        add_to_path(bound_x, bound_y, dst, current, get_south_west(current), hit_tiles, queued_tiles);
    }

    for(array<int, 2> index = current; index != start;)
    {
        path.push(index);
        index = hit_tiles[index];
    }

    return path; 
}


void low_rez::Map::add_to_path(
        int bound_x,
        int bound_y,
        array<int, 2> dst,
        array<int, 2> src,
        array<int, 2> to_add,
        map<array<int, 2>, array<int, 2>>& hit_tiles,
        Min_Heap<array<int, 2>>& queued_tiles)
{
    bool x_out_of_bounds = to_add[0] < 0 || to_add[0] > bound_x;
    bool y_out_of_bounds = to_add[1] < 0 || to_add[1] > bound_y;
    unsigned int distance_x = to_add[0] - dst[0];
    unsigned int distance_y = to_add[1] - dst[1];
    unsigned int distance = distance_x * distance_x + distance_y * distance_y;


    if(hit_tiles.find(to_add) == hit_tiles.end() && !x_out_of_bounds && !y_out_of_bounds)
    {
        queued_tiles.push(to_add, distance);
        hit_tiles[to_add] = src;
    }
}


array<int, 2> low_rez::Map::get_north(array<int, 2> src) 
{
    array<int, 2> out_point = {src[0], src[1] - 1};

    return out_point;
}


array<int, 2> low_rez::Map::get_south(array<int, 2> src)
{
    array<int, 2> out_point = {src[0], src[1] + 1};

    return out_point;
}


array<int, 2> low_rez::Map::get_east(array<int, 2> src)
{
    array<int, 2> out_point = {src[0] + 1, src[1]};

    return out_point;
}


array<int, 2> low_rez::Map::get_west(array<int, 2> src)
{
    array<int, 2> out_point = {src[0] - 1, src[1]};

    return out_point;
}


array<int, 2> low_rez::Map::get_south_west(array<int, 2> src)
{
    array<int, 2> out_point = {src[0] - 1, src[1] + 1};

    return out_point;
}
array<int, 2> low_rez::Map::get_south_east(array<int, 2> src)
{
    array<int, 2> out_point = {src[0] + 1, src[1] + 1};

    return out_point;
}
array<int, 2> low_rez::Map::get_north_west(array<int, 2> src)
{
    array<int, 2> out_point = {src[0] + 1, src[1] - 1};

    return out_point;
}
array<int, 2> low_rez::Map::get_north_east(array<int, 2> src)
{
    array<int, 2> out_point = {src[0] + 1, src[1] + 1};

    return out_point;
}
