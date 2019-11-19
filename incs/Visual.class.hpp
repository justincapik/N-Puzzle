#ifndef VISUAL_CLASS_HPP
#define VISUAL_CLASS_HPP

#include <ncurses.h>
#include <list>

class Visual
{
    private:
		int _size;
		std::string _type;


    public:

        Visual(int, std::string);
        ~Visual();
		void print(list<int**>);
		void GenerateWeb(list<int**>);

};

#endif
