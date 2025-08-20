#include <vector>

using std::vector;

#ifndef MIN_HEAP
#define MIN_HEAP

template <typename T>

struct Element
{
    T contents;
    unsigned int weight;
}

class Min_Heap
{
    private:
        vector<Element> heap;
    

    public:
        Min_Heap() :
            count(0),
            heap() {};
        bool push(T i_contents, unsigned int i_weight);
        T pop();
        bool balance(unsigned int entry_point);
}

#endif
