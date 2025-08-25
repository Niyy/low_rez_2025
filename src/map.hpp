#ifndef LOWREZ_MAP
#define LOWREZ_MAP


#include <tuple>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <array>
#include <queue>
#include <stack>
#include <iostream>
#include "object.hpp"
#include "actor.hpp"
#include "min_heap.hpp"

using std::array;
using std::vector;
using std::set;
using std::queue;
using std::string;
using std::map;
using std::stack;
using std::cout;
using std::endl;


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
            ~Map();
            bool place(Object* obj);
            bool remove(Object* obj);
            Object* get_at(int x, int y);
            std::map<unsigned int, Object*> get_objects();
            Object* query(SDL_FRect where, std::string what);
            stack<array<int, 2>> make_path(int bound_x, int bound_y, Object* obj, array<int, 2> dst);
            void add_to_path(
                int bound_x,
                int bound_y,
                array<int, 2> dst,
                array<int, 2> src,
                array<int, 2> to_add,
                map<array<int, 2>, array<int, 2>>& hit_tiles,
                Min_Heap<array<int, 2>>& queued_tiles);
            array<int, 2> get_north(array<int, 2> src);
            array<int, 2> get_south(array<int, 2> src);
            array<int, 2> get_east(array<int, 2> src);
            array<int, 2> get_west(array<int, 2> src);
            array<int, 2> get_south_west(array<int, 2> src);
            array<int, 2> get_south_east(array<int, 2> src);
            array<int, 2> get_north_west(array<int, 2> src);
            array<int, 2> get_north_east(array<int, 2> src);
            void update(Object* obj);
            void describe();

        private:
            std::map<unsigned int, Object*> _objects;
            std::map<array<int, 2>, unsigned int> _collisions;
            std::map<array<int, 2>, set<array<int, 2>>> _stored_path_results;
            std::map<string, vector<array<int, 2>>> _map_bound_groups;
            std::map< array <array <int, 2>, 2>, vector<array<int, 2>>> _mapped_path; 
    };
}


#endif
