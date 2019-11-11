#include <cmath>
#include "Node.class.hpp"

Node::Node(const int size):
size(size)
{
    this->tab = new Node*[size * size];
    for (int i = 0; i < size * size; ++i)
        this->tab[i] = NULL;
    
    this->prev = NULL;
    this->needToCheck = true;
    this->heuristic = -1;
    this->depth = -1;
}

Node::~Node()
{
    /*
    for (int i = 0; i < this->size * this->size; ++i)
    {
        if (this->tab[i] == NULL)
            std::cout << "_ ";
        else
            std::cout << this->heuristic << " "
    }
    std::cout << std::endl;
    */
    for (int i = 0; i < this->size * this->size; ++i)
    {
        if (this->tab[i] != NULL)
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
    this->prev = NULL;
    
    this->tab = new Node*[size * size];
    for (int i = 0; i < size * size; ++i)
        this->tab[i] = NULL;

}

Node        *Node::throwSearch(int **puzzle, int *treeDepth)
{
    Node *tmp = this;
    *treeDepth = 0;

    while (*treeDepth < this->size * this->size
        && tmp->tab[puzzle[static_cast<int>(std::floor(*treeDepth / this->size))][*treeDepth % this->size]] != NULL)
    {
        tmp = tmp->tab[puzzle[static_cast<int>(std::floor(*treeDepth / this->size))][*treeDepth % this->size]];
        ++(*treeDepth);
    }
    return tmp;
}

void        Node::addPuzzleToTree(int **puzzle, int treeDepth, int depth, int heuristic)
{
    Node *current = this;
    Node *tmp;
    
    for (int i = treeDepth; i < this->size * this->size; ++i)
    {
        tmp = new Node(*current);
        current->tab[puzzle[static_cast<int>(std::floor(i / this->size))][i % this->size]] = tmp;
        tmp->prev = current;
        current = tmp;
        current->needToCheck = true;
        current->depth = depth;
        current->heuristic = heuristic;
    }    
}

void        Node::closeNode(void)
{
    Node    *prev = this->prev;

    Node    *tmp = this;

    this->needToCheck = false;
    this->heuristic = -1;
    for (int i = this->size * this->size - 1; i >= 0; ++i)
    {
        /*
        std::cout << static_cast<void*>(tmp) << "=> ";
        for (int j = 0; j < this->size * this->size; ++j)
            std::cout << static_cast<void*>(prev->tab[j]) <<
                ((prev->tab[j] != NULL) ? ("(" + std::to_string(prev->tab[j]->heuristic) + ") "): " ");
        std::cout << std::endl;
        */
        for (int j = 0; j < this->size * this->size; ++j)
        {
            if (prev->tab[j] != NULL && prev->tab[j] != this &&
                (prev->tab[j]->needToCheck == true
                || prev->tab[j]->heuristic < this->heuristic))
                return;
        }
        prev->heuristic = this->heuristic;
        prev->needToCheck = false;
        if (prev->heuristic > this->heuristic)
            prev->heuristic = this->heuristic;
        tmp = prev;
        prev = prev->prev;
    }
}