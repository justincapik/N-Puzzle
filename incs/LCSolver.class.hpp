#ifndef LCSOLVER_CLASS_HPP
#define LCSOLVER_CLASS_HPP

#include <iostream>

#include "LCPuzzle.class.hpp"

class LCSolver
{
    private:
        int         **_solution;
        LCPuzzle   *_solutionPuzzle;
        int         _size;

        LCPuzzle   *_openList;
        LCPuzzle   *_closedList;

        LCPuzzle   *genSolution(void);

        LCPuzzle   *UniformCostSearch(LCPuzzle *generated,
            std::string heuristicType, LCPuzzle *best);
        LCPuzzle   *GreedySearch(LCPuzzle *generated,
            std::string heuristicType, LCPuzzle *best);

    public:

        LCSolver(int **original, int size);
        ~LCSolver();

        //heuristicType -> "ManhattanDistance", "EuclidianDistance" ...
        //searchType -> "GreedySearch" or "UniformCost"
        LCPuzzle      *solve(std::string heuristicType, std::string searchType);
        
        double  calcHeuristic(LCPuzzle *puzzle, std::string heuristicType); 
        void    addToOpenList(LCPuzzle *puzzle);
        void    addToClosedList(LCPuzzle *puzzle);
        // returns -1 if the puzzle is not in the closed list,
        // it's position if it is
        bool    checkIfInClosedList(LCPuzzle *generated, LCPuzzle **closed,
            LCPuzzle **prevClosed);
        bool    checkIfInOpenList(LCPuzzle *generated, LCPuzzle **opened,
            LCPuzzle **prevOpened);
        LCPuzzle   *getNextAndUpdateOpenList();
        void        deleteFromClosedList(LCPuzzle *prevClose);
        void        deleteFromOpenList(LCPuzzle *prevOpen);

        double      timeTaken;
        int         totalOperations;
        int         totalOpenStates;
        int         maxStatesAtATime;
    
};

//optimization idea =>
// make a table pointing to each depth or heuristic, for example:
// open list heuristic values -> {1, 1, 2, 3, 3, 3, 3, 3, 4, 4, 5, 5, 5, 5, ...}
// pointer table ->              {^,    ^, ^,             ^,    ^,          ...}

#endif