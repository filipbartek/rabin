// Implemented from Faster Solutions of Rabin and Streett Games
// by Nir Piterman and Amir Pnueli
// Lines beginning with "//Piterman:" show the names of the corresponding
// methods and variables in the article

#include "PitermanRabinGameSolver.hpp"
#include <vector>
#include <ostream> // ostream, cout, endl
#include "Global.hpp" // BitsetType, bitset_empty, bitset_full, debug_cout
#include "RabinGame.hpp"

using std::vector;
using std::ostream;
using std::cout;
using std::endl;

typedef PitermanRabinGameSolver::TreeType::sibling_iterator TreeIteratorType;

#define STORE_DEFAULT true
#define EXTREME_FIX_OPTIMALISATION_DEFAULT true 

// Watch out! Descend and Advance work in an incompatible manner, although they
// take the same types of arguments. Descend returns a new iterator while
// Advance modifies the old iterator.

PitermanRabinGameSolver::TreeType::sibling_iterator
    PitermanRabinGameSolver::Descend(TreeType& tree,
    const TreeType::sibling_iterator& i, const BitsetType& default_value) {
  if (store_) {
    // i.node->first_child == 0 is an unclean but fast way to check whether
    // a node has no children.
    if (i.node->first_child == 0) tree.append_child(i, default_value);
    return i.begin();
  } else {
    *i = default_value;
    return i;
  }
}

void PitermanRabinGameSolver::Advance(TreeType& tree,
    TreeType::sibling_iterator& i, const BitsetType& default_value) {
  if (store_) {
    // i.note->next_sibling == 0 is an unclean way to check whether a node has
    // no next sibling.
    if (i.node->next_sibling == 0) tree.insert_after(i, default_value);
    ++i;
  } else *i = default_value;
}

PitermanRabinGameSolver::BitsetType PitermanRabinGameSolver::GetValue(
    const TreeType::sibling_iterator& i,
    const BitsetType& default_value) const {
  if (store_) return *i;
  else return default_value;
}

void PitermanRabinGameSolver::SetValue(const TreeType::sibling_iterator& i,
    const BitsetType& value) {
  if (store_) *i = value;
}

PitermanRabinGameSolver::PitermanRabinGameSolver()
  : RabinGameSolver(), store_(STORE_DEFAULT),
    extreme_fix_optimalisation_(EXTREME_FIX_OPTIMALISATION_DEFAULT),
    tree_x_(TreeType(BitsetType())),
    tree_y_(TreeType(BitsetType())) {
  debug_cout << "PitermanRabinGameSolver()" << endl;
}

PitermanRabinGameSolver::PitermanRabinGameSolver(const RabinGame& game)
  : RabinGameSolver(game), store_(STORE_DEFAULT),
    extreme_fix_optimalisation_(EXTREME_FIX_OPTIMALISATION_DEFAULT),
    tree_x_(TreeType(BitsetType())),
    tree_y_(TreeType(BitsetType())) {
  debug_cout << "PitermanRabinGameSolver(const RabinGame&)" << endl;
}

PitermanRabinGameSolver::PitermanRabinGameSolver(const bool& store,
    const bool& extreme_fix_optimalisation) : RabinGameSolver(),
    store_(store), extreme_fix_optimalisation_(extreme_fix_optimalisation),
    tree_x_(TreeType(BitsetType())), tree_y_(TreeType(BitsetType())) {} 

void PitermanRabinGameSolver::update_game() {
  debug_cout << "update_game()" << endl;
  // Reset trees.
  tree_x_ = TreeType(BitsetType());
  tree_y_ = TreeType(BitsetType()); 
}

void PitermanRabinGameSolver::store(const bool& store) {
  store_ = store;
}

void PitermanRabinGameSolver::extreme_fix_optimalisation(
    const bool& extreme_fix_optimalisation) {
  extreme_fix_optimalisation_ = extreme_fix_optimalisation;
}

