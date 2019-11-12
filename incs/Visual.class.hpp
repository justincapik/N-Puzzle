#ifndef VISUAL_CLASS_HPP
#define VISUAL_CLASS_HPP

#include "Puzzle.class.hpp"
#include <ncurses.h>

class Visual
{
    private:
		int _size;


    public:

        Visual(int size);
        ~Visual();
		void print();

};

#endif
