#include <iostream>
#include <vector>
#include <stdexcept>
#include "Solver.class.hpp"

Solver::Solver(Puzzle *original, int size):
    _original(original), _size(size)
{
    this->_solution = this->genSolution();    
}

Solver::~Solver()
{
    delete this->_original;
    delete this->_solution;

    for (std::vector<Puzzle*>::iterator it = this->_openList.begin();
        it < this->_openList.end(); ++it)
    {
        //delete *it;
        *it = NULL;
    }
    this->_openList.clear();

    for (std::vector<Puzzle*>::iterator it = this->_closedList.begin();
        it < this->_closedList.end(); ++it)
    {
        delete *it;
        *it = NULL;
    }
    this->_closedList.clear();
}

Puzzle    *Solver::solve(std::string heuristicType, std::string searchType)
{
    Puzzle      *open;

    this->_openList.push_back(this->_original);

/*
    while (1)
    {
        open = this->_openList.front();
        this->_openList.erase(this->_openList.begin());

        if (open == this->_solution)
            return open;
        
        std::vector<Puzzle*>    generated = open->generatePuzzleFromPosition();
        for (std::vector<Puzzle*>::iterator it = generated.begin();
            it < generated.end(); ++it)
        {
            int i = checkIfInClosedList(*it);

            (*it)->setHeuritic(this->calcHeuristic(*it, heuristicType));
            if (i == -1)
                addToOpenList(*it, searchType);
            else
            {
                if (this->_closedList.at(i)->getHeuristic() <= (*it)->getHeuristic())
                    delete (*it);
                else
                {
                    delete this->_closedList.at(i);
                    this->_closedList.erase(this->_closedList.begin() + i);
                    addToOpenList(*it, searchType);
                }
            }
        }
    }
    */
    //TODO: operations info (max states at a time, total open states...)
    (void)heuristicType;
    (void)searchType;
    (void)open;
    return NULL;
}

int     Solver::calcHeuristic(Puzzle *puzzle, std::string heauristicType)
{
    if (heauristicType.compare("BFS") == 0)
        return 0 + puzzle->getDepth();
    else
        throw std::runtime_error(std::string("Invalide heuristic name"));
}

Puzzle  *Solver::genSolution(void)
{
    int **solutionTab = new int*[this->_size];
    for(int i = 0; i < this->_size; ++i)
    {
        solutionTab[i] = new int[this->_size];
        for(int j = 0; j < this->_size; ++j)
            solutionTab[i][j] = -1;
    }
    
    int i = 1, x = 0, y = 0;
    while (1)
    {
        solutionTab[y][x] = i++;
        if (x + 1 < this->_size && solutionTab[y][x + 1] == -1)
            ++x;
        else if (y + 1 < this->_size && solutionTab[y + 1][x] == -1)
            ++y;
        else if (x - 1 >= 0 && solutionTab[y][x - 1] == -1)
            --x;
        else if (y - 1 >= 0 && solutionTab[y - 1][x] == -1)
            --y;
        else
        {
            solutionTab[y][x] = 0;
            break;
        }
        
    }
    Puzzle *solution = new Puzzle(solutionTab, this->_size);
    return solution;
}

int     Solver::checkIfInClosedList(Puzzle *puzzle)
{
    for (std::vector<Puzzle*>::iterator it = this->_closedList.begin();
        it < this->_closedList.end(); ++it)
    {
        if (*it == puzzle)
            return (it - this->_closedList.begin());
    }
    return (-1);
}

void    Solver::addToOpenList(Puzzle *puzzle, std::string searchType)
{
    std::vector<Puzzle*>::iterator it = this->_openList.begin();
    while ((*it)->getHeuristic() <= puzzle->getHeuristic())
        ++it;
    this->_openList.insert(it, puzzle);
    (void)searchType; //TODO: for greedy and uniform search
}