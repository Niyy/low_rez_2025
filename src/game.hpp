#include "object.hpp"
#include <map>
#include <vector>


using std::map;
using std::vector;


class Game
{
    public:
        const int grid_w = 10;
        const int grid_h = 10;           
        const int grid_step = 32;

        map<int[2], vector<Object>> _map;

        bool run();
};
