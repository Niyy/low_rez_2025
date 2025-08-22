#include <vector>

using std::vector;

#ifndef MIN_HEAP
#define MIN_HEAP

template<typename T> class Min_Heap
{
    private:
        struct Element
        {
            T contents;
            unsigned int weight;
        };

        vector<Element> heap;
    

    public:
        Min_Heap() :
            heap() {};
        bool push(T i_contents, unsigned int i_weight)
        {
            Element new_element;

            new_element.contents = i_contents;
            new_element.weight = i_weight;

            heap.push_back(new_element);
            return balance(0);
        }
        T pop()
        {
            Element temp = heap[0];

            heap[0] = heap[heap.size() - 1];
            heap.pop_back();

            balance(0);

            return temp.contents;
        }
        bool balance(unsigned int entry_point)
        {
            unsigned int sibling = 0;
            unsigned int parent = 0;
            
            if(entry_point % 2 == 0) 
            { 
                sibling = entry_point - 1;
                parent = entry_point / 2 - 1;
            }
            else 
            { 
                sibling = entry_point + 1; 
                parent = sibling / 2 - 1;
            }

            return true;
        }
        bool empty()
        {
            return heap.size() == 0;
        }
};

#endif
