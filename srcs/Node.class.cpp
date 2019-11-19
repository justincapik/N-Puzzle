#include <cmath>
#include "Node.class.hpp"

Node::Node(const int size):
size(size)
{
    this->tab = new Node*[size * size];
    for (int i = 0; i < size * size; ++i)
        this->tab[i] = nullptr;
    
    this->prev = nullptr;
    this->needToCheck = true;
    this->heuristic = -1.;
    this->depth = -1;
    this->lastInSequence = nullptr;
}

Node::~Node()
{
    /*
    for (int i = 0; i < this->size * this->size; ++i)
    {
        if (this->tab[i] == nullptr)
            std::cout << "_ ";
        else
            std::cout << this->heuristic << " "
    }
    std::cout << std::endl;
    */
    for (int i = 0; i < this->size * this->size; ++i)
    {
        if (this->tab[i] != nullptr)
            delete tab[i];
    }
    delete this->tab; 
}

Node::Node(Node const & instance):
size(instance.size)
{
    this->depth = instance.depth;
    this->heuristic = instance.heuristic;
    this->needToCheck = instance.needToCheck;
    this->prev = nullptr;
    
    this->tab = new Node*[size * size];
    for (int i = 0; i < size * size; ++i)
        this->tab[i] = nullptr;

}

Node        *Node::throwSearch(int **puzzle, int *treeDepth)
{
    Node *tmp = this;
    *treeDepth = 0;

    while (*treeDepth < this->size * this->size //TODO: THIS SHOULDN'T HAVE TO BE HERE
        && tmp->tab[puzzle[static_cast<int>(std::floor(*treeDepth / this->size))][*treeDepth % this->size]] != nullptr)
    {
        tmp = tmp->tab[puzzle[static_cast<int>(std::floor(*treeDepth / this->size))][*treeDepth % this->size]];
        ++(*treeDepth);
    }
    return tmp;
}

Node        *Node::addPuzzleToTree(int **puzzle, int treeDepth, int depth, double heuristic)
{
    Node    *currentDown = this;
    Node    *tmp;

    // Adding new branche(s)
    for (int i = treeDepth; i < this->size * this->size; ++i)
    {
        tmp = new Node(*currentDown);
        currentDown->tab[puzzle[static_cast<int>(std::floor(i / this->size))][i % this->size]] = tmp;
        tmp->prev = currentDown;
        currentDown = tmp;
        currentDown->needToCheck = true;
        currentDown->depth = depth;
        currentDown->heuristic = heuristic;
    }

    // Updating above branche(s)
    Node    *currentUp = this;
    for (int i = treeDepth; i >= 0; ++i)
    {
        if (currentUp->needToCheck == false
            || currentUp->heuristic > heuristic)
        {
            currentUp->heuristic = heuristic;
            currentUp->depth = depth;
            currentUp->needToCheck = true;
        }
        else
            return currentDown;
        currentUp = currentUp->prev;
    }

    return currentDown;
}

void        Node::closeNode(void)
{
    Node    *prev = this->prev;
    Node    *current = this;

    this->needToCheck = false;
    for (int i = 0; i < this->size * this->size - 1; ++i)
    {
        if (prev->needToCheck == false)
            throw std::runtime_error("Encountered needToCheck=false while closing node");
        
        bool    checkIfOpened = false;
        int     bestidx = -1;
        int     bestval = INT_MAX;
        for (int j = 0; j < this->size * this->size; ++j)
        {
            if (prev->tab[j] != nullptr)
            {
                if (prev->tab[j]->needToCheck == true
                    && checkIfOpened == false)
                {
                    bestidx = j;
                    bestval = prev->tab[j]->heuristic;
                    checkIfOpened = true;
                }
                else if (checkIfOpened == false
                    && prev->tab[j]->heuristic < bestval)
                {
                    bestidx = j;
                    bestval = prev->tab[j]->heuristic;
                }
                else if (checkIfOpened == true
                    && prev->tab[j]->needToCheck == true
                    && prev->tab[j]->heuristic < bestval)
                {
                    bestidx = j;
                    bestval = prev->tab[j]->heuristic;
                }
            }
        }
        if (bestidx == -1 || bestval == INT_MAX)
            throw std::runtime_error("Something went horribly wrong while closing a node");
        prev->heuristic = bestval;
        prev->depth = prev->tab[bestidx]->depth; 
        prev->needToCheck = checkIfOpened;
        current = prev;
        prev = prev->prev;
    }
}

void        Node::updateBranchToTop(double heuristic, int depth)
{
    Node    *current = this->prev;

    this->heuristic = heuristic;
    this->depth = depth;
    this->needToCheck = true;
    for (int i = 0; i < this->size * this->size - 1; ++i)
    {
        if (current->needToCheck == false
            || current->heuristic > heuristic)
        {
            current->heuristic = heuristic;
            current->depth = depth;
            current->needToCheck = true;
        }
        else
            return ;
        current = current->prev;
    }
}