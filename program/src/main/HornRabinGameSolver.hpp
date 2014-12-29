// Horn's Rabin game solver
// Implemented according to the following article:
// http://liafa.jussieu.fr/~horn/Papiers/Hor05-GDV/Streett%20Games%20on%20Finite%20Graphs%20-%20Horn%20-%20GDV%202005.pdf
// Streett Games on Finite Graphs
// by Florian Horn
// Time complexity: O(n^k k!)

#ifndef RABINGAMESOLVER_HORNRABINGAMESOLVER_HPP_
#define RABINGAMESOLVER_HORNRABINGAMESOLVER_HPP_

#include "Global.hpp" // BitsetType
#include "RabinGameSolver.hpp"

class RabinGame;

class HornRabinGameSolver : public RabinGameSolver {
 public:
  HornRabinGameSolver();
  HornRabinGameSolver(const RabinGame& game);
  
  virtual void update_game();

  // Always returns false.
  virtual bool Solve(const bool& winning_set, const bool& strategy);
 private:
  BitsetType SolveSubgame(
      const BitsetType&/*n*/ subgame,
      const BitsetType&/*k*/ condition,
      const bool& strategy = false);
  // Attractor
  BitsetType Attr(
      const bool& player,
      const BitsetType&/*n*/ subgame,
      const BitsetType&/*n*/ set,
      const bool& strategy = false);
};

#endif //RABINGAMESOLVER_HORNRABINGAMESOLVER_HPP_
