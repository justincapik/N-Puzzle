#ifndef OLDSOLVER_CLASS_HPP
#define OLDSOLVER_CLASS_HPP

#include <iostream>
#include <vector>

#include "Puzzle.class.hpp"

class OldSolver
{
    private:
        Puzzle*                 _original;
        Puzzle*                 _solution;
        int                     _size;

        std::vector<Puzzle*>    _openList;
        std::vector<Puzzle*>    _closedList;

        Puzzle                  *genSolution(void);
        void                    deleteFromVector(int i,
            std::vector<Puzzle*> *vec);

    public:

        OldSolver(Puzzle* original, int size);
        ~OldSolver();

        //heuristicType -> "ManhattanDistance", "EuclidianDistance" ...
        //searchType -> "GreedySearch" or "UniformCost"
        Puzzle      *solve(std::string heuristicType, std::string searchType);
        
        int                     calcHeuristic(Puzzle *puzzle,
            std::string heuristicType); 
        void                    addToOpenList(Puzzle *puzzle,
            std::string searchType);
        // returns -1 if the puzzle is not in the closed list,
        // it's position if it is
        int                     checkIfInClosedList(Puzzle *puzzle);
        int                     checkIfInOpenList(Puzzle *puzzle);

        double      timeTaken;
        int         totalOperations;
        int         totalOpenStates;
        int         maxStatesAtATime;
    
};

#endif