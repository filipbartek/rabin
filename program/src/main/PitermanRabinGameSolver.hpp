// Piterman's Rabin game solver
// Implemented according to the following article:
// http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.134.1574
// Faster Solutions of Rabin and Streett Games
// by Nir Piterman, Amir Pnueli
// Time complexity: O(n^(k+1) k!)
// Space complexity: O(n^(k+1) k!)

#ifndef RABINGAMESOLVER_PITERMANRABINGAMESOLVER_HPP_
#define RABINGAMESOLVER_PITERMANRABINGAMESOLVER_HPP_

#include "Global.hpp" // BitsetType
#include "RabinGameSolver.hpp"
#include "tree.hh"

class RabinGame;

class PitermanRabinGameSolver : public RabinGameSolver {
 public:
  PitermanRabinGameSolver();
  PitermanRabinGameSolver(const RabinGame& game);
  PitermanRabinGameSolver(const bool& store,
                          const bool& extreme_fix_optimalisation);
  
  virtual void update_game();
  
  void store(const bool& store);
  void extreme_fix_optimalisation(const bool& extreme_fix_optimalisation);

  // Always returns false.
  // Piterman: main_Rabin
  virtual bool Solve(const bool& winning_set, const bool& strategy);

  typedef tree<BitsetType/*n*/> TreeType;
 private: 
  // Piterman: Rabin(Set,seqnr,right)
  // Set is the set of condition pairs that need resolving by this call to
  // Rabin.
  // seqnr contains all the vertices except those which are contained in r_ of
  // some condition pair that isn't in Set. 
  // Returned set always contains right.
  BitsetType Rabin(BitsetType&/*k*/ condition, const BitsetType&/*n*/ seqnr,
      const BitsetType&/*n*/ right, const TreeType::sibling_iterator& tree_x_i,
      const TreeType::sibling_iterator& tree_y_i, const bool& strategy = false);
  // Piterman: cpred
  BitsetType CPred(const BitsetType&/*n*/ set, const bool& strategy = false);
  TreeType::sibling_iterator Descend(TreeType& tree,
      const TreeType::sibling_iterator& i,
      const BitsetType& default_value = BitsetType());
  void Advance(TreeType& tree,
    TreeType::sibling_iterator& i,
    const BitsetType& default_value = BitsetType());
  BitsetType GetValue(const TreeType::sibling_iterator& i,
    const BitsetType& default_value = BitsetType()) const;
  void SetValue(const TreeType::sibling_iterator& i,
    const BitsetType& value);

  bool store_;
  bool extreme_fix_optimalisation_;
  // Trees are used to store additional global computation data.
  TreeType tree_x_; // empty sets by default for LeastFix
  TreeType tree_y_; // full sets by default for GreatestFix
};

#endif //RABINGAMESOLVER_PITERMANRABINGAMESOLVER_HPP_
