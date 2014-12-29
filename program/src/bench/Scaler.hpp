#ifndef RABINGAMESOLVER_SCALER_HPP_
#define RABINGAMESOLVER_SCALER_HPP_

class Scaler {
 public:
  typedef unsigned int ParameterType;
  typedef unsigned int ReturnType;
  typedef unsigned int BaseType;
  
  virtual ReturnType operator()(BaseType base) = 0;
  
  ParameterType parameter_;
};

#endif // RABINGAMESOLVER_SCALER_HPP_
