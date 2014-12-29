#include "RabinStreettPair.hpp"
#include <iostream> // ostream, istream

using std::ostream;
using std::istream;

RabinStreettPair::size_type RabinStreettPair::size() const {
  return g_.size();
}

void RabinStreettPair::resize(const RabinStreettPair::size_type& size) {
  g_.resize(size);
  r_.resize(size);
}

void RabinStreettPair::reset() {
  g_.reset();
  r_.reset();
}

bool operator==(const RabinStreettPair& l, const RabinStreettPair& r) {
  if (&l == &r) return true;
  if (l.g_ != r.g_) return false;
  if (l.r_ != r.r_) return false;
  return true;
}

ostream& operator<<(ostream& os, const RabinStreettPair& obj) {
  os << obj.g_ << " " << obj.r_;
  return os;
}

// Also assures g_.size = r_.size.
istream& operator>>(istream& is, RabinStreettPair& obj) {
  RabinStreettPair::size_type size(obj.size());
  is >> obj.g_ >> obj.r_;
  obj.resize(size);
  return is;
}
