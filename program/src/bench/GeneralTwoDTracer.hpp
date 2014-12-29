#ifndef RABINGAMESOLVER_GENERALTWODTRACER_HPP_
#define RABINGAMESOLVER_GENERALTWODTRACER_HPP_

#include "TwoDTracer.hpp"

class GeneralTwoDTracer {
 public:
  typedef TwoDTracer::IntType IntType;
  typedef TwoDTracer::InfIntType InfIntType;
  GeneralTwoDTracer();
  ~GeneralTwoDTracer();

  void TearDown();

  void tracer(TwoDTracer *tracer);
  void parameters(IntType x_begin, IntType x_end,
                  IntType y_begin, IntType y_end, bool direction_x);
  
  bool TestEnd() const;
  void Advance(clock_t clock_last);
  IntType x() const;
  IntType y() const;
 private:
  TwoDTracer *tracer_;
  bool direction_x_;
};

#endif // RABINGAMESOLVER_GENERALTWODTRACER_HPP_
