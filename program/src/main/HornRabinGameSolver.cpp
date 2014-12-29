#include "HornRabinGameSolver.hpp"
#include <iostream>
#include "RabinGame.hpp"

using std::cout;
using std::endl;
using std::list;

HornRabinGameSolver::HornRabinGameSolver() : RabinGameSolver() {}

HornRabinGameSolver::HornRabinGameSolver(const RabinGame& game)
  : RabinGameSolver(game) {}

void HornRabinGameSolver::update_game() {}

// n = |V|
// k = |C|
// T_Solve(n,k)=n+T_Attr(n)+T_SolveSubgame(n,k)
bool HornRabinGameSolver::Solve(const bool& winning_set, const bool& strategy)
{
  if ((winning_set && !winning_set_solved_) || (strategy && !strategy_solved_))
  {
    // Always solves winning_set.
    // losing is a set of vertices where Adam loses because Eve can force
    // the token into a vertex that has no successors.
    BitsetType losing(n());
    for (NType v(0); v < n(); ++v) // n
      losing.set(v, game_.game_[v].successors_.none());
    losing = Attr(false, bitset_full<BitsetType>(n()), losing);
    // ~losing is a trap for player false (Eve).
    winning_set_ = SolveSubgame(~losing, bitset_full<BitsetType>(k()),
        strategy);
    winning_set_solved_ = true;
    if (strategy) strategy_solved_ = true;
  }
  return false;
}

// Returns winning region for player true (Adam) in subgame with condition
// m = |subgame|
// c = |condition|
// T_SolveSubgame(m,c)=
// =k+c(m(T_Attr(m)+T_SolveSubgame(m,c-1))+T_SolveSubgame(m-1,c))=
HornRabinGameSolver::BitsetType HornRabinGameSolver::SolveSubgame(
    const BitsetType&/*n*/ subgame,
    const BitsetType&/*k*/ condition,
    const bool& strategy) {
  debug_cout << "SolveSubgame(" << subgame << ", " << condition << ")" << endl;
  BitsetType result(bitset_empty<BitsetType>(n())); //initialised to false
  BitsetType condition_new(condition);
  for (KType p(0); p < k(); ++p) { // O(k)
    if (condition.test(p)) { // O(c)
      condition_new.reset(p);
      const RabinStreettPair& pair(game_.condition_[p]);
      BitsetType subgame_g(subgame & ~Attr(false, subgame, pair.r_));
          // O([c]T_Attr(m))
      BitsetType subgame_g_prev(n()); //not initialised
      do { // GreatestFix // O([c]m)
        debug_cout << "subgame_g=" << subgame_g << endl;
        subgame_g_prev = subgame_g;
        BitsetType subgame_h(subgame_g & ~Attr(true, subgame_g, pair.g_));
            // O([cm]T_Attr(m))
        debug_cout << "subgame_h=" << subgame_h << endl;
        debug_cout << "+" << endl;
        subgame_g = subgame_g & ~Attr(false, subgame_g,
            subgame_h & ~SolveSubgame(subgame_h, condition_new, false));
            // O([cm](T_Attr(m)+T_SolveSubgame(m,c-1))
        debug_cout << "-" << endl;
      } while (subgame_g != subgame_g_prev);
      debug_cout << "fixpoint subgame_g=" << subgame_g << endl;
      if (subgame_g.any()) { // O(c)
        debug_cout << "subgame_g.any()" << endl;
        subgame_g = Attr(true, subgame, subgame_g, strategy); //winning for Adam
            // O([c]T_Attr(m))
        BitsetType subgame_new(subgame & ~subgame_g);
        debug_cout << "+" << endl;
        result = subgame_g | SolveSubgame(subgame_new, condition, strategy);
            // O([c]T_SolveSubgame(m-1,c))
        debug_cout << "-" << endl;
        break;//KType p
      }
      condition_new.set(p);
    }
  }
  debug_cout << "SolveSubgame(" << subgame << ", " << condition << ")="
             << result << endl;
  return result;
}

//Attractor
//T(n) in O(n^2)
//only set&subgame is considered from set - set should be subset of subgame
// m = |subgame|
// T_Attr(m)=O(mn)
HornRabinGameSolver::BitsetType HornRabinGameSolver::Attr(
    const bool& player,
    const BitsetType&/*n*/ subgame,
    const BitsetType&/*n*/ set,
    const bool& strategy) {
  BitsetType result(subgame & set);
  BitsetType result_prev(n()); // not initialised
  do { // LeastFix result beginning with subgame&set (instead of an empty set)
      // O(m)
    result_prev = result;
    for (NType v(0); v < n(); ++v) { // O([m]n)
      if (subgame.test(v) && !result.test(v)) {
        const Vertex& vertex(game_.game_[v]);
        // Any player: result true if some successor in result
        if ((vertex.successors_ & result).any()) {
          if (vertex.player_ == player) {
            // Active player: result true iff some successor in result
            result.set(v);
            //if (strategy) strategy_.elements_[v] = (vertex.successors_ & result).find_first();
          } else {
            // Passive player: result true iff all successors (in subgame)
            // in result and some successor in result
            if ((vertex.successors_ & subgame).is_subset_of(result))
              result.set(v);
          }
        }
      }
    }
  } while (result != result_prev);
  // Update strategy
  if (strategy && player) { // Only generate strategy for Adam
    for (NType v(0); v < n(); ++v) { // O(n)
      if (result.test(v) && game_.game_[v].player_) {
        BitsetType cache(game_.game_[v].successors_ & result);
        if (cache.any()) {
          strategy_[v] = cache.find_first();
        }
      }
    }
  }
  return result;
}
