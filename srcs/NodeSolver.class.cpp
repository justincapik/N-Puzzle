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

    for (int i = 0; i < size * size; ++i)
    {
        current->tab[
            original[static_cast<int>(std::floor(i / size))][i % size]
        ] = new Node(*current);
        tmp = current;
        current = current->tab[original[static_cast<int>(std::floor(i / size))][i % size]];
        current->prev = tmp;
    }
    this->_totalsize = size * size;
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
            throw std::runtime_error("Finished before the end of the tree's depth");
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

Node        *NodeSolver::solve(std::string heuristicType, std::string searchType)
{
    int     **puzzle;

    puzzle = new int*[this->_size];
    for (int i = 0; i < this->_size; ++i)
        puzzle[i] = new int[this->_size];
    this->convertNodeToTable(this->getBestPuzzle(), &puzzle);
    for (int i = 0; i < this->_size; ++i)
    {
        for (int j = 0; j < this->_size; ++j)
            std::cout << puzzle[i][j];
        std::cout << std::endl;
    }
    /*
    while(1)
    {
    }
    */
   (void)heuristicType;
   (void)searchType;
   return NULL;
}