#ifndef PRQPUZZLE_CLASS_HPP
# define PRQPUZZLE_CLASS_HPP

#include <iostream>
#include <vector>

class PRQPuzzle
{
    private:
        int         **_puzzle;
        const int   _size;
        double      _heuristic;
        int         _depth;
        size_t      _hash;

        size_t      hashPuzzle(int **puzzle);

    public:

        PRQPuzzle(int **puzzle, int size, double heuristic=0, int depth=0);
        PRQPuzzle(PRQPuzzle const & instance);
        ~PRQPuzzle();

        bool    compare(PRQPuzzle const rhs) const;

        int     **getPuzzle(void) const;
        int     getSize(void) const;
        int     getDepth(void) const;
        size_t  getHash(void) const;
        void    increaseDepth(void);

        double  getHeuristic(void) const;
        void    setHeuritic(double heuristic);

        void    printPuzzle(int tabs=0) const;

        void    findNumberinPuzzle(int nb, int *x, int *y);

        void        swapValues(int x1, int y1, int x2, int y2);
        std::vector<PRQPuzzle*>    generatePuzzleFromPosition(void);

        PRQPuzzle   *prevInSolution;
};


#endif