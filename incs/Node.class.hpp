#ifndef NODE_CLASS_HPP
# define NODE_CLASS_HPP

# include <iostream>

class Node
{
    private:
        

    public:
        Node(int size);
        Node(Node const & instance);
        ~Node();

        Node    *prev;
        Node    **tab; //*Node table, not 2d array
        double  heuristic;
        int     depth;
        bool    needToCheck;
        Node    *lastInSequence;

        const int   size;
        
        //these functions will only be called from the head node
        Node        *throwSearch(int **puzzle, int *treeDepth);

        //from any node exept bottom ones
        Node        *addPuzzleToTree(int **puzzle, int treeDepth, int depth, double heuristic);

        //only from bottom nodes
        void        closeNode(void);
        void        updateBranchToTop(double heuristic, int depth);
};

#endif