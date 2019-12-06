#include <iostream>
#include <stdexcept>
#include <iomanip>

#include "PRQPuzzle.class.hpp"

int PRQPuzzle::size = -1;

PRQPuzzle::PRQPuzzle(int **puzzle, int size, double heuristic, int depth):
_puzzle(puzzle), _heuristic(heuristic), _depth(depth)
{
    PRQPuzzle::size = size;
    //std::cout << "puzzle create";
    this->prevInSolution = nullptr;

    this->_hash = 0;
    this->_hash = this->hashPuzzle(puzzle);
    //std::cout << " with hash = " << this->_hash << " and adress = " << this << std::endl;
}

PRQPuzzle::PRQPuzzle(PRQPuzzle const & instance):  _hash(instance.getHash()),
    _heuristic(instance.getHeuristic()), _depth(instance.getDepth())
{
    const int size = instance.getSize();
    this->_heuristic = instance.getHeuristic();
    this->_depth = instance.getDepth();
    this->_hash = instance.getHash();

    this->_puzzle = new int*[size];
    for (int i = 0; i < size; i++)
        this->_puzzle[i] = new int[size];
    int **tmp = instance.getPuzzle();
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
            this->_puzzle[y][x] = tmp[y][x];
    }
    this->prevInSolution = nullptr;
}

PRQPuzzle::~PRQPuzzle(void)
{
    for (int i = 0; i < PRQPuzzle::size; ++i)
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

    if (rhs.getSize() != PRQPuzzle::size)
    {
        printf("lhs size = %d, rhs size = %d\n", PRQPuzzle::size, rhs.getSize());
        printf("rhs size = %d, lhs size = %d\n", rhs.getSize(), PRQPuzzle::size);
        throw std::runtime_error(std::string("Tried to compare puzzles of different size"));
    }
    if (this->_hash != rhs.getHash())
        return (false);
    for(int i = 0; i < PRQPuzzle::size; ++i)
        for (int j = 0; j < PRQPuzzle::size; j++)
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
    return PRQPuzzle::size;
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
    for(int y = 0; y < PRQPuzzle::size; ++y)
    {
        for (int i = 0; i < tabs; ++i)
            std::cout << "\t";
        for (int x = 0; x < PRQPuzzle::size; ++x)
            std::cout << std::setw((PRQPuzzle::size > 3) ? 3 : 2) << (std::to_string(this->_puzzle[y][x]));
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
    if (x == PRQPuzzle::size && y == PRQPuzzle::size)
        throw std::runtime_error(std::string("Tried to generate puzzles from puzzle without empty space"));

    for(int i = 0; i < 4; ++i)
    {
        if (y + poss[i][0] < 0 || y + poss[i][0] >= PRQPuzzle::size
            || x + poss[i][1] < 0 || x + poss[i][1] >= PRQPuzzle::size)
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
    for (*y = 0; *y < PRQPuzzle::size; ++(*y))
        for (*x = 0; *x < PRQPuzzle::size; ++(*x))
            if (this->_puzzle[*y][*x] == nb)
                return ;
    throw std::runtime_error(("couldn't find " + std::to_string(nb)
        + " in puzzle").c_str());
}

size_t  PRQPuzzle::hashPuzzle(int **puzzle)
{
    std::hash<std::string> hasher;
    std::string str = "";

    for (int i = 0; i < PRQPuzzle::size; ++i)
        for (int j = 0; j < PRQPuzzle::size; ++j)
            str += std::to_string(puzzle[i][j]);
    return hasher(str);
}
