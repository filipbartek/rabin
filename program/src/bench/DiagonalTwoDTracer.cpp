#include "DiagonalTwoDTracer.hpp"

DiagonalTwoDTracer::DiagonalTwoDTracer() {}

bool DiagonalTwoDTracer::TestEnd() const {
  return InfIntType(y_) == y_end_;
}

void DiagonalTwoDTracer::Advance(clock_t clock_last) {
  if (TestEnd()) return;
  if (x_ == x_begin_ || InfIntType(y_) == (y_end_ - 1u)) {
    x_ = x_ + (y_ - y_begin_) + 1;
    if (InfIntType(x_) >= x_end_) {
      // Now we can use x_end_.number because the previous condition makes sure
      // x_end_ is finite.
      y_ = y_begin_ + (x_ - x_end_.number) + 1;
      x_ = x_end_.number - 1;
    } else {
      y_ = y_begin_;      
    }
  } else {
    --x_;
    ++y_;
  }
}

DiagonalTwoDTracer::IntType DiagonalTwoDTracer::x() const {
  return x_;
}

DiagonalTwoDTracer::IntType DiagonalTwoDTracer::y() const {
  return y_;
}

void DiagonalTwoDTracer::initialize() {
  x_ = x_begin_;
  y_ = y_begin_;
}
