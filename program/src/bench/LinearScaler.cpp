#include "LinearScaler.hpp"

LinearScaler::ReturnType LinearScaler::operator()(BaseType base) {
  return base * parameter_;
}
