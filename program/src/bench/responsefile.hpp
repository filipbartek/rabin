// http://www.boost.org/doc/libs/1_48_0/doc/html/program_options/howto.html#id2444807

#ifndef RABINGAMESOLVER_RESPONSEFILE_HPP_
#define RABINGAMESOLVER_RESPONSEFILE_HPP_

#include <utility> // pair
#include <string> // string
#include "boost/program_options.hpp" // variables_map, options_description

std::pair<std::string, std::string> at_option_parser(std::string const&s);

// returns true on error
bool responsefile(boost::program_options::variables_map& vm, const boost::program_options::options_description& desc, const char* name = "response-file");

#endif // RABINGAMESOLVER_RESPONSEFILE_HPP_
