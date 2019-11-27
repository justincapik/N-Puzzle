#include <iostream>
#include <stdexcept>
#include <valarray>
#include "PRQSolver.class.hpp"
#include "PRQ.class.hpp"
#include "PRQPuzzle.class.hpp"

// TODO: list
// change generatePuzzleFromPosition to send std::vector

PRQSolver::PRQSolver(int **original, int size):
    _size(size)
{
    this->_openList.add(new PRQPuzzle(original, size));
    this->_solutionPuzzle = this->genSolution();

    this->ComplexityInSize = 0;
    this->ComplexityInTime = 0;
    this->SolutionDepth = -1;
}

PRQSolver::~PRQSolver()
{
    delete this->_solutionPuzzle;

    //no need to call open list destructor
    //TODO:
}

PRQPuzzle    *PRQSolver::solve(std::string heuristicType, std::string searchType)
{
    PRQPuzzle      *best;

    //printf("\e[?25l");

    if (this->_openList.getTop()->compare(*(this->_solutionPuzzle)))
        return this->_openList.getTop();
    while (this->_openList.getTop() != nullptr)
    {
        if (this->ComplexityInTime % 100000 < 50)
        {
            std::cout << "\r";
            std::cout << "complexity in time = " << this->ComplexityInTime;
            std::cout << ", complexity in size = " << this->ComplexityInSize;
        }
        best = this->getNextAndUpdateOpenList();
        ++this->ComplexityInTime;

        std::vector<PRQPuzzle*>  generated = best->generatePuzzleFromPosition();

        PRQPuzzle   *result = nullptr;
        if (searchType.compare("UCS") == 0)
            result = this->uniformCostSearch(generated, heuristicType, best);
        else if (searchType.compare("GS") == 0)
            result = this->greedySearch(generated, heuristicType, best);
        else
            throw std::runtime_error("Invalide search type name (UCS or GS)");
        if (result != nullptr)
        {
            std::cout << "\r                                                                   \r";
            printf("\e[?25h");
            return result;
        }
        this->addToClosedList(best);
    }
    //TODO: operations info (max states at a time, total open states...)
    std::cout << "\r                                                                   \r";
    std::cout << "No possible solution" << std::endl;
    //printf("\e[?25h");
    return nullptr;
}

PRQPuzzle   *PRQSolver::uniformCostSearch(std::vector<PRQPuzzle*> generated,
    std::string heuristicType, PRQPuzzle *best)
{
    for (std::vector<PRQPuzzle*>::iterator it = generated.begin();
        it < generated.end(); ++it)
    {
        /*
        if (best->prevInSolution != nullptr
            && best->prevInSolution->compare(**it))
        {
            delete (*it);
            *it = nullptr;
            continue;
        } //optimization
        */

        if ((*it)->compare(*(this->_solutionPuzzle)))
        {
            (*it)->prevInSolution = best;
            return (*it);
        }

        PRQPuzzle   *closed;
        (*it)->setHeuritic(this->calcHeuristic((*it), heuristicType));
        if (checkIfInClosedList(*it, &closed))
        {
            if (closed->getHeuristic() <= (*it)->getHeuristic())
            {
                delete (*it);
                (*it) = nullptr;
            }
            else
            {
                this->deleteFromClosedList(closed);
                this->addToOpenList((*it));
                (*it)->prevInSolution = best;
            }

        }
        else
        {
            this->addToOpenList((*it));
            (*it)->prevInSolution = best;
        }
    }
    return nullptr;
}

PRQPuzzle   *PRQSolver::greedySearch(std::vector<PRQPuzzle*> generated,
    std::string heuristicType, PRQPuzzle *best)
{
    /*
    PRQPuzzle   *bestpuzzle = nullptr;
    int          bestval = INT_MIN;

    for (std::vector<PRQPuzzle*>::iterator it = generated.begin();
        it < generated.end(); ++it)
    {
        if ((*it)->compare(*(this->_solutionPuzzle)))
        {
            (*it)->prevInSolution = best;
            return (*it);
        }
        
        (*it)->setHeuritic(this->calcHeuristic((*it), heuristicType));
        PRQPuzzle *closed;
        if (this->checkIfInClosedList(*it, &closed))
        {
            if (closed->getHeuristic() <= (*it)->getHeuristic())
            {
                delete (*it);
                (*it) = nullptr;
            }
            else
            {
                if (closed->getHeuristic() < bestval)
                {
                    
                }
            }

        }
        
    }
    this->addToOpenList(bestpuzzle);
    bestpuzzle->prevInSolution = best;
    return nullptr;
    */
    (void)generated;
    (void)heuristicType;
    (void)best;
    throw std::runtime_error("greedy search function not done yet"); //TODO:
    return nullptr;
}

double     PRQSolver::calcHeuristic(PRQPuzzle *puzzle, std::string heauristicType)
{
    int **tmp = puzzle->getPuzzle();
    int **sol = this->_solutionPuzzle->getPuzzle();

    if (heauristicType.compare("BFS") == 0) //Breadth First Search
        return 0 + puzzle->getDepth();
    else if (heauristicType.compare("AHD") == 0) //A* Tiles-out
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
                this->_solutionPuzzle->findNumberinPuzzle(tmp[y][x], &tmpx, &tmpy);
                score += abs(tmpy - y) + abs(tmpx - x);
            }
        }
        return puzzle->getDepth() + score;
    }
    else if (heauristicType.compare("AED") == 0) // A* Euclidian distance
    {
        double score = 0;
        for (int y = 0; y < this->_size; ++y)
        {
            for (int x = 0; x < this->_size; ++x)
            {
                int tmpy, tmpx;
                this->_solutionPuzzle->findNumberinPuzzle(tmp[y][x], &tmpx, &tmpy);
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
    this->_solution = solutionTab;
    PRQPuzzle *solution = new PRQPuzzle(solutionTab, this->_size);
    return solution;
}

/* À modifier à partir d'ici */
bool    PRQSolver::checkIfInClosedList(PRQPuzzle *generated, PRQPuzzle **closed)
{
    //std::cout << "check closed list" << std::endl;
    if ((*closed = this->_closedList.isInTree(generated)) == nullptr)
    {
        //std::cout << "check closed list end" << std::endl;
        return false;
    }
    else
    {
        //std::cout << "check closed list end" << std::endl;
        return true;
    }
}

//DO NOT USED (inefficient)
bool    PRQSolver::checkIfInOpenList(PRQPuzzle *generated)
{
    (void)generated;
    throw std::runtime_error("why the hell is checkIfInOpenListbeing used");
}

void    PRQSolver::addToOpenList(PRQPuzzle *puzzle)
{
    this->_openList.add(puzzle);
    ++this->ComplexityInSize;
}

void    PRQSolver::addToClosedList(PRQPuzzle *puzzle)
{
    //std::cout << "add to closed" << std::endl;
    this->_closedList.add(puzzle);
    ++this->ComplexityInSize;
    //std::cout << "add to closed end" << std::endl;
}

PRQPuzzle   *PRQSolver::getNextAndUpdateOpenList(void)
{
    --this->ComplexityInSize;
    return this->_openList.pop_head();
}

void        PRQSolver::deleteFromClosedList(PRQPuzzle *prevClosed)
{
    //std::cout << "remove from closed" << std::endl;
    this->_closedList.remove(prevClosed);
    --this->ComplexityInSize;
    //std::cout << "remove from closed end" << std::endl;
}

void        PRQSolver::deleteFromOpenList(PRQPuzzle *prevOpen)
{
    (void)prevOpen;
    throw std::runtime_error("Error: called deletFromOpenList()");
}
