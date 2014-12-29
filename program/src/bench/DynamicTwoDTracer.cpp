#include "DynamicTwoDTracer.hpp"

DynamicTwoDTracer::DynamicTwoDTracer() {
  initialize();
}

bool DynamicTwoDTracer::TestEnd() const {
  return end_;
}

void DynamicTwoDTracer::Advance(clock_t clock_last) {
  if (TestEnd()) return;
  if (direction_x_) {
    y_max_clock_ += clock_last;
  } else {
    x_max_clock_ += clock_last;
  }
  if (x_ == x_max_ && y_ == y_max_) {
    if (x_max_clock_ <= y_max_clock_) {
      ++x_;
      if (InfIntType(x_) >= x_end_) {
        ++y_;
        if (InfIntType(y_) >= y_end_) {
          end_ = true;
          return;
        }
        ++y_max_;
        y_max_clock_ = 0;
        x_max_clock_ += clock_last;
        x_ = x_begin_;
        direction_x_ = true;
      } else {
        x_max_clock_ = 0;
        ++x_max_;
        y_ = y_begin_;
        direction_x_ = false;
      }
    } else {
      ++y_;
      if (InfIntType(y_) >= y_end_) {
        ++x_;
        if (InfIntType(x_) >= x_end_) {
          end_ = true;
          return;
        }
        ++x_max_;
        x_max_clock_ = 0;
        y_max_clock_ += clock_last;
        y_ = y_begin_;
        direction_x_ = false;
      } else {
        y_max_clock_ = 0;
        ++y_max_;
        x_ = x_begin_;
        direction_x_ = true;
      }
    }
  } else if (x_ < x_max_) {
    ++x_;
    direction_x_ = true;
  } else if (y_ < y_max_) {
    ++y_;
    direction_x_ = false;
  }
}

DynamicTwoDTracer::IntType DynamicTwoDTracer::x() const {
  return x_;
}

DynamicTwoDTracer::IntType DynamicTwoDTracer::y() const {
  return y_;
}

void DynamicTwoDTracer::initialize() {
  x_ = x_begin_;
  y_ = y_begin_;
  x_max_ = x_;
  y_max_ = y_;
  x_max_clock_ = 0;
  y_max_clock_ = 0;
  end_ = false;
  direction_x_ = false;
}
