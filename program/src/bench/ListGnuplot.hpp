#ifndef RABINGAMESOLVER_LISTGNUPLOT_HPP_
#define RABINGAMESOLVER_LISTGNUPLOT_HPP_

#include <istream> // istream
#include <ostream> // ostream
#include "nullstream.hpp" // null_stream

void ListGnuplot(std::istream& is, std::ostream& os,
    const unsigned int& column_x, const unsigned int& column_y);

#endif // RABINGAMESOLVER_LISTGNUPLOT_HPP_
