#include <tuple>
#include <map>
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
             std::map<std::tuple<int, int>, Object>& objects();

        private:
            std::map<std::tuple<int, int>, Object> _objects;
    };
}
