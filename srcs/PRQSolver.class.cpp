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

    this->timeTaken = 0.;
    this->totalOperations = 0;
    this->totalOpenStates = 0;
    this->maxStatesAtATime = 0;
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

    if (this->_openList.getTop()->compare(*(this->_solutionPuzzle)))
        return this->_openList.getTop();
    //while (this->_openList.getTop() != nullptr)
    for (int i = 0; i < 50 && this->_openList.getTop() != nullptr; ++i)
    {
        best = this->getNextAndUpdateOpenList();

        std::vector<PRQPuzzle*>  generated = best->generatePuzzleFromPosition();

        PRQPuzzle   *result = nullptr;
        if (searchType.compare("UCS") == 0)
            result = this->uniformCostSearch(generated, heuristicType, best);
        else if (searchType.compare("GS") == 0)
            result = this->greedySearch(generated, heuristicType, best);
        else
            throw std::runtime_error("Invalide search type name (UCS or GS)");
        if (result != nullptr)
            return result;
        this->addToClosedList(best);
    }
    //TODO: operations info (max states at a time, total open states...)
    std::cout << "well" << std::endl;
    return nullptr;
}

PRQPuzzle   *PRQSolver::uniformCostSearch(std::vector<PRQPuzzle*> generated,
    std::string heuristicType, PRQPuzzle *best)
{
    for (std::vector<PRQPuzzle*>::iterator it = generated.begin();
        it < generated.end(); ++it)
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
                this->deleteFromClosedList(*it);
                this->addToOpenList((*it));
                (*it)->prevInSolution = best;
            }

        }
        else
        {
            this->addToOpenList((*it));
            (*it)->prevInSolution = best;
        }
        /*
        else if (checkIfInOpenList((*it)))
        {
            if (opened->getHeuristic() <= (*it)->getHeuristic())
            {
                    && VALUE(LEFT_CHILD(tmp)]->getHeuristic() < VALUE(tmp]->getHeuristic())
                (*it) = nullptr;
            }
            else
            {
                this->deleteFromOpenList(prevOpened);
                this->addToOpenList((*it));
                (*it)->prevInSolution = best;
            }
        }
        */
    }
    return nullptr;
}

PRQPuzzle   *PRQSolver::greedySearch(std::vector<PRQPuzzle*> generated,
    std::string heuristicType, PRQPuzzle *best)
{
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

/* À modifier à partir d'ici */
bool    PRQSolver::checkIfInClosedList(PRQPuzzle *generated, PRQPuzzle **closed)
{
    std::cout << "check closed list" << std::endl;
    if ((*closed = this->_closedList.isInTree(generated)) == nullptr)
        return false;
    else
        return true;
    std::cout << "check closed list end" << std::endl;
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
}

void    PRQSolver::addToClosedList(PRQPuzzle *puzzle)
{
    std::cout << "add to closed" << std::endl;
    this->_closedList.add(puzzle);
    std::cout << "add to closed end" << std::endl;
}

PRQPuzzle   *PRQSolver::getNextAndUpdateOpenList(void)
{
    return this->_openList.pop_head();
}

void        PRQSolver::deleteFromClosedList(PRQPuzzle *prevClosed)
{
    std::cout << "remove from closed" << std::endl;
    this->_closedList.remove(prevClosed);
    std::cout << "remove from closed end" << std::endl;
}

void        PRQSolver::deleteFromOpenList(PRQPuzzle *prevOpen)
{
    (void)prevOpen;
}
