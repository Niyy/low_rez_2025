#include <tuple>
#include <map>
#include "object.hpp"

using namespace low_rez
{
    class Map 
    {
        public:
             Map();
             place(int x, int y);
             remove(int x, int y);

        private:
            std::map<std::tuple<int, int>, Object> _objects;
    };
}
