#ifndef PUZZLE_CLASS_HPP
# define PUZZLE_CLASS_HPP

class PRQPuzzle
{
    private:
        int         **_puzzle;
        const int   _size;
        int         _heuristic;
        int         _depth;

    public:

        PRQPuzzle();
        PRQPuzzle(int **puzzle, int size, int heuristic=0, int depth=0);
        PRQPuzzle(PRQPuzzle const & instance);
        ~PRQPuzzle();

        bool    operator==(PRQPuzzle const rhs);

        int     **getPRQPuzzle(void) const;
        int     getSize(void) const;
        int     getDepth(void) const;
        void    increaseDepth(void);

        int     getHeuristic(void) const;
        void    setHeuritic(int heuristic);

        void    printPRQPuzzle(int tabs=0) const;

        void    findNumberinPRQPuzzle(int nb, int *x, int *y);

        void        swapValues(int x1, int y1, int x2, int y2);
        PRQPuzzle*  generatePRQPuzzleFromPosition(void);

        PRQPuzzle   *next;
        PRQPuzzle   *prevInSolution;
};


#endif