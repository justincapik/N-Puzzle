#ifndef VISUAL_CLASS_HPP
#define VISUAL_CLASS_HPP

#include <ncurses.h>
#include <list>

class Visual
{
    private:
		int _size;


    public:

        Visual(int size);
        ~Visual();
		void print(list<int**>);
		void GenerateWeb(list<int**>);

};

#endif
