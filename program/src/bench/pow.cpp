// http://stackoverflow.com/a/1505791

#include "pow.hpp"

unsigned int pow(unsigned int base, unsigned int exp)
{
  if (exp == 0) return 1;
  if (exp == 1) return base;

  unsigned int tmp = pow(base, exp / 2);
  if (exp % 2 == 0) return tmp * tmp;
  else return base * tmp * tmp;
}
