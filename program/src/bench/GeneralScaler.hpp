#ifndef RABINGAMESOLVER_GENERALSCALER_HPP_
#define RABINGAMESOLVER_GENERALSCALER_HPP_

#include "Scaler.hpp"

class GeneralScaler {
 public:
  typedef Scaler::BaseType BaseType;
  typedef Scaler::ReturnType ReturnType;
  typedef Scaler::ParameterType ParameterType;
  
  GeneralScaler();
  ~GeneralScaler();
  
  ReturnType operator()(BaseType base);
  
  void scaler(Scaler *scaler);
  void parameter(ParameterType parameter);
 private:
  void TearDown();
  void scaler_update();
  
  Scaler *scaler_;
  ParameterType parameter_;
};

#endif // RABINGAMESOLVER_GENERALSCALER_HPP_
