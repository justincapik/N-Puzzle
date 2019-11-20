#include <iostream>
#include <stdexcept>
#include <valarray>
#include "PRQSolver.class.hpp"

PRQSolver::PRQSolver(int **original, int size):
    _size(size)
{
    this->_solutionPuzzle = this->genSolution();
    this->_openList = new PRQPuzzle(original, size);
    this->_closedList = nullptr;

    this->timeTaken = 0.;
    this->totalOperations = 0;
    this->totalOpenStates = 0;
    this->maxStatesAtATime = 0;
}

PRQSolver::~PRQSolver()
{
    for (int i = 0; i < this->_size; ++i)
    {
        delete this->_solution[i];
        this->_solution[i] = nullptr;
    }
    delete this->_solution;
    this->_solution = nullptr;

    PRQPuzzle   *next;
    PRQPuzzle   *current = this->_openList;
    while (current != nullptr)
    {
        next = current->next;
        delete current;
        current = next;
    }
    
    current = this->_closedList;
    while (current != nullptr)
    {
        next = current->next;
        delete current;
        current = next;
    }
}

PRQPuzzle    *PRQSolver::solve(std::string heuristicType, std::string searchType)
{
    PRQPuzzle      *open;

    for (int i = 0; i < 5; ++i)
    //while (this->_openList != nullptr)
    {
        std::cout << "starting loop" << std::endl;
        std::cout << "1" << std::endl;
        open = this->_openList;
        this->_openList = this->_openList->next;
        std::cout << "2" << std::endl;

        //TODO: check directly at generated puzzles, watch if solution is also start
        if (*open == *(this->_solutionPuzzle))
            return open;

        std::cout << "3" << std::endl;
        PRQPuzzle*  generated = open->generatePRQPuzzleFromPosition();
        PRQPuzzle*  tmpGenerated;
        std::cout << "4" << std::endl;
        while (generated != nullptr)
        {
            /*
            if (open->getPrevPRQPuzzle() != nullptr
                && *(open->getPrevPRQPuzzle()) == **it)
            {
                delete (*it);
                *it = nullptr;
                continue;
            } //optimization
            */
            tmpGenerated = generated;
            PRQPuzzle*  prevClosed = nullptr;
            PRQPuzzle*  prevOpened = nullptr;
            PRQPuzzle*  closed = nullptr;
            PRQPuzzle*  opened = nullptr;

            generated->setHeuritic(this->calcHeuristic(generated, heuristicType));
            std::cout << "5" << std::endl;
            if (checkIfInClosedList(generated, &closed, &prevClosed))
            {
                std::cout << "closed" << std::endl;
                if (closed->getHeuristic() <= generated->getHeuristic())
                {
                    delete generated;
                    generated = nullptr;
                }
                else
                {
                    prevClosed->next = closed->next;
                    this->addToOpenList(generated, searchType);
                }
            }
            else if (checkIfInOpenList(generated, &opened, &prevOpened))
            {
                std::cout << "open" << std::endl;
                if (opened->getHeuristic() <= generated->getHeuristic())
                {
                    delete generated;
                    generated = nullptr;
                }
                else
                {
                    prevOpened->next = opened->next;
                    this->addToOpenList(generated, searchType);
                }
            }
            else
            {
                std::cout << "adding to open list" << std::endl;
                this->addToOpenList(generated, searchType);
            }
            std::cout << "6" << std::endl;
            generated = tmpGenerated->next;
            std::cout << "7" << std::endl;
        }
        std::cout << "finished exploring possibilities" << std::endl;
        this->addToClosedList(open);
    }
    //TODO: operations info (max states at a time, total open states...)
    return nullptr;
}

int     PRQSolver::calcHeuristic(PRQPuzzle *puzzle, std::string heauristicType)
{
    int **tmp = puzzle->getPRQPuzzle();
    int **sol = this->_solutionPuzzle->getPRQPuzzle();

    if (heauristicType.compare("BFS") == 0) //Breadth First Search
        return 0 + puzzle->getDepth();
    else if (heauristicType.compare("ATO") == 0) //A* Tiles-out
    {
        int score = 0;
        for (int y = 0; y < this->_size; ++y)
            for (int x = 0; x < this->_size; ++x)
                if (tmp[y][x] != sol[y][x])
                    ++score;
        return puzzle->getDepth() + score;
    }
    else if (heauristicType.compare("AMD") == 0) //A* Manhattan Distance
    {
        int score = 0;
        for (int y = 0; y < this->_size; ++y)
        {
            for (int x = 0; x < this->_size; ++x)
            {
                int tmpy, tmpx;
                this->_solutionPuzzle->findNumberinPRQPuzzle(tmp[y][x], &tmpx, &tmpy);
                score += abs(tmpy - y) + abs(tmpx - x);
            }
        }
        return puzzle->getDepth() + score;
    }
    else if (heauristicType.compare("AED") == 0) // A* Euclidian distance
    {
        int score = 0;
        for (int y = 0; y < this->_size; ++y)
        {
            for (int x = 0; x < this->_size; ++x)
            {
                int tmpy, tmpx;
                this->_solutionPuzzle->findNumberinPRQPuzzle(tmp[y][x], &tmpx, &tmpy);
                score += std::sqrt((tmpy - y) * (tmpy - y) + (tmpx - x) * (tmpx - x));
            }
        }
        return puzzle->getDepth() + score;
    }
    else
        throw std::runtime_error(std::string("Invalide heuristic name"));
}

PRQPuzzle  *PRQSolver::genSolution(void)
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
    this->_solution = solutionTab;
    PRQPuzzle *solution = new PRQPuzzle(solutionTab, this->_size);
    return solution;
}

bool    PRQSolver::checkIfInClosedList(PRQPuzzle *generated, PRQPuzzle **closed,
    PRQPuzzle **prevClosed)
{
    PRQPuzzle   *tmp = this->_closedList;
    PRQPuzzle   *prev;

    std::cout << "checking closed list" << std::endl;
    while (tmp != nullptr)
    {
        if (*tmp == *generated)
        {
            *closed = tmp;
            *prevClosed = prev;
            return true;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    return false;
}

bool    PRQSolver::checkIfInOpenList(PRQPuzzle *generated, PRQPuzzle **opened,
    PRQPuzzle **prevOpened)
{
    PRQPuzzle   *tmp = this->_openList;
    PRQPuzzle   *prev;
    
    std::cout << "checking open list" << std::endl;
    while (tmp != nullptr)
    {
        if (*tmp == *generated)
        {
            *opened = tmp;
            *prevOpened = prev;
            return true;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    return false;
}
void    PRQSolver::addToOpenList(PRQPuzzle *puzzle, std::string searchType)
{
    PRQPuzzle   *tmp = this->_openList;
    PRQPuzzle   *prev = nullptr;
    
    if (puzzle == nullptr || puzzle->getHeuristic() == -1)
        throw std::runtime_error("Tried to add an invalide pointer to open list");
    if (tmp == nullptr)
    {
        this->_openList = puzzle;
        return ;
    }
    while (tmp != nullptr && tmp->getHeuristic() > puzzle->getHeuristic())
    {
        prev = tmp;
        tmp = tmp->next;
    }
    prev->next = puzzle;
    puzzle->next = tmp;
    (void)searchType; //TODO: for greedy and uniform search
}

void    PRQSolver::addToClosedList(PRQPuzzle *puzzle)
{
    puzzle->next = this->_closedList;
    this->_closedList = puzzle;
}