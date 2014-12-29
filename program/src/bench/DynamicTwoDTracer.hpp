#ifndef RABINGAMESOLVER_DYNAMICTWODTRACER_HPP_
#define RABINGAMESOLVER_DYNAMICTWODTRACER_HPP_

#include "TwoDTracer.hpp"

class DynamicTwoDTracer : public TwoDTracer {
 public:
  DynamicTwoDTracer();
  virtual bool TestEnd() const;
  virtual void Advance(clock_t clock_last);
  virtual IntType x() const;
  virtual IntType y() const;
  virtual void initialize();
 private:
  IntType x_;
  IntType y_;
  IntType x_max_;
  IntType y_max_;
  clock_t x_max_clock_;
  clock_t y_max_clock_;
  bool end_;
  bool direction_x_;
};

#endif // RABINGAMESOLVER_DYNAMICTWODTRACER_HPP_
