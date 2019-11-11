#include <algorithm>
#include <iostream>
#include <cmath>
#include "NodeSolver.class.hpp"
#include "Node.class.hpp"

NodeSolver::NodeSolver(int **original, const int size):
    _size(size)
{
    this->_base = new Node(size);
    Node    *current = this->_base;
    Node    *tmp;

    current->heuristic = 0;
    for (int i = 0; i < size * size; ++i)
    {
        current->tab[
            original[static_cast<int>(std::floor(i / size))][i % size]
        ] = new Node(*current);
        tmp = current;
        current = current->tab[original[static_cast<int>(std::floor(i / size))][i % size]];
        current->heuristic = 0;
        current->prev = tmp;
    }
    this->_totalsize = size * size;
    for (int i = 0; i < size; ++i)
        delete original[i];
    delete original;
}

NodeSolver::~NodeSolver()
{
    delete this->_base;
}

Node        *NodeSolver::getBestPuzzle(void)
{
    Node    *tmp = this->_base;
    int     bestidx, bestval;

    for (int i = 0; i < this->_totalsize; ++i)
    {
        bestidx = -1;
        bestval = 1000000;
        for (int j = 0; j < this->_totalsize; ++j)
        {
            if (tmp->tab[j] != NULL && tmp->tab[j]->needToCheck == true
                && tmp->tab[j]->heuristic < bestval)
            {
                bestval = tmp->tab[j]->heuristic;
                bestidx = j;
            }
        }
        if (bestidx == -1)
            throw std::runtime_error("Finished search for best puzzle before the end of the tree's depth");
        tmp = tmp->tab[bestidx];
    }
    return tmp;
}

void        NodeSolver::convertNodeToTable(Node *node, int ***tab)
{
    Node **tmp;
    for (int i = this->_totalsize - 1; i >= 0; --i)
    {
        if (node->prev == NULL)
            throw std::runtime_error("Error while converting node to int**, prev is NULL");
        if ((tmp = std::find(node->prev->tab,
                node->prev->tab + this->_totalsize, node))
            == node->prev->tab + this->_totalsize)
            throw std::runtime_error("Error while converting node to int**, node not found in prev");
        
        (*tab)[static_cast<int>(std::floor(i / this->_size))][i % this->_size] =
            tmp - node->prev->tab;
        node = node->prev;
    }
}

void        NodeSolver::findNumberinPuzzle(int nb, int *x, int *y, int **puzzle)
{
    for (*y = 0; *y < this->_size; ++(*y))
        for (*x = 0; *x < this->_size; ++(*x))
            if (puzzle[*y][*x] == nb)
                return ;
    throw std::runtime_error(("couldn't find " + std::to_string(nb)
        + " in puzzle").c_str());
}

bool        NodeSolver::genNextPuzzle(int **puzzle)
{
    int x, y;
    int poss[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    static int  turn = 0;
    static bool rectify = false;
    if (turn >= 4)
    {   
        turn = 0;
        rectify = false;
    }

    this->findNumberinPuzzle(0, &x, &y, puzzle);
    
    if (rectify == true)
    {
        std::swap(puzzle[y - poss[turn - 1][0]][x - poss[turn - 1][1]], puzzle[y][x]);
        y -= poss[turn - 1][0];
        x -= poss[turn - 1][1];
    }

    if (y + poss[turn][0] < 0 || y + poss[turn][0] >= this->_size
        || x + poss[turn][1] < 0 || x + poss[turn][1] >= this->_size)
        rectify = false;
    else
    {
        std::swap(puzzle[y + poss[turn][0]][x + poss[turn][1]], puzzle[y][x]);
        rectify = true;
    }
    ++turn; 
    return rectify;
}

void        NodeSolver::calculateHeuristic(int **puzzle, int *heuristic,
    int *depth, std::string heuristicType)
{
    *heuristic = 1;
    *depth = 0;
    (void)puzzle;
    (void)heuristicType;
} //TODO:

void        rec(Node *node, std::string *toPrint, int size)
{
    static int level = 0;

    //toPrint[level] += "|";
    for (int i = 0; i < size; ++i)
    {
        if (node->tab[i] != NULL)
        {
            toPrint[level] += "|";
            toPrint[level] += std::to_string(node->tab[i]->heuristic);
            ++level;
            rec(node->tab[i], toPrint, size);
            --level;
        }
    }
    toPrint[level] += "|";
}

void        showTree(Node *base, const int size)
{
    std::string  *toPrint = new std::string[size];
    for (int i = 0; i < size; ++i)
        toPrint[i] = "";
    rec(base, toPrint, size);
    for (int i = 0; i < size; ++i)
        std::cout << toPrint[i] << std::endl;
    delete[] toPrint;
}

Node        *NodeSolver::solve(std::string heuristicType, std::string searchType)
{
    int     **puzzle;

    puzzle = new int*[this->_size];
    for (int i = 0; i < this->_size; ++i)
        puzzle[i] = new int[this->_size];

    Node    *best = this->getBestPuzzle();
    this->convertNodeToTable(best, &puzzle);
    for (int k = 0; k < 4; ++k)
    {
        if (genNextPuzzle(puzzle) == false)
            continue;
        showTree(this->_base, this->_size * this->_size);
        
        int treeDepth;
        Node *current = this->_base->throwSearch(puzzle, &treeDepth);
        
        int heuristic, depth;
        this->calculateHeuristic(puzzle, &heuristic, &depth, heuristicType);
        
        if (treeDepth != this->_totalsize - 1)
            current->addPuzzleToTree(puzzle, treeDepth, 0, 0);
        /*
        else
        {
            if (current->heuristic > heuristic)
            {
                current->updateBranchToTop(heuristic, depth); //TODO:
            }
        }
        */
        for (int i = 0; i < this->_size; ++i)
        {
            for (int j = 0; j < this->_size; ++j)
                std::cout << puzzle[i][j];
            std::cout << std::endl;
        }
        std::cout << "depth = " << treeDepth << std::endl;
        std::cout << std::endl;
    }
    best->closeNode();
    showTree(this->_base, this->_size * this->_size);
    /*
    while(1)
    {
    }
    */
    (void)heuristicType;
    (void)searchType;
    for (int i = 0; i < this->_size; ++i)
        delete puzzle[i];
    delete puzzle;
    return NULL;
}