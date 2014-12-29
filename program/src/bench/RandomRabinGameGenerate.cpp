#include "RandomRabinGameGenerate.hpp"
#include <cstdlib> // srand
#include <vector>
#include "RabinGame.hpp"
#include "Toss.hpp"

using std::vector;
using rabingamesolver_global::NType;
using rabingamesolver_global::KType;

RabinGame RandomRabinGameGenerate(NType n, KType k, unsigned int seed) {
  RabinGame result(n, k);
  srand(seed);
  for (vector<Vertex>::iterator v(result.game_.begin());
      v != result.game_.end(); ++v) {
    for (NType w(0); w < n; ++w) {
      v->successors_.set(w, Toss(1, 2));
    }
    v->player_ = Toss(1, 2);
  }
  for (vector<RabinStreettPair>::iterator p(result.condition_.begin());
      p != result.condition_.end(); ++p) {
    for (NType w(0); w < n; ++w) {
      p->g_.set(w, Toss(1, 2));
    }
    for (NType w(0); w < n; ++w) {
      p->r_.set(w, Toss(1, 2));
    }
  }
  return result;
}
