#ifndef SOLVER_CLASS_HPP
#define SOLVER_CLASS_HPP

#include <iostream>
#include <vector>

#include "Puzzle.class.hpp"

class Solver
{
    private:
        Puzzle*              original;

        std::vector<Puzzle*> openList;
        std::vector<Puzzle*> closedList;

        bool                checkIfSolvable(void);


    public:

        Solver(int size, Puzzle* original);
        Solver(Solver const & instance);
        ~Solver();
        Solver &operator=(Solver const rhs);

        //heuristicType -> "ManhattanDistance", "EuclidianDistance" ...
        //searchType -> "GreedySearch" or "UniformCost"
        bool        solve(std::string heuristicType, std::string searchType);
        
        double      timeTaken;
        int         totalOperations;
        int         totalOpenStates;
        int         maxStatesAtATime;
        static int  size;
    
};

#endif