#ifndef RABINGAMESOLVER_SQUARETWODTRACER_HPP_
#define RABINGAMESOLVER_SQUARETWODTRACER_HPP_

#include "TwoDTracer.hpp"

class SquareTwoDTracer : public TwoDTracer {
 public:
  SquareTwoDTracer();
  virtual bool TestEnd() const;
  virtual void Advance(clock_t /*clock_last*/);
  virtual IntType x() const;
  virtual IntType y() const;
  virtual void initialize();
 private:
  IntType x_;
  IntType y_;
  bool end_;
};

#endif // RABINGAMESOLVER_SQUARETWODTRACER_HPP_
