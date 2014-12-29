#include "SquareTwoDTracer.hpp"

SquareTwoDTracer::SquareTwoDTracer() {}

bool SquareTwoDTracer::TestEnd() const {
  return end_;
}

void SquareTwoDTracer::Advance(clock_t clock_last) {
  if (TestEnd()) return;
  if (x_ == y_) {
    ++x_;
    if (InfIntType(x_) >= x_end_ - x_begin_) {
      ++y_;
      if (InfIntType(y_) >= y_end_ - y_begin_) end_ = true;
      x_ = 0;
    } else y_ = 0;
  } else if (x_ == y_ + 1) {
    ++y_;
    if (InfIntType(y_) >= y_end_ - y_begin_) {
      ++x_;
      if (InfIntType(x_) >= x_end_ - x_begin_) end_ = true;
      y_ = 0;
    } else x_ = 0;
  } else if (x_ < y_) {
    ++x_;
  } else {
    ++y_;
  }
}

SquareTwoDTracer::IntType SquareTwoDTracer::x() const {
  return x_begin_ + x_;
}

SquareTwoDTracer::IntType SquareTwoDTracer::y() const {
  return y_begin_ + y_;
}

void SquareTwoDTracer::initialize() {
  x_ = 0;
  y_ = 0;
  end_ = false;
}
