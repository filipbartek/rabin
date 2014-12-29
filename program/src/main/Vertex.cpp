#include "Vertex.hpp"
#include <iostream> // ostream, istream

using std::ostream;
using std::istream;

Vertex::Vertex() : player_(false) {}

Vertex::size_type Vertex::size() const {
  return successors_.size();
}

void Vertex::resize(const Vertex::size_type& size) {
  successors_.resize(size);
}

void Vertex::reset() {
  successors_.reset();
  player_ = false;
}

bool operator==(const Vertex& l, const Vertex& r) {
  if (&l == &r) return true;
  if (l.successors_ != r.successors_) return false;
  if (l.player_ != r.player_) return false;
  return true;
}

ostream& operator<<(ostream& os, const Vertex& obj) {
  os << obj.successors_ << " " << obj.player_;
  return os;
}

istream& operator>>(istream& is, Vertex& obj) {
  Vertex::size_type size(obj.size());
  is >> obj.successors_ >> obj.player_;
  obj.resize(size);
  return is;
}
