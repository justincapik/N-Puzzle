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

        const int     size;
        
        /*
        //these functions will only be called from the head node
        Node            *checkIfPuzzleExists(int **puzzle);
        bool            checkAndMaybeAddPuzzle(int **puzzle, int heuristic, int depth);
        */

};

#endif