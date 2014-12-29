// A pair in Rabin or Streett condition.
// Contains sets of vertices (bitsets) G (g_) and R (r_).

#ifndef RABINGAMESOLVER_RABINSTREETTPAIR_HPP_
#define RABINGAMESOLVER_RABINSTREETTPAIR_HPP_

#include <iostream> // ostream, istream
#include "Global.hpp" // BitsetType

class RabinStreettPair {
 public:
  typedef rabingamesolver_global::BitsetType BitsetType;
  typedef BitsetType::size_type size_type;

  size_type size() const;
  void resize(const size_type& size);
  void reset(); // Empties the sets (without resizing them).

  friend bool operator==(const RabinStreettPair& l,
                          const RabinStreettPair& r);
  friend std::ostream& operator<<(std::ostream& os,
      const RabinStreettPair& obj);
  friend std::istream& operator>>(std::istream& is, RabinStreettPair& obj);
 private:
 public: //for faster access
  // g_ and r_ should always have the same size (n).
  BitsetType/*n*/ g_;
  BitsetType/*n*/ r_;
};

#endif //RABINGAMESOLVER_RABINSTREETTPAIR_HPP_
