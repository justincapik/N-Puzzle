#ifndef PUZZLE_CLASS_HPP
# define PUZZLE_CLASS_HPP

class Puzzle
{
private:
    int     **puzzle;
    int     heuristic;
    int     depth;

public:

    Puzzle();
    Puzzle(int **puzzle, int value, int movements);
    Puzzle(Puzzle const & instance);
    ~Puzzle();
    Puzzle &operator=(Puzzle const rhs);

    bool    operator==(Puzzle const rhs);

    int     **getPuzzle(void) const;
    int     getHeuristic(void) const;
    int     getDepth(void) const;

    Puzzle  *generatePuzzleFromPosition(void);

};


#endif