//Piterman: main_Rabin
// Complexity: n*T(k)=n^(2k+1)*k!
bool PitermanRabinGameSolver::Solve(const bool& winning_set,
    const bool& strategy) {
  if ((winning_set && !winning_set_solved_) || (strategy && !strategy_solved_))
  {
    debug_cout << "winning || strategy" << endl;
    //Piterman: Z
    //winning_set_ could be used directly in place of x
    BitsetType x(bitset_empty<BitsetType>(n()));
    BitsetType x_prev(n()); //not initialised
    //Piterman: Set
    BitsetType condition(bitset_full<BitsetType>(k()));
    //Piterman: true
    BitsetType seqnr(bitset_full<BitsetType>(n()));
    BitsetType p1(n()); //not initialised
    // tree_y_i has to descend to second level first so that it can Advance
    // later.
    TreeIteratorType tree_y_i(Descend(tree_y_, tree_y_.begin()));
    do { // LeastFix x // Complexity: n
      debug_cout << "x=" << x << endl;
      x_prev = x;
      p1 = CPred(x, strategy);
      x = Rabin(condition, seqnr, p1, tree_x_.begin(), tree_y_i, strategy);
          // Complexity: [n*]T(k)
      Advance(tree_y_, tree_y_i);
    } while (x != x_prev);
    debug_cout << "fixpoint x=" << x << endl;
    winning_set_ = x;
    winning_set_solved_ = true;
    if (strategy) {
      for (NType v(0); v < n(); ++v) { // Complexity: n
        if (!winning_set_.test(v)) strategy_[v] = n();
      }
      strategy_solved_ = true;
    }
  }
  return false;
}

