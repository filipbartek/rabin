#ifndef RABINGAMESOLVER_EXPONENTIALSCALER_HPP_
#define RABINGAMESOLVER_EXPONENTIALSCALER_HPP_

#include "Scaler.hpp"

class ExponentialScaler : public Scaler {
 public:
  virtual ReturnType operator()(BaseType base);
};

#endif // RABINGAMESOLVER_EXPONENTIALSCALER_HPP_
