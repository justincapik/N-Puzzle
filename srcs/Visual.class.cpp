#include <ncurses.h>
#include <string>
#include <time.h>
#include <sstream>
#include <fstream>
#include <iostream>


using namespace std;
#include "Visual.class.hpp"

Visual::Visual(int size, string type) : _size(size), _type(type) {
	if (_type == "text") {
		initscr();
		noecho();
		start_color();
		timeout(0);
		cbreak();
		curs_set(0);
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		init_pair(2, COLOR_GREEN, COLOR_BLACK);
	}
}

Visual::~Visual() {
	if (_type == "text")
		endwin();
}

void Visual::print(list<int**> solution) {
	int input = getch();
	bool end = FALSE;
	list<int**>::iterator it = solution.begin();
	list<int**>::iterator tmp = solution.begin();
	time_t start = time(NULL);
	while(input != 27) {
        stringstream ss;
		if (time(NULL) - 1 > start && tmp != solution.end()) {
			start = time(NULL);
			tmp++;
			if (tmp != solution.end())
				it++;
			else
				end = TRUE;
		}
		for (int i = 0; i < this->_size; i++) {
			for (int n = 0; n < this->_size; n++)
				ss << ((n == 0) ? "+--+" : "--+");
			for (int n = 0; n < this->_size; n++) {
				ss << ((n == 0) ? "\n|" : "");
				ss << (((*it)[i][n] < 10) ? " " : "") << (*it)[i][n] << "|";
			}
			ss << ("\n");
			if (i == this->_size - 1)
				for (int n = 0; n < this->_size; n++)
					ss << ((n == 0) ? "+--+" : "--+");
		}

		string tab = ss.str();
		move(0,0);
		attron(COLOR_PAIR((end) ? 2 : 1));
		printw(tab.c_str());
		refresh();
		input = getch();
	}
	return ;
}

void Visual::GenerateWeb(list<int**> solution) {
	ofstream file("visu/data");
	for (list<int**>::iterator it = solution.begin(); it != solution.end(); it++) {

		cout << *it << endl;
		if (!(*it)[0])
			cout << "nope" <<endl;
		for (int n = 0; n < _size; n++) {
			for (int i = 0; i < _size; i++) {
				file << (*it)[n][i] << " ";
				cout << (*it)[n][i] << " ";
			}

		}
		file << endl;
		cout << endl;
	}
	cout << "end ?" << endl;
	file.close();
}
