// Defines various functors etc. specific for RabinGameSolver project.

#ifndef RABINGAMESOLVER_GLOBAL_HPP_
#define RABINGAMESOLVER_GLOBAL_HPP_

#include <vector>       
#include <list>
#include <algorithm> // transform
#include <iostream> // istream, ostream
#include "boost/dynamic_bitset.hpp" // dynamic_bitset
#include "nullstream.hpp" // null_stream

/*
#ifdef DEBUG
std::ostream& debug_cout(std::cout);
#else
null_stream debug_cout;
#endif // DEBUG
*/

#ifndef debug_cout
#ifdef DEBUG
#define debug_cout std::cout
#else
#define debug_cout null_stream()
#endif // DEBUG
#endif // debug_cout

namespace rabingamesolver_global {

typedef boost::dynamic_bitset<> BitsetType;
typedef BitsetType::size_type NType;
typedef BitsetType::size_type KType;

// Used in operator>> for vector and list
template <typename T>
class OperatorIstream {
 public:
  OperatorIstream(std::istream& is) : is_(is) {}
  void operator()(T& obj) {
    is_ >> obj;
  }
 private:
  std::istream& is_;
};

// Used in operator<< for vector and list
template <typename T>
class OperatorOstream {
 public:
  OperatorOstream(std::ostream& os) : os_(os) {}
  void operator()(const T& obj) {
    os_ << obj;
  }
 private:
  std::ostream& os_;
};

// Resize(size)(obj) calls obj.resize(size).
template <typename T>
class Resize {
 private:
  typedef typename T::size_type size_type;
 public:
  Resize(size_type size) : size_(size) {}
  void operator()(T& obj) {
    obj.resize(size_);
  }
 private:
  size_type size_;
};

// reset(obj) calls obj.reset()
template <typename T>
void reset(T& obj) { obj.reset(); }

} // namespace rabingamesolver_global

// Returns a bitset (BitsetType) of length size with all the bits set to false.
template<typename BitsetType>
BitsetType bitset_empty(typename BitsetType::size_type size) {
  //expects BitsetType constructs empty sets by default
  return BitsetType(size);
}

// Returns a bitset (BitsetType) of length size with all the bits set to true.
template<typename BitsetType>
BitsetType bitset_full(typename BitsetType::size_type size) {
  BitsetType result(size);
  result.set();
  return result;
}

/*
template <typename ElementType, template<typename> class ContainerType>
std::ostream& operator<<(std::ostream& os,
    const ContainerType<ElementType>& obj) {
  for (typename ContainerType<ElementType>::const_iterator obj_i(obj.begin());
      obj_i != obj.end(); ++obj_i) {
    if (obj_i != obj.begin()) os << std::endl;
    os << *obj_i;
  }
  return os;
}

template <typename ElementType, template<typename ElementType> class ContainerType>
std::istream& operator>>(std::istream& is, ContainerType<ElementType>& obj) {
  for_each(obj.begin(), obj.end(),
      rabingamesolver_global::OperatorIstream<ElementType>(is)); 
  return is;
}
*/

template <typename ElementType>
std::ostream& operator<<(std::ostream& os, const std::vector<ElementType>& obj)
    {
  for (typename std::vector<ElementType>::const_iterator obj_i(obj.begin());
      obj_i != obj.end(); ++obj_i) {
    if (obj_i != obj.begin()) os << std::endl;
    os << *obj_i;
  }
  return os;
}

template <typename ElementType>
std::istream& operator>>(std::istream& is, std::vector<ElementType>& obj) {
  for_each(obj.begin(), obj.end(),
      rabingamesolver_global::OperatorIstream<ElementType>(is)); 
  return is;
}

template <typename ElementType>
std::ostream& operator<<(std::ostream& os, const std::list<ElementType>& obj) {
  for (typename std::list<ElementType>::const_iterator obj_i(obj.begin());
      obj_i != obj.end(); ++obj_i) {
    if (obj_i != obj.begin()) os << std::endl;
    os << *obj_i;
  }
  return os;
}

template <typename ElementType>
std::istream& operator>>(std::istream& is, std::list<ElementType>& obj) {
  for_each(obj.begin(), obj.end(),
     rabingamesolver_global::OperatorIstream<ElementType>(is)); 
  return is;
}

#endif //RABINGAMESOLVER_GLOBAL_HPP_
