#include <vector>
#include <array>
#include <stack>
#include "object.hpp"


using std::stack;
using std::array;


class Actor : protected Object
{
    protected:
        stack<array<int, 2>> _path;

    public:
        void set_path(stack<array<int, 2>> i_path);

        void move();
};
