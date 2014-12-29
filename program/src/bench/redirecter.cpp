// redirecter by Thomas Guest
// Source:
// http://wordaligned.org/articles/cpp-streambufs#tocredirect-streams
// Adjusted by Filip Bartek

#include "redirecter.hpp"
#include <ostream>

redirecter::redirecter() : src(NULL), sbuf(NULL) {}

redirecter::redirecter(std::ostream & dst, std::ostream & src_new)
  : src(&src_new), sbuf(src->rdbuf(dst.rdbuf())) {}

redirecter::~redirecter() { src->rdbuf(sbuf); }

void redirecter::set(std::ostream & dst, std::ostream & src_new) {
  if (src != NULL) src->rdbuf(sbuf);
  src = &src_new;
  sbuf = src->rdbuf(dst.rdbuf());
}


optofstream::optofstream(const char* filename, ios_base::openmode mode)
  : ofs(filename, mode) {
  if (ofs.is_open()) redirect.set(ofs, *this);
  else redirect.set(ns, *this);
}

optofstream::~optofstream() {
  if (ofs.is_open()) ofs.close();
}

bool optofstream::active() {
  return ofs.is_open();
}

teestream::teestream(std::ostream & o1, std::ostream & o2)
  : std::ostream(&tbuf)
  , tbuf(o1.rdbuf(), o2.rdbuf()) {}
