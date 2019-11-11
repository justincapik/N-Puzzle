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
        int     heuristic;
        int     depth;
        bool    needToCheck;

        const int   size;
        
        //these functions will only be called from the head node
        Node        *throwSearch(int **puzzle, int *treeDepth);

        //from any node exept bottom ones
        void        addPuzzleToTree(int **puzzle, int treeDepth, int depth, int heuristic);

        //only from bottom nodes
        void        closeNode(void);
        //void        updateBranchToTop(int heuristic, int depth);
};

#endif