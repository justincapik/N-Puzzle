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
    for (int i = 0; i < this->size * this->size; ++i)
    {
        if (this->tab[i] == NULL)
            std::cout << "NULL, ";
        else
            std::cout << static_cast<void*>(this->tab[i]) << ", ";
    }
    std::cout << std::endl;
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