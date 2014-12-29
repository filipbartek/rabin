#ifndef RABINGAMESOLVER_DIAGONALTWODTRACER_HPP_
#define RABINGAMESOLVER_DIAGONALTWODTRACER_HPP_

#include "TwoDTracer.hpp"

class DiagonalTwoDTracer : public TwoDTracer {
 public:
  DiagonalTwoDTracer();
  virtual bool TestEnd() const;
  // Diagonal tracer doesn't use clock_last.
  virtual void Advance(clock_t clock_last);
  virtual IntType x() const;
  virtual IntType y() const;
  virtual void initialize();
 private:
  IntType x_;
  IntType y_;
};

#endif // RABINGAMESOLVER_DIAGONALTWODTRACER_HPP_
