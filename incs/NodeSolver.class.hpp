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
        int         **_solutionTab;
        Node        *_solutionNode;

        void        findNumberinPuzzle(int nb, int *x, int *y, int **puzzle);
        bool        genNextPuzzle(int **puzzle);
        void        handlePuzzle(int **puzzle);
        Node        *getBestPuzzle(void);
        void        calculateHeuristic(int **puzzle, double *heuristic, int depth, std::string heuristicType); //TODO:

        void        addSolution(void);
        bool        checkifsolution(int **puzzle);

        long long   totalOpenedEver;
        long long   totalStatesEver;
        long long   currentClosed;

    public:
        NodeSolver(int **original, int size);
        ~NodeSolver();

        Node        *solve(std::string heuristicType, std::string searchType);

        void        convertNodeToTable(Node *node, int **tab);
};

#endif