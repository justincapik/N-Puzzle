#ifndef LCPUZZLE_CLASS_HPP
# define LCPUZZLE_CLASS_HPP

class LCPuzzle
{
    private:
        int         **_puzzle;
        const int   _size;
        double      _heuristic;
        int         _depth;
        size_t      _hash;

        size_t      hashPuzzle(int **puzzle) const;

    public:

        LCPuzzle(int **puzzle, int size, double heuristic=0, int depth=0);
        LCPuzzle(LCPuzzle const & instance);
        ~LCPuzzle();

        bool    compare(LCPuzzle const rhs) const;

        int     **getPuzzle(void) const;
        int     getSize(void) const;
        int     getDepth(void) const;
        size_t  getHash(void) const;
        void    increaseDepth(void);

        double  getHeuristic(void) const;
        void    setHeuritic(double heuristic);

        void    printPuzzle(int tabs=0) const;

        void    findNumberinLCPuzzle(int nb, int *x, int *y);

        void        swapValues(int x1, int y1, int x2, int y2);
        LCPuzzle*  generatePuzzleFromPosition(void);

        LCPuzzle   *next;
        LCPuzzle   *prevInSolution;
};


#endif