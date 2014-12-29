// Vertex is a state in a game's state transition system.
// Total number of states in the game is n.
// resize must be called before the Vertex is used because successors_ is
// initialised to empty bitset (size 0).

#ifndef RABINGAMESOLVER_VERTEX_HPP_
#define RABINGAMESOLVER_VERTEX_HPP_ 

#include <vector>
#include <iostream> // ostream, istream
#include "Global.hpp" // BitsetType

class Vertex {
 public:
  typedef rabingamesolver_global::BitsetType BitsetType;
  typedef BitsetType::size_type size_type;

  Vertex();
  size_type size() const;
  void resize(const size_type& size);
  void reset();

  friend bool operator==(const Vertex& l, const Vertex& r);
  // Prints "successors player"
  // For example "01 0"
  friend std::ostream& operator<<(std::ostream& os, const Vertex& obj);
  // Preserves size
  friend std::istream& operator>>(std::istream& is, Vertex& obj);
 private:
 public: //for faster access
  BitsetType/*n*/ successors_;
  bool player_;
};

#endif //RABINGAMESOLVER_VERTEX_HPP_
