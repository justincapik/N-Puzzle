#include <cxxabi.h>
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

    this->_solutionTab = nullptr;
    this->_solutionNode = nullptr;
    this->_totalsize = size * size;
    
    current->heuristic = -1;
    current->depth = -1;
    current->prev = nullptr;
    current->needToCheck = true;

    this->_base->addPuzzleToTree(original, 0, 0, 0);
    this->addSolution();

    for (int i = 0; i < this->_size; ++i)
    {
        delete[] original[i];
        original[i] = nullptr;
    }
    delete[] original;
    original = nullptr;
}

NodeSolver::~NodeSolver()
{
    delete this->_base;
    if (this->_solutionTab != nullptr)
    {
        for (int i = 0; i < this->_size; ++i)
            delete[] this->_solutionTab[i];
        delete[] this->_solutionTab;
    }
}

Node        *NodeSolver::getBestPuzzle(void)
{
    Node    *tmp = this->_base;
    int     bestidx, bestval;

    //std::cout << "looking for best puzzle" << std::endl;
    for (int i = 0; i < this->_totalsize; ++i)
    {
        bestidx = -1;
        bestval = INT_MAX;
        for (int j = 0; j < this->_totalsize; ++j)
        {
            /*
            if (tmp->tab[j] != nullptr)
            {
                std::cout << std::to_string(tmp->tab[j]->heuristic) << " (";
                std::cout << j << ") " << std::endl;
            }
            */
            if (tmp->tab[j] != nullptr && tmp->tab[j]->needToCheck == true
                && tmp->tab[j]->heuristic < bestval)
            {
                bestval = tmp->tab[j]->heuristic;
                bestidx = j;
            }
        }
        //std::cout << " bestidx = " << bestidx << ", bestval = " << bestval;
        //std::cout << ", i = " << i << std::endl;
        if (bestidx == -1)
        {
            std::cout << "total opened => " << this->totalOpenedEver << std::endl;
            std::cout << "current closed => " << this->currentClosed << std::endl;
            std::cout << "total states => " << this->totalStatesEver << std::endl;
            throw std::runtime_error("Finished search for best puzzle before the end of the tree's depth");
        }
        tmp = tmp->tab[bestidx];
    }
    //std::cout << "finished looking for best puzzle" << std::endl;
    return tmp;
}

