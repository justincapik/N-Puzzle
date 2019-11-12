#include <ncurses.h>
#include <string>
using namespace std;
#include "Visual.class.hpp"

Visual::Visual(int size) : _size(size) {
	initscr();
	noecho();
	cbreak();
	curs_set(0);
}

Visual::~Visual() {
	endwin();
}

void Visual::print() {
	int input = getch();
	while(input != 27) {
		string tab = "";
		for (int i = 0; i < this->_size; i++) {
			for (int n = 0; n < this->_size; n++)
				tab += ((n == 0) ? "+--+" : "--+");
			for (int n = 0; n < this->_size; n++)
				tab += ((n == 0) ? "\n|  |" : "  |");
			tab += ("\n");
			if (i == this->_size - 1)
				for (int n = 0; n < this->_size; n++)
					tab += ((n == 0) ? "+--+" : "--+");
		}
		move(0,0);
		printw(tab.c_str());
		refresh();
		input = getch();
	}
	return ;
}
