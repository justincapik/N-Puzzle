#include <iostream>
#include <stdexcept>

#include "PRQPuzzle.class.hpp"

PRQPuzzle::PRQPuzzle():
    _size(-1)
{
    this->_puzzle = nullptr;
    this->_heuristic = -1;
    this->_depth = -1;
    this->prevInSolution = nullptr;
    this->next = nullptr;
}

PRQPuzzle::PRQPuzzle(int **puzzle, int size, int heuristic, int depth):
_puzzle(puzzle), _size(size), _heuristic(heuristic), _depth(depth)
{
    this->prevInSolution = nullptr;
}

PRQPuzzle::PRQPuzzle(PRQPuzzle const & instance):
    _size(instance.getSize())
{
    this->_heuristic = instance.getHeuristic();
    this->_depth = instance.getDepth();
    
    this->_puzzle = new int*[this->_size];
    for (int i = 0; i < this->_size; i++)
        this->_puzzle[i] = new int[this->_size];
    int **tmp = instance.getPRQPuzzle();
    for (int y = 0; y < this->_size; ++y)
    {
        for (int x = 0; x < this->_size; ++x)
            this->_puzzle[y][x] = tmp[y][x];
    }
    this->prevInSolution = nullptr;
    this->next = nullptr;
}

PRQPuzzle::~PRQPuzzle(void)
{
    for (int i = 0; i < this->_size; ++i)
    {
        delete[] this->_puzzle[i];
        this->_puzzle[i] = nullptr;
    }
    delete[] this->_puzzle;
    this->_puzzle = nullptr;
}

bool        PRQPuzzle::operator==(PRQPuzzle const rhs)
{
    int     **tmp = rhs.getPRQPuzzle();

    if (rhs.getSize() != this->_size)
    {
        printf("lhs size = %d, rhs size = %d\n", this->_size, rhs.getSize());
        std::cout << static_cast<void*>(this) << std::endl;
        throw std::runtime_error(std::string("Tried to compare puzzles of different size"));
    }
    for(int i = 0; i < this->_size; ++i)
        for (int j = 0; j < this->_size; j++)
            if (tmp[i][j] != this->_puzzle[i][j])
                return (false);
    return (true);
}

int         **PRQPuzzle::getPRQPuzzle(void) const
{
    return this->_puzzle;
}

int         PRQPuzzle::getSize(void) const
{
    return this->_size;
}

int         PRQPuzzle::getHeuristic(void) const
{
    return this->_heuristic;
}

void        PRQPuzzle::setHeuritic(int heuristic)
{
    this->_heuristic = heuristic;
}

int         PRQPuzzle::getDepth(void) const
{
    return this->_depth;
}

void        PRQPuzzle::increaseDepth(void)
{
    ++(this->_depth);
}

void        PRQPuzzle::printPRQPuzzle(int tabs) const
{
    for(int y = 0; y < this->_size; ++y)
    {
        for (int i = 0; i < tabs; ++i)
            std::cout << "\t";
        for (int x = 0; x < this->_size; ++x)
            std::cout << (std::to_string(this->_puzzle[y][x]) + " ");
        std::cout << std::endl;
    }
}

void        PRQPuzzle::swapValues(int x1, int y1,
    int x2, int y2)
{
    int     tmp;

    tmp = this->_puzzle[y1][x1];
    this->_puzzle[y1][x1] = this->_puzzle[y2][x2];
    this->_puzzle[y2][x2] = tmp;
}

PRQPuzzle   *PRQPuzzle::generatePRQPuzzleFromPosition(void)
{
    PRQPuzzle   *retPoss = nullptr, *current = nullptr;
    int poss[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    int x, y;

    this->findNumberinPRQPuzzle(0, &x, &y);
    if (x == this->_size || y == this->_size)
        throw std::runtime_error("Tried to generate puzzles from puzzle without empty space");

    for(int i = 0; i < 4; ++i)
    {
        if (y + poss[i][0] < 0 || y + poss[i][0] >= this->_size
            || x + poss[i][1] < 0 || x + poss[i][1] >= this->_size)
            continue;
        PRQPuzzle *tmp = new PRQPuzzle(*this);
        tmp->swapValues(x, y, x + poss[i][1], y + poss[i][0]);
        tmp->prevInSolution = this;
        tmp->increaseDepth();
        if (current != nullptr)
        {
            current->next = tmp;
            current = tmp;
        }
        else
        {
            retPoss = tmp;
            current = tmp;
        }
    }


    return (retPoss);
}

void    PRQPuzzle::findNumberinPRQPuzzle(int nb, int *x, int *y)
{
    for (*y = 0; *y < this->_size; ++(*y))
        for (*x = 0; *x < this->_size; ++(*x))
            if (this->_puzzle[*y][*x] == nb)
                return ;
    throw std::runtime_error(("couldn't find " + std::to_string(nb)
        + " in puzzle").c_str());
}