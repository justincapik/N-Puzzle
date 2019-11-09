#ifndef SOLVER_CLASS_HPP
#define SOLVER_CLASS_HPP

#include <iostream>
#include <vector>

#include "Puzzle.class.hpp"

class Solver
{
    private:
        Puzzle*                 _original;
        Puzzle*                 _solution;
        int                     _size;

        std::vector<Puzzle*>    _openList;
        std::vector<Puzzle*>    _closedList;

        Puzzle                  *genSolution(void);
        bool                    isSolvable(int size);

    public:

        Solver(Puzzle* original, int size);
        ~Solver();

        //heuristicType -> "ManhattanDistance", "EuclidianDistance" ...
        //searchType -> "GreedySearch" or "UniformCost"
        Puzzle      *solve(std::string heuristicType, std::string searchType);

        int                     calcHeuristic(Puzzle *puzzle,
            std::string heuristicType); //TODO: actual heuristics
        void                    addToOpenList(Puzzle *puzzle,
            std::string searchType); //TODO:
        // returns -1 if the puzzle is not in the closed list,
        // it's position if it is
        int                     checkIfInClosedList(Puzzle *puzzle); //TODO:

        double      timeTaken;
        int         totalOperations;
        int         totalOpenStates;
        int         maxStatesAtATime;

};

#endif
