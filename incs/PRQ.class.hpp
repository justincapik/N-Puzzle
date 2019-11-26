#ifndef PRQ_CLASS_HPP
# define PRQ_CLASS_HPP

# include <cmath>
# include "PRQPuzzle.class.hpp"

# define PQ_DATA_SIZE 500000
# define PARENT(x) static_cast<int>(std::floor(x/2))
# define RIGHT_CHILD(x) (2 * x + 1)
# define LEFT_CHILD(x) (2 * x)
# define VALUE(x) this->_data[x]->getHeuristic()

template <class T>
class PriorityQueue
{
    private:
        T           _data[PQ_DATA_SIZE];
        long long   _cursor;

    public:
        PriorityQueue()
        {
            //bzero(static_cast<void*>(this->_data), sizeof(T) * PQ_DATA_SIZE);
            this->_cursor = 1;
        }
        ~PriorityQueue()
        {
            for (int i = 1; i < this->_cursor && i < PQ_DATA_SIZE; ++i)
                delete this->_data[i];
        }

        void    add(T var)
        {
            //std::cout << "=> started ADDING " << var->getHeuristic()
            //    << "\t(" << static_cast<void*>(var) << ")" << std::endl;
            this->_data[this->_cursor] = var;
            long long tmp = this->_cursor;
            
            while (tmp != 1 && VALUE(PARENT(tmp)) > VALUE(tmp))
            {
                std::swap(this->_data[PARENT(tmp)], this->_data[tmp]);
                tmp = PARENT(tmp);
            }
            ++this->_cursor;
            if (this->_cursor >= PQ_DATA_SIZE)
                throw std::runtime_error("Priority queue not big enough, increase PQ_DATA_SIZE");
        }
        
        T       pop_head()
        {
            if (this->_cursor == 1)
                return nullptr;

            T top = this->_data[1];
            //std::cout << "=> REMOVING " << top->getHeuristic()
            //    << "\t(" << static_cast<void*>(top) << ")" << std::endl;
            long long tmp = 1;

            std::swap(this->_data[1], this->_data[this->_cursor - 1]);
            --this->_cursor;
            
            while (tmp < this->_cursor)
            {
                /*
                std::cout << "idx = " << tmp << ", val = "
                    << VALUE(tmp) <<std::endl;
                if (RIGHT_CHILD(tmp) < this->_cursor)
                {
                    std::cout << "right child idx = " << RIGHT_CHILD(tmp) << ", val = "
                        << VALUE(RIGHT_CHILD(tmp)) << std::endl;
                }
                if (LEFT_CHILD(tmp) < this->_cursor)
                {
                    std::cout << "left child idx = " << LEFT_CHILD(tmp) << ", val = "
                        << VALUE(LEFT_CHILD(tmp)) << std::endl;
                }
                */

                if (LEFT_CHILD(tmp) == this->_cursor - 1)
                {
                    if (VALUE(LEFT_CHILD(tmp)) < VALUE(tmp))
                    {
                        //std::cout << "RESULT => left child (end)" << std::endl;
                        std::swap(this->_data[LEFT_CHILD(tmp)], this->_data[tmp]);
                    }
                    break;
                }
                else if (LEFT_CHILD(tmp) >= this->_cursor
                    || (VALUE(LEFT_CHILD(tmp)) >= VALUE(tmp)
                        && VALUE(RIGHT_CHILD(tmp)) >= VALUE(tmp)))
                {
                    //std::cout << "RESULT => stop (1)" << std::endl;
                    break;
                }
                else if (VALUE(LEFT_CHILD(tmp)) <= VALUE(RIGHT_CHILD(tmp))
                    && VALUE(LEFT_CHILD(tmp)) < VALUE(tmp))
                {
                    //std::cout << "RESULT => left child" << std::endl;
                    std::swap(this->_data[LEFT_CHILD(tmp)], this->_data[tmp]);
                    tmp = LEFT_CHILD(tmp);
                }
                else if (VALUE(LEFT_CHILD(tmp)) > VALUE(RIGHT_CHILD(tmp))
                    && VALUE(RIGHT_CHILD(tmp)) < VALUE(tmp))
                {
                    //std::cout << "RESULT => right child" << std::endl;
                    std::swap(this->_data[RIGHT_CHILD(tmp)], this->_data[tmp]);
                    tmp = RIGHT_CHILD(tmp);
                }
                else
                {
                    //std::cout << "RESULT => stop (2)" << std::endl;
                    break;
                }
            }
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
            for (int i = 1; i < this->_cursor; ++i)
            {
                    std::cout << VALUE(i) << " ";
            }
            std::cout << std::endl;
        }
};

#endif