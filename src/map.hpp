#include <tuple>
#include <map>
#include <string>
#include <vector>
#include <array>
#include "object.hpp"


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
             std::map<std::array<int, 2>, Object> get_objects();
             Object* query(SDL_FRect where, low_rez::Type what);

        private:
            std::map<std::array<int, 2>, Object> _objects;
            std::map<std::string, std::array<int, 2>> _map_bound_groups;
    };
}
