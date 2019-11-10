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

        //maybe don't return void?
        void        swap(int *a, int *b);
        void        handlePuzzle(int **puzzle);
        Node        *getBestPuzzle(void);
        void        convertNodeToTable(Node *node, int ***tab);

    public:
        NodeSolver(int **original, int size);
        ~NodeSolver();

        Node        *solve(std::string heuristicType, std::string searchType);

};

#endif