#include <tuple>
#include <map>
#include <string>
#include <vector>
#include "object.hpp"
#include "pos.hpp"

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
             const std::map<std::tuple<int, int>, Object>& get_objects();
             Object& Object::query(Pos where, Type what);

        private:
            std::map<std::tuple<int, int>, Object> _objects;
            std::map<std::string, std::vector<int[2]>> _map_bound_groups;
    };
}
