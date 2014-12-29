#include "Toss.hpp"
#include <cstdlib> // rand()

bool Toss(const unsigned int& numerator, const unsigned int& denominator) {
  return (rand() % denominator) < numerator;
}
