#include "FactorialScaler.hpp"

unsigned int fact(unsigned int base) {
  if (base == 0) return 1;
  return base * fact(base - 1);
}

FactorialScaler::ReturnType FactorialScaler::operator()(BaseType base) {
  return fact(base);
}
