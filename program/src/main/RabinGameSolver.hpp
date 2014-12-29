#ifndef RABINGAMESOLVER_RABINGAMESOLVER_HPP_
#define RABINGAMESOLVER_RABINGAMESOLVER_HPP_

#include <vector>
#include "Global.hpp" // BitsetType, NType, KType
#include "RabinGame.hpp"

class RabinGameSolver {
 public:
  typedef rabingamesolver_global::BitsetType BitsetType;
  typedef rabingamesolver_global::NType NType;
  typedef rabingamesolver_global::KType KType;

  RabinGameSolver();
  RabinGameSolver(const RabinGame& game);
  virtual ~RabinGameSolver();

  // Calls update_game()
  void game(const RabinGame& game);
  virtual void update_game() = 0;

  // Returns true iff the computation failed.
  virtual bool Solve(const bool& winning_set, const bool& strategy) = 0;

  void Unsolve();

  //Contains exactly those vertices from which player true can win the game.
  const BitsetType/*n*/& winning_set();
  
  //Each vertex from winning_set() that belongs to player true is assigned
  //another vertex such that if player true follows this strategy, she wins.
  //Other vertices have undefined values assigned.
  const std::vector<NType>/*n*/& strategy();
 protected:
  NType n() const;
  KType k() const;

  RabinGame game_;
  BitsetType/*n*/ winning_set_;
  std::vector<NType>/*n*/ strategy_;
  bool winning_set_solved_;
  bool strategy_solved_;
};

// SetGame(gameobj)(obj) calls obj.game(gameobj).
/*class SetGame {
 public:
  SetGame(const RabinGame& game);
  void operator()(RabinGameSolver*& obj);
 private:
  RabinGame game_;
};*/

#endif //RABINGAMESOLVER_RABINGAMESOLVER_HPP_
