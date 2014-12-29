#ifndef RABINGAMESOLVER_TWODTRACER_H_
#define RABINGAMESOLVER_TWODTRACER_H_

#include <ctime> // clock_t

template<typename IntType>
class InfInt {
 public:
  InfInt() {};
  InfInt(IntType number) : infinite(false), number(number) {};
  InfInt(bool infinite, IntType number) : infinite(infinite), number(number)
      {};
  bool infinite;
  IntType number;
};

template<typename IntType>
bool operator==(const InfInt<IntType>& l, const InfInt<IntType>& r) {
  if (l.infinite && r.infinite) return true;
  if (!l.infinite && !r.infinite && l.number == r.number) return true;
  return false;
}

template<typename IntType>
bool operator!=(const InfInt<IntType>& l, const InfInt<IntType>& r) {
  return !(l==r);
}

template<typename IntType>
bool operator>=(const InfInt<IntType>& l, const InfInt<IntType>& r) {
  if (l.infinite) return true;
  if (!r.infinite && l.number >= r.number) return true;
  return false;
}

template<typename IntType>
bool operator>(const InfInt<IntType>& l, const InfInt<IntType>& r) {
  return l>=r && l!=r;
}

template<typename IntType>
bool operator<=(const InfInt<IntType>& l, const InfInt<IntType>& r) {
  return !(l>r);
}

template<typename IntType>
bool operator<(const InfInt<IntType>& l, const InfInt<IntType>& r) {
  return !(l>=r);
}

template<typename IntType>
InfInt<IntType> operator-(const InfInt<IntType>& l, const IntType& r) {
  if (l.infinite) return l;
  return InfInt<IntType>(l.number - r);
}

class TwoDTracer {
 public:
  typedef unsigned int IntType;
  typedef InfInt<IntType> InfIntType;

  virtual bool TestEnd() const = 0;
  virtual void Advance(clock_t clock_last) = 0;
  virtual IntType x() const = 0;
  virtual IntType y() const = 0;
  virtual void initialize() = 0;
  
  IntType x_begin_;
  InfIntType x_end_;
  IntType y_begin_;
  InfIntType y_end_;
};

#endif // RABINGAMESOLVER_TWODTRACER_H_
