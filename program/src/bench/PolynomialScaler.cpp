#include "PolynomialScaler.hpp"
#include "pow.hpp"

PolynomialScaler::ReturnType PolynomialScaler::operator()(BaseType base) {
  return pow(base, parameter_);
}
