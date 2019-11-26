#ifndef PRQSOLVER_CLASS_HPP
#define PRQSOLVER_CLASS_HPP

#include <iostream>

#include "BinaryTree.class.hpp"
#include "../srcs/BinaryTree.class.cpp"
#include "PRQ.class.hpp"
#include "PRQPuzzle.class.hpp"

class PRQSolver
{
    private:
        int         **_solution;
        PRQPuzzle   *_solutionPuzzle;
        int         _size;

        PriorityQueue<PRQPuzzle*>   _openList;
        BinaryTree<PRQPuzzle*>      _closedList;

        PRQPuzzle   *genSolution(void);

        PRQPuzzle   *uniformCostSearch(std::vector<PRQPuzzle*> generated,
            std::string heuristicType, PRQPuzzle *best);
        PRQPuzzle   *greedySearch(std::vector<PRQPuzzle*> generated,
            std::string heuristicType, PRQPuzzle *best);

    public:

        PRQSolver(int **original, int size);
        ~PRQSolver();

        //heuristicType -> "ManhattanDistance", "EuclidianDistance" ...
        //searchType -> "GreedySearch" or "UniformCost"
        PRQPuzzle      *solve(std::string heuristicType, std::string searchType);
        
        double  calcHeuristic(PRQPuzzle *puzzle, std::string heuristicType); 
        void    addToOpenList(PRQPuzzle *puzzle);
        void    addToClosedList(PRQPuzzle *puzzle);
        // returns -1 if the puzzle is not in the closed list,
        // it's position if it is
        bool    checkIfInClosedList(PRQPuzzle *generated, PRQPuzzle **closed);
        bool    checkIfInOpenList(PRQPuzzle *generated);
        PRQPuzzle   *getNextAndUpdateOpenList();
        void        deleteFromClosedList(PRQPuzzle *prevClose);
        void        deleteFromOpenList(PRQPuzzle *prevOpen);

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