// c = condition.count():
// T(c)=k+c(n(n+n(n+T(c-1))))=k+cnn+cnnn+cnnT(c-1)
// T(k)=n^(2k+1)*k!
PitermanRabinGameSolver::BitsetType PitermanRabinGameSolver::Rabin(
    BitsetType& condition,
    const PitermanRabinGameSolver::BitsetType& seqnr,
    const PitermanRabinGameSolver::BitsetType& right,
    const TreeIteratorType& tree_x_i,
    const TreeIteratorType& tree_y_i,
    const bool& strategy) {
  debug_cout << "Rabin(" << condition << "," << seqnr << "," << right << ")"
             << endl;
  if (condition.count() == 0) return right;
  debug_cout << "start:count=" << condition.count() << endl;
  //Piterman: U
  BitsetType result(right);
  //Piterman: Y
  BitsetType y(n()); //initialised to true later
  BitsetType y_prev(n()); //not initialised
  BitsetType p2(n()); //not initialised
  //Piterman: X
  BitsetType x(n()); //initialised to false later
  BitsetType x_prev(n()); //not initialised
  BitsetType p3(n()); //not initialised
  // tree_x_condition_i is never used to access the data member so it needn't
  // have a value.
  TreeIteratorType tree_x_condition_i(Descend(tree_x_, tree_x_i));
  // tree_y_condition_i is used to store data so it needs to be initialised.
  // y is calculated as a GreatestFix so tree_y_condition_i needs to be
  // initialised to full set.
  TreeIteratorType tree_y_condition_i(Descend(tree_y_, tree_y_i,
      bitset_full<BitsetType>(n())));
  for (KType p(0); p < k(); ++p) { // Complexity: k
    if (condition.test(p)) { // Complexity: c
      debug_cout << " p=" << p << endl;
      condition.reset(p);
      const RabinStreettPair& pair(game_.condition_[p]);
      const BitsetType& g(pair.g_);
      const BitsetType& r(pair.r_);
      y = GetValue(tree_y_condition_i, bitset_full<BitsetType>(n()));
      debug_cout << " store y=" << y << endl;
      // tree_x_vertices_i is used to store data so it needs to be initialised.
      // x is calculated as a LeastFix so tree_x_vertices_i needs to be
      // initialised to empty set.
      TreeIteratorType tree_x_vertices_i(Descend(tree_x_, tree_x_condition_i,
          bitset_empty<BitsetType>(n())));
      do { // GreatestFix y // Complexity: [c*]n
        // Minor optimalisation - GreatestFix y found when y empty
        if (extreme_fix_optimalisation_ && y.none()) break;
        debug_cout << "  y=" << y << endl;
        y_prev = y;
        p2 = right | (seqnr & ~r & g & CPred(y)); // Complexity: [c*n*]n
        x = GetValue(tree_x_vertices_i, bitset_empty<BitsetType>(n()));
        debug_cout << "  store x=" << x << endl;
        // tree_y_vertices_i is never used to access the data member so it
        // needn't have a value.
        TreeIteratorType tree_y_vertices_i(Descend(tree_y_,
            tree_y_condition_i));       
        do { // LeastFix x // Complexity: [c*n*]n
          // Minor optimalisation - LeastFix x found when x full
          if (extreme_fix_optimalisation_ && (~x).none()) break;
          debug_cout << "   x=" << x << endl;
          x_prev = x;
          p3 = p2 | (seqnr & ~r & CPred(x, strategy)); // Complexity: [c*n*n*]n
          x = Rabin(condition, seqnr & ~r, p3, tree_x_vertices_i,
              tree_y_vertices_i, strategy); // Complexity: [c*n*n*]T(c-1)
          // tree_y_vertices_i is never used to access the data member so it
          // needn't have a value.
          Advance(tree_y_, tree_y_vertices_i);
        } while (x != x_prev);
        debug_cout << "  fixpoint x=" << x << endl;
        // If the tree_y_vertices_i was to ascend through tree_y_, this would
        // be the proper place for calling Ascend.
        debug_cout << "  store x="
            << GetValue(tree_x_vertices_i, bitset_empty<BitsetType>(n()))
            << endl;
        SetValue(tree_x_vertices_i, x);
        y = x;
        // tree_x_vertices_i is used to store data so it needs to be
        // initialised.
        // x is calculated as a LeastFix so tree_x_vertices_i needs to be
        // initialised to empty set.
        Advance(tree_x_, tree_x_vertices_i, bitset_empty<BitsetType>(n()));
      } while (y != y_prev);
      debug_cout << " fixpoint y=" << y << endl;
      // If tree_x_vertices_i was to ascend through tree_x_, this would be
      // the proper place for calling Ascend.
      debug_cout << " store y=" << GetValue(tree_y_condition_i, bitset_full<BitsetType>(n())) << endl;
      SetValue(tree_y_condition_i, y);
      result = result | y;
      // tree_x_condition_i is never used to access the data member so it
      // needn't have a value.
      Advance(tree_x_, tree_x_condition_i);
      // tree_y_condition_i is used to store data so it needs to be
      // initialised.
      // y is calculated as a GreatestFix so tree_y_condition_i needs to be
      // initialised to full set.
      Advance(tree_y_, tree_y_condition_i, bitset_full<BitsetType>(n()));
      condition.set(p);
    }
  }
  // If tree_a_condition_i was to ascend through tree_a_, this would be
  // the proper place for calling Ascend.
  debug_cout << "return:count=" << condition.count() << endl;
  return result;
}

// Control predecessor (for player true)
// T()=O(n)
// n = number of vertices in the game
PitermanRabinGameSolver::BitsetType PitermanRabinGameSolver::CPred(
    const BitsetType& set,
    const bool& strategy) {
  BitsetType result(n()); // empty
  vector<Vertex>& game(game_.game_);
  NType vertex_i_num(0);
  for (vector<Vertex>::const_iterator vertex_i = game.begin();
      vertex_i != game.end(); ++vertex_i) {
    const BitsetType successors_in_set(vertex_i->successors_ & set);
    // Any player: result true if some successor is in set
    if (successors_in_set.any()) {
      if (vertex_i->player_) {
        // Player true: result true iff some successor is in set
        result.set(vertex_i_num);
        if (strategy) strategy_[vertex_i_num] = successors_in_set.find_first();
      } else {
        // Player false: result true iff all successors are in set and some
        // successor is in set
        if (vertex_i->successors_.is_subset_of(set))
          result.set(vertex_i_num);
      }
    }
    ++vertex_i_num;
  }
  return result;
}
