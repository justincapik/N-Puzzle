#include <iostream>
#include <stdexcept>
#include <valarray>
#include <iomanip>

#include "PRQSolver.class.hpp"
#include "PRQ.class.hpp"
#include "PRQPuzzle.class.hpp"

// TODO: list
// change generatePuzzleFromPosition to send std::vector

PRQSolver::PRQSolver(int **original, int size):
    _size(size)
{
    this->_solutionPuzzle = this->genSolution();
    this->_openList.add(new PRQPuzzle(original, size));
    if (!this->isSolvable(size, original))
    {
        delete this->_solutionPuzzle;
        throw std::runtime_error("This puzzle is not solvable.");
    }
    this->ComplexityInSize = 0;
    this->ComplexityInTime = 0;
    this->SolutionDepth = -1;
}

PRQSolver::~PRQSolver()
{
    std::cout << "destructor called" << std::endl;
    delete this->_solutionPuzzle;
}

PRQPuzzle    *PRQSolver::solve(std::string heuristicType, std::string searchType)
{
    PRQPuzzle      *best;

    if (this->_openList.getTop()->compare(*(this->_solutionPuzzle)))
        return this->_openList.getTop();
    while (this->_openList.getTop() != nullptr)
    {

        if (this->ComplexityInTime % 10000 < 50)
        {
            std::cout << "\r";
            std::cout << "complexity in time = " << this->ComplexityInTime;
            std::cout << ", complexity in size = " << this->ComplexityInSize;
        }
        best = this->getNextAndUpdateOpenList();
        ++this->ComplexityInTime;

        std::vector<PRQPuzzle*>  generated = best->generatePuzzleFromPosition();

        PRQPuzzle   *result = nullptr;
        result = this->algoDecisions(generated, heuristicType, searchType, best);
        if (result != nullptr)
        {
            std::cout << "\r                                                                   \r";
            this->addToClosedList(best);
            this->addToClosedList(result);
            return result;
        }
        this->addToClosedList(best);
    }
    std::cout << "\r                                                                   \r";
    std::cout << "No possible solution" << std::endl;
    return nullptr;
}

PRQPuzzle   *PRQSolver::algoDecisions(std::vector<PRQPuzzle*> generated,
    std::string heuristicType, std::string searchType,PRQPuzzle *best)
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
            PRQPuzzle   *ret = *it;
            ++it;
            while (it < generated.end())
            {
                delete (*it);
                (*it) = nullptr;
                ++it;
            }
            return (ret);
        }

        PRQPuzzle   *closed;
        (*it)->setHeuritic(this->calcHeuristic((*it), heuristicType, searchType));
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


double     PRQSolver::calcHeuristic(PRQPuzzle *puzzle, std::string heauristicType,
    std::string searchType)
{
    int **tmp = puzzle->getPuzzle();
    int **sol = this->_solutionPuzzle->getPuzzle();

    double score = 0;
    if (heauristicType.compare("BFS") == 0) //Breadth First Search
        ;
    else if (heauristicType.compare("AHD") == 0) //A* Hamming distance
    {
        for (int y = 0; y < this->_size; ++y)
            for (int x = 0; x < this->_size; ++x)
                if (tmp[y][x] != sol[y][x])
                    ++score;
    }
    else if (heauristicType.compare("AMD") == 0) //A* Manhattan Distance
    {
        for (int y = 0; y < this->_size; ++y)
        {
            for (int x = 0; x < this->_size; ++x)
            {
                if (tmp[y][x] != 0)
                {
                    int tmpy, tmpx;
                    this->_solutionPuzzle->findNumberinPuzzle(tmp[y][x], &tmpx, &tmpy);
                    score += abs(tmpy - y) + abs(tmpx - x);
                }
            }
        }
    }
    else if (heauristicType.compare("AED") == 0) // A* Euclidian distance
    {
        for (int y = 0; y < this->_size; ++y)
        {
            for (int x = 0; x < this->_size; ++x)
            {
                if (tmp[y][x] != 0)
                {
                    int tmpy, tmpx;
                    this->_solutionPuzzle->findNumberinPuzzle(tmp[y][x], &tmpx, &tmpy);
                    score += std::sqrt((tmpy - y) * (tmpy - y) + (tmpx - x) * (tmpx - x));
                }
            }
        }
    }
    else
        throw std::runtime_error(std::string("Invalide heuristic name"));
    if (searchType.compare("UCS") == 0)
        score += puzzle->getDepth();
    else if (searchType.compare("GS") != 0)
        throw std::runtime_error("Invalide search type name");
    return score;
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
    PRQPuzzle *solution = new PRQPuzzle(solutionTab, this->_size);
    return solution;
}

/* À modifier à partir d'ici */
bool    PRQSolver::checkIfInClosedList(PRQPuzzle *generated, PRQPuzzle **closed)
{
    if ((*closed = this->_closedList.isInTree(generated)) == nullptr)
        return false;
    else
        return true;
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
    this->_closedList.add(puzzle);
    ++this->ComplexityInSize;
}

PRQPuzzle   *PRQSolver::getNextAndUpdateOpenList(void)
{
    --this->ComplexityInSize;
    return this->_openList.pop_head();
}

void        PRQSolver::deleteFromClosedList(PRQPuzzle *prevClosed)
{
    this->_closedList.remove(prevClosed);
    --this->ComplexityInSize;
}

void        PRQSolver::deleteFromOpenList(PRQPuzzle *prevOpen)
{
    (void)prevOpen;
    throw std::runtime_error("Error: called deletFromOpenList()");
}

bool        PRQSolver::isSolvable(int size, int **original) {
    int p0s = 0;
    int convert[size * size];
    int **solution = this->_solutionPuzzle->getPuzzle();

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (solution[y][x] != 0)
                convert[solution[y][x] - 1] = original[y][x];
            else
                convert[size * size - 1] = original[y][x];
            if (original[y][x] == 0)
                p0s = size - y;
        }
    }

    int inver = 0;
    for (int i = 0; i < size * size; i++) {
        for (int j = i + 1; j < size * size; j++) {
            if (convert[i] > convert[j])
                inver++;
            if (convert[i] == convert[j])
                throw std::runtime_error("Invalid file format.");
        }

    }

    if (size % 2 == 0 && p0s % 2 != 0)
        return (inver % 2 != 0);
    return (inver % 2 == 0);
}
