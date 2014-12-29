#ifndef RABINGAMESOLVER_LINEARSCALER_HPP_
#define RABINGAMESOLVER_LINEARSCALER_HPP_

#include "Scaler.hpp"

class LinearScaler : public Scaler {
 public:
  virtual ReturnType operator()(BaseType base);
};

#endif // RABINGAMESOLVER_LINEARSCALER_HPP_
