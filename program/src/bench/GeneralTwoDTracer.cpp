#include "GeneralTwoDTracer.hpp"

GeneralTwoDTracer::GeneralTwoDTracer() : tracer_(NULL) {}

GeneralTwoDTracer::~GeneralTwoDTracer() {
  TearDown();
}

void GeneralTwoDTracer::TearDown() {
  if (tracer_ != NULL) delete tracer_;
}

void GeneralTwoDTracer::tracer(TwoDTracer *tracer) {
  TearDown();
  tracer_ = tracer;
  tracer_->initialize();
}

void GeneralTwoDTracer::parameters(IntType x_begin, IntType x_end,
    IntType y_begin, IntType y_end,
    bool direction_x) {
  direction_x_ = direction_x;
  if (direction_x_) {
    tracer_->x_begin_ = x_begin;
    tracer_->x_end_ = InfIntType(x_end <= x_begin, x_end);
    tracer_->y_begin_ = y_begin;
    tracer_->y_end_ = InfIntType(y_end <= y_begin, y_end);
  } else {
    tracer_->x_begin_ = y_begin;
    tracer_->x_end_ = InfIntType(y_end <= y_begin, y_end);
    tracer_->y_begin_ = x_begin;
    tracer_->y_end_ = InfIntType(x_end <= x_begin, x_end);
  }
  tracer_->initialize();
}

bool GeneralTwoDTracer::TestEnd() const {
  return tracer_->TestEnd();
}

void GeneralTwoDTracer::Advance(clock_t clock_last) {
  tracer_->Advance(clock_last);
}

GeneralTwoDTracer::IntType GeneralTwoDTracer::x() const {
  if (direction_x_) return tracer_->x();
  else return tracer_->y();
}

GeneralTwoDTracer::IntType GeneralTwoDTracer::y() const {
  if (direction_x_) return tracer_->y();
  else return tracer_->x();
}
