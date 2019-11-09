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
    this->_original = NULL;
    delete this->_solution;
    this->_solution = NULL;

    for (std::vector<Puzzle*>::iterator it = this->_openList.begin();
        it < this->_openList.end(); ++it)
    {
        delete *it;
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

    this->_openList.push_back(new Puzzle(*(this->_original)));

    while (1)
    {
        //std::cout << (std::to_string(this->_openList.size())
        //    + ", " + std::to_string(this->_closedList.size())) << std::endl;
        open = this->_openList.front();
        this->_openList.erase(this->_openList.begin());
        this->_closedList.push_back(open);

        if (*open == *(this->_solution))
            return open;
        
        //open->printPuzzle();
        std::vector<Puzzle*>    generated = open->generatePuzzleFromPosition();
        for (std::vector<Puzzle*>::iterator it = generated.begin();
            it < generated.end(); ++it)
        {
            if (open->getPrevPuzzle() != NULL
                && *(open->getPrevPuzzle()) == **it)
            {
                delete (*it);
                *it = NULL;
                continue;
            }
            int i = checkIfInClosedList(*it);

            (*it)->setHeuritic(this->calcHeuristic(*it, heuristicType));
            if (i == -1)
            {
                addToOpenList(*it, searchType);
            }
            else
            {
                if (this->_closedList.at(i)->getHeuristic() <= (*it)->getHeuristic())
                {
                    delete (*it);
                    *it = NULL;
                }
                else
                {
                    delete this->_closedList.at(i);
                    this->_closedList.at(i) = NULL;
                    this->_closedList.erase(this->_closedList.begin() + i);
                    addToOpenList(*it, searchType);
                }
            }
        }
    }
    //TODO: operations info (max states at a time, total open states...)
}

int     Solver::calcHeuristic(Puzzle *puzzle, std::string heauristicType)
{
    int **tmp = puzzle->getPuzzle();
    int **sol = this->_solution->getPuzzle();

    if (heauristicType.compare("BFS") == 0)
        return 0 + puzzle->getDepth();
    else if (heauristicType.compare("A* Tiles-out") == 0)
    {
        int score = 0;
        for (int y = 0; y < this->_size; ++y)
            for (int x = 0; x < this->_size; ++x)
                if (tmp[y][x] != sol[y][x])
                    ++score;
        return puzzle->getDepth() + score;
    }
    else if (heauristicType.compare("A* Manhattan distance") == 0)
    {
        int score = 0;
        for (int y = 0; y < this->_size; ++y)
        {
            for (int x = 0; x < this->_size; ++x)
            {
                int tmpy, tmpx;
                this->_solution->findNumberinPuzzle(tmp[y][x], &tmpx, &tmpy);
                score += abs(tmpy - y) + abs(tmpx - x);
            }
        }
        return puzzle->getDepth() + score;
    }
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
        if (**it == *puzzle)
            return (it - this->_closedList.begin());
    }
    return (-1);
}

int     Solver::checkIfInOpenList(Puzzle *puzzle)
{
    for (std::vector<Puzzle*>::iterator it = this->_openList.begin();
        it < this->_openList.end(); ++it)
    {
        if (**it == *puzzle)
            return (it - this->_openList.begin());
    }
    return (-1);
}
void    Solver::addToOpenList(Puzzle *puzzle, std::string searchType)
{
    if (puzzle == NULL)
        throw std::runtime_error("Tried to add a NULL pointer to open list");
    
    std::vector<Puzzle*>::iterator it = this->_openList.begin();
    while (it < this->_openList.end()
        && (*it)->getHeuristic() <= puzzle->getHeuristic())
        ++it;
    this->_openList.insert(it, puzzle);
    /*
    std::cout << ">>>>>";
    if (puzzle->getSize() != this->_size)
    {
        std::cout << puzzle->getSize() << std::endl;
    }
    std::cout << "<<<<" << std::endl;
    */
    (void)searchType; //TODO: for greedy and uniform search
}