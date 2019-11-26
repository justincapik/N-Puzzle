#include <iostream>
#include <stdexcept>
#include <valarray>
#include "LCSolver.class.hpp"

// TODO: list
// change generatePuzzleFromPosition to send std::vector
//

LCSolver::LCSolver(int **original, int size):
    _size(size)
{
    this->_solutionPuzzle = this->genSolution();
    this->_openList = new LCPuzzle(original, size);
    this->_closedList = nullptr;

    this->timeTaken = 0.;
    this->totalOperations = 0;
    this->totalOpenStates = 0;
    this->maxStatesAtATime = 0;
}

void    showlist(LCPuzzle *list, std::string name)
{
    std::cout << (name + " list {") << std::endl;
    while (list != nullptr)
    {
        std::cout << "\tlist => (" << static_cast<void*>(list) << ")" << std::endl;
        std::cout << "\tsize = " << list->getSize() << std::endl;
        std::cout << "\theuristic = " << list->getHeuristic() << std::endl;
        if (list->getPuzzle() == nullptr)
            std::cout << "\tpuzzle is null" << std::endl;
        else
        {
            int **tmp = list->getPuzzle();
            for (int i = 0; i < list->getSize(); ++i)
            {
                std::cout << "\t";
                for (int j = 0; j < list->getSize(); ++j)
                    std::cout << tmp[i][j] << " ";
                std::cout << std::endl;
            }
        }
        list = list->next;
    }
    std::cout << ("} " + name + " list") << std::endl;
}

LCSolver::~LCSolver()
{
    LCPuzzle   *next;
    LCPuzzle   *current = this->_openList;
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

    delete this->_solutionPuzzle;
}

LCPuzzle    *LCSolver::solve(std::string heuristicType, std::string searchType)
{
    LCPuzzle      *best;

    while (this->_openList != nullptr)
    //for (int i = 0; i < 100; ++i)
    {
        best = this->getNextAndUpdateOpenList();

        LCPuzzle*  generated = best->generatePuzzleFromPosition();

        LCPuzzle   *result = nullptr;
        if (searchType.compare("UCS") == 0)
            result = this->UniformCostSearch(generated, heuristicType, best);
        else if (searchType.compare("GS") == 0)
            result = this->GreedySearch(generated, heuristicType, best);
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

LCPuzzle   *LCSolver::UniformCostSearch(LCPuzzle *generated, std::string heuristicType,
    LCPuzzle *best)
{
    LCPuzzle*  nextGenerated;
    while (generated != nullptr)
    {
        /*
        if (open->getPrevLCPuzzle() != nullptr
            && *(open->getPrevLCPuzzle()) == **it)
        {
            delete (*it);
            *it = nullptr;
            continue;
        } //optimization
        */
        nextGenerated = generated->next;
        LCPuzzle*  prevClosed = nullptr;
        LCPuzzle*  prevOpened = nullptr;
        LCPuzzle*  closed = nullptr;
        LCPuzzle*  opened = nullptr;

        if (generated->compare(*(this->_solutionPuzzle)))
        {
            generated->prevInSolution = best;
            return generated;
        }
        generated->setHeuritic(this->calcHeuristic(generated, heuristicType));
        if (checkIfInClosedList(generated, &closed, &prevClosed))
        {
            if (closed->getHeuristic() <= generated->getHeuristic())
            {
                delete generated;
                generated = nullptr;
            }
            else
            {
                this->deleteFromClosedList(prevClosed);
                this->addToOpenList(generated);
                generated->prevInSolution = best;
            }
        }
        else if (checkIfInOpenList(generated, &opened, &prevOpened))
        {
            if (opened->getHeuristic() <= generated->getHeuristic())
            {
                delete generated;
                generated = nullptr;
            }
            else
            {
                this->deleteFromOpenList(prevOpened);
                this->addToOpenList(generated);
                generated->prevInSolution = best;
            }
        }
        else
        {
            this->addToOpenList(generated);
            generated->prevInSolution = best;
        }
        generated = nextGenerated;
    }
    return nullptr;
}

LCPuzzle   *LCSolver::GreedySearch(LCPuzzle *generated, std::string heuristicType,
    LCPuzzle *best)
{
    throw std::runtime_error("greedy search function not done yet"); //TODO:
    (void)generated;
    (void)heuristicType;
    (void)best;
    return nullptr;
}

double     LCSolver::calcHeuristic(LCPuzzle *puzzle, std::string heauristicType)
{
    int **tmp = puzzle->getPuzzle();
    int **sol = this->_solutionPuzzle->getPuzzle();

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
                this->_solutionPuzzle->findNumberinLCPuzzle(tmp[y][x], &tmpx, &tmpy);
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
                this->_solutionPuzzle->findNumberinLCPuzzle(tmp[y][x], &tmpx, &tmpy);
                score += std::sqrt((tmpy - y) * (tmpy - y) + (tmpx - x) * (tmpx - x));
            }
        }
        return puzzle->getDepth() + score;
    }
    else
        throw std::runtime_error(std::string("Invalide heuristic name"));
}

LCPuzzle  *LCSolver::genSolution(void)
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
    LCPuzzle *solution = new LCPuzzle(solutionTab, this->_size);
    return solution;
}

/* À modifier à partir d'ici */
bool    LCSolver::checkIfInClosedList(LCPuzzle *generated, LCPuzzle **closed,
    LCPuzzle **prevClosed)
{
    LCPuzzle   *tmp = this->_closedList;
    LCPuzzle   *prev;

    while (tmp != nullptr)
    {
        if (tmp->compare(*generated))
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

bool    LCSolver::checkIfInOpenList(LCPuzzle *generated, LCPuzzle **opened,
    LCPuzzle **prevOpened)
{
    LCPuzzle   *tmp = this->_openList;
    LCPuzzle   *prev;

    while (tmp != nullptr)
    {
        if (tmp->compare(*generated))
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
void    LCSolver::addToOpenList(LCPuzzle *puzzle)
{
    LCPuzzle   *tmp = this->_openList;
    LCPuzzle   *prev = nullptr;

    if (puzzle == nullptr || puzzle->getHeuristic() == -1)
        throw std::runtime_error("Tried to add an invalide pointer to open list");
    ++this->totalOpenStates;
    if (tmp == nullptr || tmp->getHeuristic() > puzzle->getHeuristic())
    {
        puzzle->next = this->_openList;
        this->_openList = puzzle;
        return ;
    }
    while (tmp != nullptr && tmp->getHeuristic() <= puzzle->getHeuristic())
    {
        prev = tmp;
        tmp = tmp->next;
    }
    prev->next = puzzle;
    puzzle->next = tmp;
}

void    LCSolver::addToClosedList(LCPuzzle *puzzle)
{
    puzzle->next = this->_closedList;
    this->_closedList = puzzle;
}

LCPuzzle   *LCSolver::getNextAndUpdateOpenList(void)
{
    LCPuzzle *tmp = this->_openList;
    this->_openList = this->_openList->next;
    return tmp;
}

void        LCSolver::deleteFromClosedList(LCPuzzle *prevClosed)
{
    prevClosed->next = prevClosed->next->next;
}

void        LCSolver::deleteFromOpenList(LCPuzzle *prevOpen)
{
    prevOpen->next = prevOpen->next->next;
}
