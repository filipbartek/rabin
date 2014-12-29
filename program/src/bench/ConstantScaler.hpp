#ifndef RABINGAMESOLVER_CONSTANTSCALER_HPP_
#define RABINGAMESOLVER_CONSTANTSCALER_HPP_

#include "Scaler.hpp"

class ConstantScaler : public Scaler {
 public:
  virtual ReturnType operator()(BaseType base = BaseType());
};

#endif // RABINGAMESOLVER_CONSTANTSCALER_HPP_
