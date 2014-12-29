#include "ExponentialScaler.hpp"
#include "pow.hpp"

ExponentialScaler::ReturnType ExponentialScaler::operator()(BaseType base) {
  return pow(parameter_, base);
}
