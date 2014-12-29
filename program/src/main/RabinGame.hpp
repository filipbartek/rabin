// RabinGame
// by Filip Bartek (xbartek3@fi.muni.cz)
// Represents a Rabin game.

#ifndef RABINGAMESOLVER_RABINGAME_HPP_
#define RABINGAMESOLVER_RABINGAME_HPP_

#include <vector>
#include <iostream> // ostream, istream
#include "Global.hpp" // NType, KType
#include "Vertex.hpp"
#include "RabinStreettPair.hpp"

class RabinGame {
 public:
  typedef rabingamesolver_global::NType NType;
  typedef rabingamesolver_global::KType KType;

  RabinGame();
  RabinGame(const NType& n, const KType& k);

  // Note: RabinGame doesn't provide resize(size_t).
  void resize(const NType& n, const KType& k);
  void reset();

  NType n() const;
  KType k() const;

  friend bool operator==(const RabinGame& l, const RabinGame& r);
  friend std::ostream& operator<<(std::ostream& os, const RabinGame& game);
  friend std::istream& operator>>(std::istream& is, RabinGame& game);
 private:
 public: // for faster access
  // A two-player game - a directed graph where each vertex belongs to
  // a player.
  std::vector<Vertex/*n*/>/*n*/ game_;
  
  // Condition pairs.
  // Note: Set might be a more appropriate data structure,
  // but for these implementations vector is more suitable.
  std::vector<RabinStreettPair/*n*/>/*k*/ condition_;
};

#endif //RABINGAMESOLVER_RABINGAME_HPP_
