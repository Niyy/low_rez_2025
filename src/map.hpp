#include <tuple>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <array>
#include <queue>
#include "object.hpp"

using std::array;
using std::vector;
using std::set;
using std::queue;
using std::string;
using std::map;

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
            unsigned int make_path(int bound_x, int bound_y, Object obj, array<int, 2> dst);
            array<int, 2> get_north(int bound_x, int bound_y, array<int, 2> src);
            array<int, 2> get_south(int bound_x, int bound_y, array<int, 2> src);
            array<int, 2> get_east(int bound_x, int bound_y, array<int, 2> src);
            array<int, 2> get_west(int bound_x, int bound_y, array<int, 2> src);

        private:
            std::map<array<int, 2>, Object> _objects;
            std::map<array<int, 2>, set<array<int, 2>>> _stored_path_results;
            std::map<string, vector<array<int, 2>>> _map_bound_groups;
    };
}
