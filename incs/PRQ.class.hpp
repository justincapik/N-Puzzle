#ifndef PRQ_CLASS_HPP
# define PRQ_CLASS_HPP

#include <cmath>
//#include "PRQPuzzle.class.hpp"

# define PQ_DATA_SIZE 500000
# define PARENT(x) static_cast<int>(std::floor(x/2))
# define RIGHT_CHILD(x) (2 * x + 1)
# define LEFT_CHILD(x) (2 * x)

template <class T>
class PriorityQueue
{
    private:
        T           _data[PQ_DATA_SIZE];
        long long   _max;
        long long   _cursor;

    public:
        PriorityQueue()
        {
            //bzero(static_cast<void*>(this->_data), sizeof(T) * PQ_DATA_SIZE);
            this->_cursor = 1;
        }
        ~PriorityQueue()
        {
            for (int i = 1; i < this->_max; ++i)
                delete this->_data[i];
        }

        void    add(T var)
        {
            std::cout << "add start" << std::endl;
            this->_data[this->_cursor] = var;
            long long tmp = this->_cursor;

            while (tmp != 1 && this->_data[PARENT(tmp)] > this->_data[tmp])
            {
                std::swap(this->_data[PARENT(tmp)], this->_data[tmp]);
                tmp = PARENT(tmp);
            }
            this->_cursor = (this->_cursor + 1) % PQ_DATA_SIZE;
            this->_max = (this->_max + 1) % PQ_DATA_SIZE;
            std::cout << "add end" << std::endl;
            this->printData();
        }
        
        T       pop_head()
        {
            std::cout << "pop_head start" << std::endl;
            if (this->_cursor == 1)
                return nullptr;

            T top = this->_data[1];
            long long tmp = 1;

            std::swap(this->_data[1], this->_data[this->_cursor - 1]);
            --this->_cursor;
            
            while (tmp < this->_cursor)
            {
                std::cout << "idx = " << tmp << ", val = "
                    << this->_data[tmp]->getHeuristic() <<std::endl;
                if (RIGHT_CHILD(tmp) < this->_cursor)
                {
                    std::cout << "right child idx = " << RIGHT_CHILD(tmp) << ", val = "
                        << this->_data[RIGHT_CHILD(tmp)]->getHeuristic() << std::endl;
                }
                if (LEFT_CHILD(tmp) < this->_cursor)
                {
                    std::cout << "left child idx = " << LEFT_CHILD(tmp) << ", val = "
                        << this->_data[LEFT_CHILD(tmp)]->getHeuristic() << std::endl;
                }
                
                
                if (LEFT_CHILD(tmp) == this->_cursor)
                {
                    std::cout << "RESULT => left child" << std::endl;
                    std::swap(this->_data[LEFT_CHILD(tmp)], this->_data[tmp]);
                    tmp = LEFT_CHILD(tmp);
                }
                else if (RIGHT_CHILD(tmp) == this->_cursor)
                {
                    std::cout << "RESULT => right child" << std::endl;
                    std::swap(this->_data[RIGHT_CHILD(tmp)], this->_data[tmp]);
                    tmp = RIGHT_CHILD(tmp);
                }
                else if (LEFT_CHILD(tmp) > this->_cursor
                    || (this->_data[LEFT_CHILD(tmp)] >= this->_data[tmp]
                        && this->_data[RIGHT_CHILD(tmp)] >= this->_data[tmp]))
                    break;
                else if (this->_data[LEFT_CHILD(tmp)]->getHeuristic() <= this->_data[RIGHT_CHILD(tmp)]->getHeuristic()
                    && this->_data[LEFT_CHILD(tmp)]->getHeuristic() < this->_data[tmp]->getHeuristic())
                {
                    std::cout << "RESULT => left child" << std::endl;
                    std::swap(this->_data[LEFT_CHILD(tmp)], this->_data[tmp]);
                    tmp = LEFT_CHILD(tmp);
                }
                else if (this->_data[LEFT_CHILD(tmp)]->getHeuristic() > this->_data[RIGHT_CHILD(tmp)]->getHeuristic()
                    && this->_data[RIGHT_CHILD(tmp)]->getHeuristic() < this->_data[tmp]->getHeuristic())
                {
                    std::cout << "RESULT => right child" << std::endl;
                    std::swap(this->_data[RIGHT_CHILD(tmp)], this->_data[tmp]);
                    tmp = RIGHT_CHILD(tmp);
                }
                else
                    break;
            }
            std::cout << "pop_head end" << std::endl;
            this->printData();
            return top;
        }

        T       getTop()
        {
            if (this->_cursor > 1)
                return this->_data[1];
            else
                return nullptr;
        }

        void    printData()
        {
            std::cout << std::endl;
            std::cout << "tree start" << std::endl;
            for (int i = 1; i < this->_cursor; ++i)
            {
                    std::cout << this->_data[i]->getHeuristic() << " ";
            }
            std::cout << std::endl << "tree end" << std::endl;
            std::cout << std::endl;
        }
};

#endif