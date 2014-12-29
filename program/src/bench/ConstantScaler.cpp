#include "ConstantScaler.hpp"

ConstantScaler::ReturnType ConstantScaler::operator()(BaseType base) {
  return parameter_;
}
