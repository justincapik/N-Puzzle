#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>
using namespace std;

#include "Puzzle.class.hpp"
#include "Solver.class.hpp"

int size = 0;

int **parsing(string fileName) {
    ifstream file;
    string line;
    int **ret = NULL;

    int y = -1;
    int x = 0;

    file.open(fileName);
    if (!file.is_open())
        throw runtime_error("Error opening file");
    while (getline(file, line)) {
        if (line[0] == '#')
            continue;
        stringstream ss;
        ss << line;
        while (!ss.eof()) {
            unsigned int n;
            ss >> n;
            if (ss.fail()) {
                ss.clear();
                if (line[ss.tellg()] == '#')
                    break;
                else
                    throw runtime_error("Invalid file format");
            }
            if (::size == 0) {
                ::size = n;
                x = ::size;
                ret = new int*[n];
            }
            else if (ss.good() || ss.eof()) {
                if (x == 0)
                    ret[y] = new int[::size];
                if (n + 1 > static_cast<unsigned int>(::size * ::size))
                    throw runtime_error("Invalid file format");
                ret[y][x++] = n;
            }
        }
        if (x != ::size || ::size > 5 || ::size < 3)
            throw runtime_error("Invalid file format");
        x = 0;
        y++;
    }
    file.close();
    return ret;
}

int     main(int ac, char **av)
{
    string algo = "AStarSearch";
    string heuristic = "ManhattanDistance";
    int **firstTab = NULL;

    try {
        if (ac == 1)
            throw runtime_error("usage");
        for (int n = 1; n < ac; n++) {
            if (av[n][0] == '-') {
                switch (av[n][1]) {
                    case 'g':
                        algo = "GreedySearch";
                        break;
                    case 'u':
                        algo = "UniformCost";
                        break;
                    default:
                        throw runtime_error("invalid option: " + static_cast<string>(av[n]));
                }
            }
            else if (!firstTab) {
                firstTab = parsing(av[n]);
                cout << ::size << endl;
            }
        }
    }
    catch (exception& e) {
        if (static_cast<string>(e.what()).compare("usage"))
            cout << e.what() << endl << endl;
        cout << "Usage: N-Puzzle [-g] [-u] FileName" << endl
        << "   -g   Use greedy search (default : A*)" << endl
        << "   -u   Use uniform-cost search" << endl;
        return 0;
    }
    try {
        Puzzle *ogPuzzle = new Puzzle(firstTab, ::size);
        std::cout << std::endl;

        Solver solver(ogPuzzle, 3);
        Puzzle *solution = solver.solve("A* Tiles-out", "None");
        solution->printPuzzle();
        std::cout << (std::to_string(solution->getDepth())) << std::endl;
    }
    catch (exception& e) {
        cout << e.what() << endl;
    }


    return (0);
}
