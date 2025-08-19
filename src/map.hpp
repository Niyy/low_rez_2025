#include <tuple>
#include <map>
#include <string>
#include <vector>
#include <array>
#include "object.hpp"

using std::array;

#pragma once
namespace low_rez
{
    enum Type
    {
        ACTOR,
        RESOURCE_NODE,
        RESOURCE_HARVESTED
    };


    class Map 
    {
        public:
             Map() {};
             bool place(Object obj);
             bool remove(Object obj);
             Object get_at(int x, int y);
             std::map<array<int, 2>, Object> get_objects();
             Object* query(SDL_FRect where, std::string what);
             std::vector<array<int, 2>>* make_path(int bound_x, int bound_y, Object obj, array<int, 2> dst);

        private:
            std::map<array<int, 2>, Object> _objects;
            std::map<std::string, std::vector<array<int, 2>>> _map_bound_groups;
    };
}
