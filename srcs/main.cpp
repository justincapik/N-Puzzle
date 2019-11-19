#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <list>
using namespace std;

//#include "Puzzle.class.hpp"
#include "NodeSolver.class.hpp"
#include "Visual.class.hpp"

int size = 0;

bool    clearTab(int **toClear) {
    if (toClear) {
        for (int i = 0; i < ::size; ++i)
            if (toClear[i])
                delete[] toClear[i];
        delete[] toClear;
    }
    return true;
}

int **parsing(string fileName) {
    ifstream file;
    string line;
    stringstream ss;
    int **ret = NULL;

    int y = -1;
    int x = 0;

    file.open(fileName);
    if (!file.is_open())
        throw runtime_error("Error opening file");
    while (getline(file, line)) {
        if (line[0] == '#')
            continue;
        ss.str("");
        ss.clear();
        ss << line;
        while (!ss.eof()) {
            unsigned int n;
            ss >> n;
            if (ss.fail()) {
                ss.clear();
                if (line[ss.tellg()] == '#')
                    break;
                else if (clearTab(ret))
                    throw runtime_error("Invalid file format");
            }
            if (::size == 0) {
                ::size = n;
                x = ::size;
                ret = new int*[n];
                for (int i = 0; i < (int)n; i++)
                    ret[i] = new int[n];
            }
            else if (ss.good() || ss.eof()) {
                if (x >= ::size)
                    break;
                if (n + 1 > static_cast<unsigned int>(::size * ::size) && clearTab(ret))
                    throw runtime_error("Invalid file format");
                ret[y][x++] = n;
            }
        }
        if ((x != ::size || ::size > 5 || ::size < 3) && clearTab(ret))
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
    string VisualMode = "";

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
                    case 'v':
                        VisualMode = "text";
                        break;
                    case 'w':
                        VisualMode = "web";
                        break;
                    default:
                        throw runtime_error("invalid option: " + static_cast<string>(av[n]));
                }
            }
            else if (!firstTab)
                firstTab = parsing(av[n]);
        }
    }
    catch (exception& e) {
        if (firstTab)
            clearTab(firstTab);
        if (static_cast<string>(e.what()).compare("usage"))
            cout << e.what() << endl << endl;
        cout << "Usage: N-Puzzle [-g] [-u] FileName" << endl
        << "   -g   Use greedy search (default : A*)" << endl
        << "   -u   Use uniform-cost search" << endl
        << "   -v   Use visualisator mode" << endl;
        return 0;
    }
    /*
    Puzzle *ogPuzzle = new Puzzle(firstTab, ::size);
    std::cout << std::endl;

    OldSolver solver(ogPuzzle, ::size);
    Puzzle *solution = solver.solve("A* Manhattan distance", "None");
    solution->printPuzzle();
    std::cout << (std::to_string(solution->getDepth())) << std::endl;
    */

    list<int**> soluce;
    Visual visu(::size, VisualMode);

    NodeSolver solver(firstTab, ::size);
    Node *solution = solver.solve("", "");
    int **tab = new int*[::size];
    for (int i = 0; i < ::size; ++i)
        tab[i] = new int[::size];
    int k = 0;

    while (solution != NULL)
    {
        //std::cout << "addr = " << static_cast<void*>(solution) << std::endl;
        int **tab = new int*[::size];
        for (int i = 0; i < ::size; ++i)
            tab[i] = new int[::size];
        solver.convertNodeToTable(solution, tab);
        for (int i = 0; i < ::size; ++i)
        {
            for (int j = 0; j < ::size; ++j)
                std::cout << tab[i][j] << " ";
            std::cout << std::endl;
        }
        if (VisualMode != "")
            soluce.push_front(tab);
        std::cout << std::endl;
        solution = solution->lastInSequence;
        ++k;
    }
    if (VisualMode != "") {
        if (VisualMode == "web")
            visu.GenerateWeb(soluce);
        else if (VisualMode == "text")
            visu.print(soluce);

    	for (list<int**>::iterator it = soluce.begin(); it != soluce.end(); it++) {
    		for (int n = 0; n < ::size; n++)
                delete (*it)[n];
            delete (*it);
    	}
    }
    //std::cout << "addr = " << static_cast<void*>(solution) << std::endl;




    (void)ac;
    (void)av;
    return (0);
}
