#include <vector>
#include <algorithm>

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
            balance_up(heap.size() - 1);
            return true;
        }


        T pop()
        {
            Element temp = heap[0];

            heap[0] = heap[heap.size() - 1];
            heap.pop_back();

            balance_down(0);

            return temp.contents;
        }


        void balance_up(unsigned int entry_point)
        {
            unsigned int sibling = 0;
            unsigned int parent = 0;

            if(entry_point == 0) { return; }
            
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

            if(heap[parent].weight > heap[entry_point].weight)
            {
                std::swap(heap[parent], heap[entry_point]);
                balance_up(parent);
            }
        }


        void balance_down(unsigned int entry_point)
        {
            unsigned int child_right = entry_point * 2 + 1;
            unsigned int child_left = entry_point;

            if(entry_point >= heap.size()) { return; }
            
            if(
                heap[child_left].weight < heap[child_right].weight && 
                heap[child_left].weight < heap[entry_point].weight
            ) 
            { 
                std::swap(heap[child_left], heap[entry_point]);
                balance_down(child_left);
            }
            else if(
                heap[child_right].weight < heap[child_left].weight && 
                heap[child_right].weight < heap[entry_point].weight
            )
            { 
                std::swap(heap[child_right], heap[entry_point]);
                balance_down(child_right);
            }
        }


        bool empty()
        {
            return heap.size() == 0;
        }
};

#endif
