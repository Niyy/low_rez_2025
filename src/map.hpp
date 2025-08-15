#include <tuple>
#include <map>
#include "object.hpp"


class Terrain
{
    public:
        Terrain();

    private:
        std::map<std::tuple<int, int>, Object> _objects;
};
