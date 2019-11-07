#ifndef PUZZLE_CLASS_HPP
# define PUZZLE_CLASS_HPP

#include <vector>

class Puzzle
{
    private:
        int         **_puzzle;
        const int   _size;
        int         _heuristic;
        int         _depth;
        Puzzle      *_prevPuzzle;

    public:

        Puzzle();
        Puzzle(int **puzzle, int size, int heuristic=0, int depth=0);
        Puzzle(Puzzle const & instance);
        ~Puzzle();
        //Puzzle &operator=(Puzzle const rhs);

        bool    operator==(Puzzle const rhs);

        int     **getPuzzle(void) const;
        int     getSize(void) const;
        int     getDepth(void) const;
        void    increaseDepth(void);

        int     getHeuristic(void) const;
        void    setHeuritic(int heuristic);
        
        Puzzle  *getPrevPuzzle(void) const;
        void    setPrevPuzzle(Puzzle *puzzle);

        void    printPuzzle(void) const;
        
        void                    swapValues(int x1, int y1, int x2, int y2);
        std::vector<Puzzle*>    generatePuzzleFromPosition(void);

};


#endif