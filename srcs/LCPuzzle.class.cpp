#include <iostream>
#include <stdexcept>

#include "LCPuzzle.class.hpp"

LCPuzzle::LCPuzzle(int **puzzle, int size, double heuristic, int depth):
_puzzle(puzzle), _size(size), _heuristic(heuristic), _depth(depth)
{
    //std::cout << "mutipul paramter instanciation ("
    //    << static_cast<void*>(this) << ")" << std::endl;
    this->prevInSolution = nullptr;
    this->next = nullptr;
    this->_hash = this->hashPuzzle(puzzle);
}

LCPuzzle::LCPuzzle(LCPuzzle const & instance): _size(instance.getSize()), _heuristic(instance.getHeuristic()),
    _depth(instance.getDepth()), _hash(instance.getHash())
{
    //std::cout << "instanced with copy (" << static_cast<void*>(this)
    //    << ")" << std::endl;
    this->_heuristic = instance.getHeuristic();
    this->_depth = instance.getDepth();
    
    this->_puzzle = new int*[this->_size];
    for (int i = 0; i < this->_size; i++)
        this->_puzzle[i] = new int[this->_size];
    int **tmp = instance.getPuzzle();
    for (int y = 0; y < this->_size; ++y)
    {
        for (int x = 0; x < this->_size; ++x)
            this->_puzzle[y][x] = tmp[y][x];
    }
    this->prevInSolution = nullptr;
    this->next = nullptr;
}

LCPuzzle::~LCPuzzle(void)
{
    for (int i = 0; i < this->_size; ++i)
    {
        delete[] this->_puzzle[i];
        this->_puzzle[i] = nullptr;
    }
    delete[] this->_puzzle;
    this->_puzzle = nullptr;
}

bool        LCPuzzle::compare(LCPuzzle rhs) const
{
    int     **tmp = rhs.getPuzzle();

    //std::cout << "calling == operator" << std::endl;
    if (rhs.getSize() != this->_size)
    {
        printf("lhs size = %d, rhs size = %d\n", this->_size, rhs.getSize());
        printf("rhs size = %d, lhs size = %d\n", rhs.getSize(), this->_size);
        throw std::runtime_error(std::string("Tried to compare puzzles of different size"));
    }
    //if (this->_hash != rhs.getHash())
    //    return (false);
    for(int i = 0; i < this->_size; ++i)
        for (int j = 0; j < this->_size; j++)
            if (tmp[i][j] != this->_puzzle[i][j])
            {
                //std::cout << "finished == operator" << std::endl;
                return (false);
            }
    //std::cout << "finished == operator" << std::endl;
    return (true);
}

int         **LCPuzzle::getPuzzle(void) const
{
    return this->_puzzle;
}

int         LCPuzzle::getSize(void) const
{
    return this->_size;
}

size_t      LCPuzzle::getHash(void) const
{
    return this->_hash;
}

double         LCPuzzle::getHeuristic(void) const
{
    return this->_heuristic;
}

void        LCPuzzle::setHeuritic(double heuristic)
{
    this->_heuristic = heuristic;
}

int         LCPuzzle::getDepth(void) const
{
    return this->_depth;
}

void        LCPuzzle::increaseDepth(void)
{
    ++(this->_depth);
}

void        LCPuzzle::printPuzzle(int tabs) const
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

void        LCPuzzle::swapValues(int x1, int y1,
    int x2, int y2)
{
    int     tmp;

    tmp = this->_puzzle[y1][x1];
    this->_puzzle[y1][x1] = this->_puzzle[y2][x2];
    this->_puzzle[y2][x2] = tmp;
}

LCPuzzle   *LCPuzzle::generatePuzzleFromPosition(void)
{
    LCPuzzle   *retPoss = nullptr, *current = nullptr;
    int poss[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    int x, y;

    this->findNumberinLCPuzzle(0, &x, &y);
    if (x == this->_size || y == this->_size)
        throw std::runtime_error("Tried to generate puzzles from puzzle without empty space");

    for(int i = 0; i < 4; ++i)
    {
        if (y + poss[i][0] < 0 || y + poss[i][0] >= this->_size
            || x + poss[i][1] < 0 || x + poss[i][1] >= this->_size)
            continue;
        LCPuzzle *tmp = new LCPuzzle(*this);
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

void    LCPuzzle::findNumberinLCPuzzle(int nb, int *x, int *y)
{
    for (*y = 0; *y < this->_size; ++(*y))
        for (*x = 0; *x < this->_size; ++(*x))
            if (this->_puzzle[*y][*x] == nb)
                return ;
    throw std::runtime_error(("couldn't find " + std::to_string(nb)
        + " in puzzle").c_str());
}

size_t  LCPuzzle::hashPuzzle(int **puzzle)
{
    std::hash<std::string> hasher;
    std::string str = "";
    
    for (int i = 0; i < this->_size; ++i)
        for (int j = 0; j < this->_size; ++j)
            str += std::to_string(puzzle[i][j]);
    return hasher(str);
}