#include <iostream>
#include <vector>
#include <stdexcept>
#include "Solver.class.hpp"

Solver::Solver()
{
    this->_original = NULL;
}

Solver::Solver(int size, Puzzle *original):
    _size(size), _original(original)
{
    
}

Solver::Solver(Solver const & instance)
{
    throw std::runtime_error(std::string("Tried to copy a Solver instance"));
}