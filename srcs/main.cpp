#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>
using namespace std;

//#include "Puzzle.class.hpp"
#include "NodeSolver.class.hpp"

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
    /*
    Puzzle *ogPuzzle = new Puzzle(firstTab, ::size);
    ogPuzzle->printPuzzle();
    std::cout << std::endl;

    OldSolver solver(ogPuzzle, ::size);
    Puzzle *solution = solver.solve("A* Manhattan distance", "None");
    solution->printPuzzle();
    std::cout << (std::to_string(solution->getDepth())) << std::endl;
    */
    /*
    int **firstTab = new int*[3];
    for (int i = 0; i < 3; ++i)
        firstTab[i] = new int[3];
    
    firstTab[0][0] = 2;
    firstTab[0][1] = 6;
    firstTab[0][2] = 0;

    firstTab[1][0] = 5;
    firstTab[1][1] = 3;
    firstTab[1][2] = 7;

    firstTab[2][0] = 8;
    firstTab[2][1] = 1;
    firstTab[2][2] = 4;
    */

    NodeSolver solver(firstTab, ::size);
    Node *solution = solver.solve("BFS", "");
    std::cout << sizeof(Node) << std::endl;
    int **tab = new int*[::size];
    for (int i = 0; i < ::size; ++i)
        tab[i] = new int[::size];
    int k = 0;
    while (solution != NULL)
    {
        //std::cout << "addr = " << static_cast<void*>(solution) << std::endl;
        solver.convertNodeToTable(solution, tab);
        for (int i = 0; i < ::size; ++i)
        {
            for (int j = 0; j < ::size; ++j)
                std::cout << tab[i][j] << " ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
        solution = solution->lastInSequence;
        ++k;
    }
    //std::cout << "addr = " << static_cast<void*>(solution) << std::endl;
    /*
    for (int i = 0; i < ::size; ++i)
        delete[] firstTab[i];
    delete[] firstTab;
    */
    (void)ac;
    (void)av;
    return (0);
}
