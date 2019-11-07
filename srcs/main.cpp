#include <iostream>
#include <vector>

#include "Puzzle.class.hpp"
#include "Solver.class.hpp"

int     main(int argc, char **argv)
{
    int **ogtab = new int*[3];
    for(int i = 0; i < 3; ++i)
        ogtab[i] = new int[3];

    ogtab[0][0] = 1;
    ogtab[0][1] = 2;
    ogtab[0][2] = 3;

    ogtab[1][0] = 0;
    ogtab[1][1] = 8;
    ogtab[1][2] = 4;
    
    ogtab[2][0] = 7;
    ogtab[2][1] = 6;
    ogtab[2][2] = 5;

    Puzzle *ogPuzzle = new Puzzle(ogtab, 3);
    ogPuzzle->printPuzzle();
    std::cout << std::endl;

    std::vector<Puzzle*>    vec = ogPuzzle->generatePuzzleFromPosition();    

    Solver solver(ogPuzzle, 3);
    solver.solve("BFS", "None");
    (void)argc;
    (void)argv;
    return (0);
}