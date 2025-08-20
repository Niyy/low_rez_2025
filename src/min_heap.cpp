#include "min_heap.hpp"


Min_Heap::push(T i_contents, unsigned int i_weight)
{
    Element new_element;

    new_element.contents = i_contents;
    new_element.weight = i_weight;

    heap.push_back(new_element);
    balance();
}


T Min_Heap::pop()
{
    Element temp = heap[0];

    heap[0] = heap[heap.size];
    heap.pop_back();

    balance(0);

    return temp.contents;
}