void        NodeSolver::convertNodeToTable(Node *node, int **tab)
{
    Node **tmp;
    for (int i = this->_totalsize - 1; i >= 0; --i)
    {
        if (node->prev == nullptr)
            throw std::runtime_error("Error while converting node to int**, prev is nullptr");
        if ((tmp = std::find(node->prev->tab,
                node->prev->tab + this->_totalsize, node))
            == node->prev->tab + this->_totalsize)
            throw std::runtime_error("Error while converting node to int**, node not found in prev");
        
        tab[static_cast<int>(std::floor(i / this->_size))][i % this->_size] =
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

void        NodeSolver::calculateHeuristic(int **puzzle, double *heuristic,
    int depth, std::string heuristicType)
{
    *heuristic = static_cast<double>(depth);
    (void)puzzle;
    (void)heuristicType;
} //TODO:

void        rec(Node *node, std::string *toPrint, int size, int level)
{
    if (level >= size)
        return;
    for (int i = 0; i < size; ++i)
    {
        if (node->tab[i] != nullptr)
        {
            toPrint[level] += " ";
            toPrint[level] += std::to_string(static_cast<int>(node->tab[i]->heuristic));
            rec(node->tab[i], toPrint, size, level + 1);
            if (level == size - 1)
            {
                if (node->tab[i]->heuristic > 9)
                    toPrint[level + 1] += " ";
                toPrint[level + 1] += " ";
                toPrint[level + 1] += std::to_string(node->tab[i]->needToCheck);
                toPrint[level + 2] += " ";
                toPrint[level + 2] += std::to_string(static_cast<int>(node->tab[i]->heuristic));
            }
        }
    }
    if (level == size - 1)
    {
        toPrint[level + 1] += "|";
        toPrint[level + 2] += "|";
    }
    toPrint[level] += "|";
}

void        showTree(Node *base, const int size)
{
    std::string  *toPrint = new std::string[size + 2];
    for (int i = 0; i < size + 2; ++i)
        toPrint[i] = "";
    rec(base, toPrint, size, 0);
    toPrint[size] += " <= needToCheck (1 = true, 0 = false)";
    toPrint[size + 1] += " <= heuristic value";
    for (int i = 0; i < size + 2; ++i)
        std::cout << toPrint[i] << std::endl;
    delete[] toPrint;
}

void        NodeSolver::addSolution(void)
{
    int **solutionTab = new int*[this->_size];
    for(int i = 0; i < this->_size; ++i)
    {
        solutionTab[i] = new int[this->_size];
        for(int j = 0; j < this->_size; ++j)
            solutionTab[i][j] = 0;
    }

    int i = 1;
    int dirX = 1, dirY = 0, dir = 0;
    int x = 0, y = 0;
    solutionTab[y][x] = i++;
    while (i < _size * _size) {
        while (y + dirY < _size && y + dirY >= 0 && x + dirX < _size && x + dirX >= 0
        && solutionTab[y + dirY][x + dirX] == 0) {
            y += dirY;
            x += dirX;
            solutionTab[y][x] = i++;
        }
        dir = (dir == 3) ? 0 : dir + 1;
        dirX = (dir == 0) ? 1 : ((dir == 2) ? -1 : 0);
        dirY = (dir == 1) ? 1 : ((dir == 3) ? -1 : 0);
    }
    this->_solutionTab = solutionTab;
    //int treeDepth;
    //this->_base->throwSearch(solutionTab, &treeDepth)->addPuzzleToTree(solutionTab, treeDepth, 0, 0);
}

bool        NodeSolver::checkifsolution(int **puzzle)
{
    for (int i = 0; i < this->_size; ++i)
        for (int j = 0; j < this->_size; ++j)
            if (puzzle[i][j] != this->_solutionTab[i][j])
                return false;
    return true;
}

Node        *NodeSolver::solve(std::string heuristicType, std::string searchType)
{
    int     **puzzle;
    int     treeDepth;
    double  heuristic;
    int     depth = 0;

    puzzle = new int*[this->_size];
    for (int i = 0; i < this->_size; ++i)
        puzzle[i] = new int[this->_size];

    std::cout << std::endl;
    while(1)
    {

        Node    *best = this->getBestPuzzle();
        best->closeNode();
        ++this->currentClosed;
        this->convertNodeToTable(best, puzzle);
        //showTree(this->_base, this->_size * this->_size);
        /*
        for (int i = 0; i < this->_size; ++i)
        {
            for (int j = 0; j < this->_size; ++j)
                std::cout << puzzle[i][j];
            std::cout << std::endl;
        }
        std::cout << "best = " << static_cast<void*>(best) << std::endl;
        */
        for (int k = 0; k < 4; ++k)
        {
            if (genNextPuzzle(puzzle) == false)
                continue;
            
            if (this->checkifsolution(puzzle) == true)
            {
                /*
                std::cout << "all good" << std::endl;
                for (int i = 0; i < this->_size; ++i)
                {
                    for (int j = 0; j < this->_size; ++j)
                        std::cout << puzzle[i][j];
                    std::cout << std::endl;
                }
                */
                std::cout << "total opened => " << this->totalOpenedEver << std::endl;
                std::cout << "current closed => " << this->currentClosed << std::endl;
                std::cout << "total states => " << this->totalStatesEver << std::endl;
                Node *tmp = this->_base->throwSearch(puzzle, &treeDepth)->addPuzzleToTree(puzzle, treeDepth, 0, 0);
                tmp->lastInSequence = best;
                for (int i = 0; i < this->_size; ++i)
                    delete[] puzzle[i];
                delete[] puzzle;
                return tmp;
            }
            Node *current = this->_base->throwSearch(puzzle, &treeDepth);
            this->calculateHeuristic(puzzle, &heuristic, depth + 1, heuristicType);
            if (treeDepth < this->_totalsize)
            {
                Node* tmp = current->addPuzzleToTree(puzzle, treeDepth, depth + 1, heuristic);
                tmp->lastInSequence = best;
                ++this->totalOpenedEver;
                ++this->totalStatesEver;
                /*
                for (int i = 0; i < this->_size; ++i)
                {
                    for (int j = 0; j < this->_size; ++j)
                        std::cout << puzzle[i][j];
                    std::cout << std::endl;
                }
                std::cout << "(adding) ";
                std::cout << "current = " << static_cast<void*>(current);
                std::cout << ", best = " << static_cast<void*>(best) << std::endl;
                */
            }
            else if (current->heuristic > heuristic)
            {
                current->updateBranchToTop(heuristic, depth);
                current->lastInSequence = best;
                ++this->totalOpenedEver;
                --this->currentClosed;
                /*
                for (int i = 0; i < this->_size; ++i)
                {
                    for (int j = 0; j < this->_size; ++j)
                        std::cout << puzzle[i][j];
                    std::cout << std::endl;
                }
                std::cout << "(modifying) ";
                std::cout << "current = " << static_cast<void*>(current);
                std::cout << ", best = " << static_cast<void*>(best) << std::endl;
                */
            }
            /*
            else
            {
                std::cout << "copy found but didn't add to tree because heuristic was too big" << std::endl;
            }
            
            std::cout << "treeDepth = " << treeDepth << std::endl;
            std::cout << std::endl;
            */
        }
        //showTree(this->_base, this->_size * this->_size);
        //std::cout << "end of loop" << std::endl << std::endl;
        ++depth;
    
    }    
    
    for (int i = 0; i < this->_size; ++i)
        delete[] puzzle[i];
    delete[] puzzle;
    
    (void)heuristicType;
    (void)searchType;
    std::cout << "wtf dude" << std::endl;
    return nullptr;
}