#include "ListGnuplot.hpp"

#include <string>
using std::string;
using std::getline;

#include <sstream>
using std::stringstream;

#include <istream>
using std::istream;

#include <ostream>
using std::ostream;
using std::endl;
using std::fixed;

#include <vector>
using std::vector;

#include <map>
using std::multimap;

#include <utility>
using std::pair;
using std::make_pair;

#include <ctime> // CLOCKS_PER_SEC

#include "boost/tuple/tuple.hpp" // tuple, get
using ::boost::tuple;
using ::boost::get;
using ::boost::make_tuple;

#include <cmath> // sqrt

#define max(a,b) (a>=b?a:b)

typedef multimap<pair<unsigned int, unsigned int>, vector<unsigned int> >
    MultimapType; 

// First line is interpreted as column labels.
// Only lines that contain x and y are taken.
tuple<MultimapType, pair<string, string>, vector<string> > ListValues(istream& is,
    const unsigned int& column_x, const unsigned int& column_y) {
  MultimapType values_multimap;
  vector<string> labels;
  pair<string, string> labels_xy;
  unsigned int column_max = max(column_x, column_y);
  bool first_line = true;
  string line;
  while (is.good()) {
    getline(is, line);
    stringstream lineStream(line);
    unsigned int x;
    unsigned int y;
    unsigned int column_i = 0;
    vector<unsigned int> values;
    while (lineStream.good()) {
      string cell;
      getline(lineStream, cell, ',');
      stringstream cellStream(cell);
      if (column_i == column_x) {
        if (!first_line) cellStream >> x;
        else labels_xy.first = cell;
      }
      if (column_i == column_y) {
        if (!first_line) cellStream >> y;
        else labels_xy.second = cell;
      }
      if (column_i != column_x && column_i != column_y) {
        if (!first_line) {
          unsigned int value;
          cellStream >> value;
          values.push_back(value);
        } else {
          labels.push_back(cell);
        }
      }
      ++column_i;
    }
    if (!first_line) {
      // Only columns that contain both x and y are taken
      if (column_i >= column_max) {
        values_multimap.insert(make_pair(make_pair(x, y), values));
      }
    } else first_line = false;
  }
  return make_tuple(values_multimap, labels_xy, labels);
}

// x = lines
// y = cells in a line
void ValuesGnuplot(const tuple<MultimapType, pair<string, string>, vector<string> >& values,
    ostream& os) {
  os << "#CLOCKS_PER_SEC=" << CLOCKS_PER_SEC << "\n";
  // Labels:
  os << "#1(x)-" << get<1>(values).first << " 2(y)-" << get<1>(values).second;
  unsigned int column_i = 3;
  for (vector<string>::const_iterator i(get<2>(values).begin());
      i != get<2>(values).end(); ++i) {
    os << " " << column_i << "-" << *i << "-sum";
    ++column_i;
    os << " " << column_i << "-" << *i << "-num";
    ++column_i;
    os << " " << column_i << "-" << *i << "-mean";
    ++column_i;
    os << " " << column_i << "-" << *i << "-deviation";
    ++column_i;
  }
  os << "\n";

  unsigned int x(0);
  unsigned int y(0);
  const MultimapType& values_multimap = get<0>(values);
  MultimapType::const_iterator i_valuebegin(values_multimap.begin());
  vector<pair<unsigned int, unsigned int> > fields;
  for (MultimapType::const_iterator i(values_multimap.begin()); ; ++i) {
    unsigned int value_x(0);
    unsigned int value_y(0);
    if (i != values_multimap.end()) {
      value_x = i->first/*key*/.first/*x*/;
      value_y = i->first/*key*/.second/*y*/;
    }
    if (i == values_multimap.end() || value_x != x || value_y != y) {
      os << x << " " << y;
      for (unsigned int field_i = 0; field_i < fields.size(); ++field_i) {
        unsigned int value_sum = fields[field_i].first;
        unsigned int value_num = fields[field_i].second;
        double value_mean = 0;
        double value_deviation = 0;
        if (value_num > 0) {
          value_mean = value_sum / value_num;
          double deviation_pow_sum(0);
          for (MultimapType::const_iterator j(i_valuebegin); j != i; ++j) {
            if (field_i < j->second.size()) {
              double deviation(j->second[field_i] - value_mean);
              deviation_pow_sum += deviation * deviation;
            }
          }
          value_deviation = sqrt(deviation_pow_sum / value_num);
        }
        os << " " << value_sum << " " << value_num << " " << value_mean << " "
          << value_deviation;
      }
      os << "\n";
      if (i == values_multimap.end()) break;
      fields.clear();
      i_valuebegin = i;
      if (value_x != x) os << "\n"; // Group lines by x
      x = value_x;
      y = value_y;
    }
    for (unsigned int value_i = 0; value_i < i->second.size(); ++value_i) {
      if (fields.size() <= value_i) {
        fields.push_back(make_pair(i->second[value_i], 1));
      } else {
        fields[value_i].first += i->second[value_i];
        ++fields[value_i].second;
      }
    }
  }
}

void ListGnuplot(istream& is, ostream& os,
    const unsigned int& column_x, const unsigned int& column_y) {
  ValuesGnuplot(ListValues(is, column_x, column_y), os);
}
