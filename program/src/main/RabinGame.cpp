#include "RabinGame.hpp"
#include <iostream> // ostream, istream
#include "Global.hpp" // Resize, reset

using std::ostream;
using std::istream;
using std::endl;
using rabingamesolver_global::Resize;

RabinGame::RabinGame() {}

RabinGame::RabinGame(const RabinGame::NType& n, const RabinGame::KType& k) {
  resize(n, k);
}

void RabinGame::resize(const RabinGame::NType& n, const RabinGame::KType& k) {
  game_.resize(n);
  for_each(game_.begin(), game_.end(), Resize<Vertex>(n));
  condition_.resize(k);
  for_each(condition_.begin(), condition_.end(), Resize<RabinStreettPair>(n));
}

void RabinGame::reset() {
  for_each(game_.begin(), game_.end(), rabingamesolver_global::reset<Vertex>);
  for_each(condition_.begin(), condition_.end(),
      rabingamesolver_global::reset<RabinStreettPair>);
}

RabinGame::NType RabinGame::n() const {
  return game_.size();
}

RabinGame::KType RabinGame::k() const {
  return condition_.size();
}

bool operator==(const RabinGame& l, const RabinGame& r) {
  if (&l == &r) return true;
  if (l.game_ != r.game_) return false;
  if (l.condition_ != r.condition_) return false;
  return true;
}

ostream& operator<<(ostream& os, const RabinGame& obj) {
  os << obj.n() << endl
     << obj.k() << endl
     << obj.game_ << endl
     << obj.condition_;
  return os;
}

istream& operator>>(istream& is, RabinGame& obj) {
  RabinGame::NType n;
  RabinGame::KType k;
  is >> n >> k;
  //following command may do unnecessary resizes in lower levels
  obj.resize(n, k);
  is >> obj.game_ >> obj.condition_;
  return is;
}
