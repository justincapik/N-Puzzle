#ifndef OLDSOLVER_CLASS_HPP
#define OLDSOLVER_CLASS_HPP

#include <iostream>

#include "PRQPuzzle.class.hpp"

class PRQSolver
{
    private:
        int         **_solution;
        PRQPuzzle   *_solutionPuzzle;
        int         _size;

        PRQPuzzle   *_openList;
        PRQPuzzle   *_closedList;

        PRQPuzzle   *genSolution(void);

    public:

        PRQSolver(int **original, int size);
        ~PRQSolver();

        //heuristicType -> "ManhattanDistance", "EuclidianDistance" ...
        //searchType -> "GreedySearch" or "UniformCost"
        PRQPuzzle      *solve(std::string heuristicType, std::string searchType);
        
        int     calcHeuristic(PRQPuzzle *puzzle, std::string heuristicType); 
        void    addToOpenList(PRQPuzzle *puzzle, std::string searchType);
        void    addToClosedList(PRQPuzzle *puzzle);
        // returns -1 if the puzzle is not in the closed list,
        // it's position if it is
        bool    checkIfInClosedList(PRQPuzzle *generated, PRQPuzzle **closed,
            PRQPuzzle **prevClosed);
        bool    checkIfInOpenList(PRQPuzzle *generated, PRQPuzzle **opened,
            PRQPuzzle **prevOpened);

        double      timeTaken;
        int         totalOperations;
        int         totalOpenStates;
        int         maxStatesAtATime;
    
};

//optimization idea =>
// make a table pointing to each depth or heuristic, for example:
// open list heuristic values -> {1, 1, 2, 3, 3, 3, 3, 3, 4, 4, 5, 5, 5, 5, ...}
// pointer table ->            {^,    ^, ^,             ^,    ^,          ...}

#endif