#include <tuple>
#include <map>
#include <string>
#include <vector>
#include "object.hpp"

#pragma once
namespace low_rez
{
    class Map 
    {
        public:
             Map() {};
             bool place(Object obj);
             bool remove(Object obj);
             Object get_at(int x, int y);
             std::map<std::tuple<int, int>, Object>& objects();

        private:
            std::map<std::tuple<int, int>, Object> _objects;
            std::map<std::string, std::vector<int[2]>> _map_bound_groups;
    };
}
