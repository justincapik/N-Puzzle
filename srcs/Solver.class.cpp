#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include "Solver.class.hpp"

Solver::Solver(Puzzle *original, int size):
    _original(original), _size(size)
{
    this->_solution = this->genSolution();
    if (!this->isSolvable(size))
        throw std::runtime_error("This puzzle is not solvable.");
}

Solver::~Solver()
{
    delete this->_solution;
    delete this->_original;
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
    return _solution;
    Puzzle      *open;

    this->_openList.push_back(this->_original);

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
            int i = checkIfInClosedList(*it);

            (*it)->setHeuritic(this->calcHeuristic(*it, heuristicType));
            if (i == -1)
            {
                addToOpenList(*it, searchType);
            }
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
    /*
    else if (heauristicType.compare("A* Manhattan distance") == 0)
    {
        int score = 0;
        for (int y = 0; y < this->_size; ++y)
            for (int x = 0; x < this->_size; ++x)
                score +=
    }
    */
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

void    Solver::addToOpenList(Puzzle *puzzle, std::string searchType)
{
    std::vector<Puzzle*>::iterator it = this->_openList.begin();
    while (it < this->_openList.end()
        && (*it)->getHeuristic() <= puzzle->getHeuristic())
        ++it;
    this->_openList.insert(it, puzzle);
    (void)searchType; //TODO: for greedy and uniform search
}

bool Solver::isSolvable(int size) {
    int p0s = 0;
    int convert[size * size];
    int **solution = this->_solution->getPuzzle();
    int **original = this->_original->getPuzzle();

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (solution[y][x] != 0)
                convert[solution[y][x] - 1] = original[y][x];
            else
                convert[size * size - 1] = original[y][x];
            if (original[y][x] == 0)
                p0s = size - y;
            std::cout << std::setw(2) << solution[y][x] << " ";
        }
        std::cout << std::endl;
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
