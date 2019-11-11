#ifndef NODESOLVER_CLASS_HPP
# define NODESOLVER_CLASS_HPP

# include <iostream>
# include "Node.class.hpp"

class NodeSolver
{
    private:
        Node        *_base;
        const int   _size;
        int         _totalsize;

        void        findNumberinPuzzle(int nb, int *x, int *y, int **puzzle);
        bool       genNextPuzzle(int **puzzle);
        void        handlePuzzle(int **puzzle);
        Node        *getBestPuzzle(void);
        void        convertNodeToTable(Node *node, int ***tab);
        void        calculateHeuristic(int **puzzle, int *heuristic, int *depth, std::string heuristicType); //TODO:

    public:
        NodeSolver(int **original, int size);
        ~NodeSolver();

        Node        *solve(std::string heuristicType, std::string searchType);

};

#endif