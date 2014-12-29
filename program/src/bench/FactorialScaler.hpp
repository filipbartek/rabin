#ifndef RABINGAMESOLVER_FACTORIALSCALER_HPP_
#define RABINGAMESOLVER_FACTORIALSCALER_HPP_

#include "Scaler.hpp"

class FactorialScaler : public Scaler {
 public:
  virtual ReturnType operator()(BaseType base);
};

#endif // RABINGAMESOLVER_FACTORIALSCALER_HPP_
