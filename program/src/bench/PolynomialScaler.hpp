#ifndef RABINGAMESOLVER_POLYNOMIALSCALER_HPP_
#define RABINGAMESOLVER_POLYNOMIALSCALER_HPP_

#include "Scaler.hpp"

class PolynomialScaler : public Scaler {
 public:
  virtual ReturnType operator()(BaseType base);
};

#endif // RABINGAMESOLVER_POLYNOMIALSCALER_HPP_
