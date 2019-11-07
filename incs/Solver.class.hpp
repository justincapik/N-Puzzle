#ifndef SOLVER_CLASS_HPP
#define SOLVER_CLASS_HPP

#include <iostream>
#include <vector>

#include "Puzzle.class.hpp"

class Solver
{
    private:
        Puzzle*                 _original;
        int                     _size;

        std::vector<Puzzle*>    _openList;
        std::vector<Puzzle*>    _closedList;

        bool                _checkIfSolvable(void);


    public:

        Solver();
        Solver(int size, Puzzle* original);
        Solver(Solver const & instance);
        ~Solver();
        Solver &operator=(Solver const rhs);

        //heuristicType -> "ManhattanDistance", "EuclidianDistance" ...
        //searchType -> "GreedySearch" or "UniformCost"
        bool        solve(std::string heuristicType, std::string searchType);

        int         getSize();

        double      timeTaken;
        int         totalOperations;
        int         totalOpenStates;
        int         maxStatesAtATime;
    
};

#endif