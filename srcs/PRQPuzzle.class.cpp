#include <iostream>
#include <stdexcept>

#include "PRQPuzzle.class.hpp"

PRQPuzzle::PRQPuzzle(int **puzzle, int size, double heuristic, int depth):
_puzzle(puzzle), _size(size), _heuristic(heuristic), _depth(depth)
{
    //std::cout << "puzzle create";
    this->prevInSolution = nullptr;

    this->_hash = 0;
    this->_hash = this->hashPuzzle(puzzle);
    //std::cout << " with hash = " << this->_hash << " and adress = " << this << std::endl;
}

PRQPuzzle::PRQPuzzle(PRQPuzzle const & instance): _size(instance.getSize()), _heuristic(instance.getHeuristic()),
    _depth(instance.getDepth()), _hash(instance.getHash())
{
    this->_heuristic = instance.getHeuristic();
    this->_depth = instance.getDepth();
    this->_hash = instance.getHash();

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
}

PRQPuzzle::~PRQPuzzle(void)
{
    //std::cout << "puzzle delete with hash = " << this->_hash << " and adress = " << this << std::endl;
    for (int i = 0; i < this->_size; ++i)
    {
        delete[] this->_puzzle[i];
        this->_puzzle[i] = nullptr;
    }
    delete[] this->_puzzle;
    this->_puzzle = nullptr;
}

bool        PRQPuzzle::compare(PRQPuzzle rhs) const
{
    int     **tmp = rhs.getPuzzle();

    if (rhs.getSize() != this->_size)
    {
        printf("lhs size = %d, rhs size = %d\n", this->_size, rhs.getSize());
        printf("rhs size = %d, lhs size = %d\n", rhs.getSize(), this->_size);
        throw std::runtime_error(std::string("Tried to compare puzzles of different size"));
    }
    if (this->_hash != rhs.getHash())
        return (false);
    for(int i = 0; i < this->_size; ++i)
        for (int j = 0; j < this->_size; j++)
            if (tmp[i][j] != this->_puzzle[i][j])
                return (false);
    return (true);
}

int         **PRQPuzzle::getPuzzle(void) const
{
    return this->_puzzle;
}

int         PRQPuzzle::getSize(void) const
{
    return this->_size;
}

size_t      PRQPuzzle::getHash(void) const
{
    return this->_hash;
}

double         PRQPuzzle::getHeuristic(void) const
{
    return this->_heuristic;
}

void        PRQPuzzle::setHeuritic(double heuristic)
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

void        PRQPuzzle::printPuzzle(int tabs) const
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
    this->_hash = this->hashPuzzle(this->_puzzle);
}

std::vector<PRQPuzzle*>    PRQPuzzle::generatePuzzleFromPosition(void)
{
    std::vector<PRQPuzzle*> retPoss;
    int poss[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    int x, y;
    this->findNumberinPuzzle(0, &x, &y);
    if (x == this->_size && y == this->_size)
        throw std::runtime_error(std::string("Tried to generate puzzles from puzzle without empty space"));

    for(int i = 0; i < 4; ++i)
    {
        if (y + poss[i][0] < 0 || y + poss[i][0] >= this->_size
            || x + poss[i][1] < 0 || x + poss[i][1] >= this->_size)
            continue;
        PRQPuzzle *tmp = new PRQPuzzle(*this);
        tmp->swapValues(x, y, x + poss[i][1], y + poss[i][0]);
        tmp->increaseDepth();
        retPoss.push_back(tmp);
    }

    return (retPoss);
}

void    PRQPuzzle::findNumberinPuzzle(int nb, int *x, int *y)
{
    for (*y = 0; *y < this->_size; ++(*y))
        for (*x = 0; *x < this->_size; ++(*x))
            if (this->_puzzle[*y][*x] == nb)
                return ;
    throw std::runtime_error(("couldn't find " + std::to_string(nb)
        + " in puzzle").c_str());
}

size_t  PRQPuzzle::hashPuzzle(int **puzzle)
{
    std::hash<std::string> hasher;
    std::string str = "";

    for (int i = 0; i < this->_size; ++i)
        for (int j = 0; j < this->_size; ++j)
            str += std::to_string(puzzle[i][j]);
    return hasher(str);
}
