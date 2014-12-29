// http://www.boost.org/doc/libs/1_48_0/doc/html/program_options/howto.html#id2444807

#include "responsefile.hpp"

#include <utility> // pair
using std::pair;
using std::make_pair;

#include <string> // string
using std::string;

#include <iostream>
using std::cout;
using std::endl;

#include <fstream> // ifstream
using std::ifstream;

#include <sstream> // stringstream
using std::stringstream;

#include <vector>
using std::vector;

#include <iterator>
using std::back_inserter;

#include <algorithm>
using std::copy;

#include "boost/program_options.hpp"
namespace po = boost::program_options;

#include "boost/tokenizer.hpp"
using boost::tokenizer;
using boost::char_separator;

pair<string, string> at_option_parser(string const&s)
{
    if ('@' == s[0])
        return make_pair(string("response-file"), s.substr(1));
    else
        return pair<string, string>();
}

bool responsefile(po::variables_map& vm, const po::options_description& desc, const char* name) {
	if (vm.count(name)) {
		 cout << "Loading response file " << vm[name].as<string>() << "..." << endl;
		 // Load the file and tokenize it
		 ifstream ifs(vm[name].as<string>().c_str());
		 if (!ifs) {
			 cout << "Could not open the response file\n";
			 return true;
		 }
		 // Read the whole file into a string
		 stringstream ss;
		 ss << ifs.rdbuf();
		 // Split the file content
		 char_separator<char> sep(" \n\r");
		 string ResponsefileContents( ss.str() );
		 tokenizer<char_separator<char> > tok(ResponsefileContents, sep);
		 vector<string> args;
		 copy(tok.begin(), tok.end(), back_inserter(args));
		 // Parse the file and store the options
		 po::store(po::command_line_parser(args).options(desc).run(), vm);
		 po::notify(vm);
		 // Filip Bartek: No recursion
     cout << "done." << endl;
	}
	return false;
}
