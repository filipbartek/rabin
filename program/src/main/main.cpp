// main provides an interface for solving a single game. The game to be solved
// and the solver to be used for it are chosen using the command line
// arguments.

// Command line parameters:
// filename - load the game for solving from filename
// -pxy - solve with Piterman solver, bit x (0 or 1) indicates the usage
//   store optimalisation, bit y (0 or 1) indicates the usage of extreme fix
//   optimalisation
// -h - solve with Horn solver
// -m - solve with MATLAB solver
// -wx - solve winning set
// -sx - solve strategy
// -o filename - save solution and statistics to filename

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <vector>
using std::vector;

#include <sstream>
using std::stringstream;

#include <cstring>
// strlen

#include "rgs.hpp"

enum method_enum {piterman, horn
#ifdef USE_MATLAB
, matlab
#endif // USE_MATLAB
};

int main(int argc, char* argv[]) {
  method_enum method(horn);
  bool piterman_store(true);
  bool piterman_extreme_fix(true);
  bool solve_winning_set(true);
  bool solve_strategy(true);
  vector<const char*> input_filenames;
  char *output_filename(NULL);
#ifdef USE_MATLAB
  char *matlab_script_path(NULL);
#endif // USE_MATLAB
  
  for (int argi = 1; argi < argc; ++argi) {
    if (argv[argi][0] == '-' && argv[argi][1] != '\0') {
      switch (argv[argi][1]) {
        case 'p': // Piterman
        case 'P':
          method = piterman;
          if (argv[argi][2] != '\0') {
            piterman_store = (argv[argi][2] == '1');
            if (argv[argi][3] != '\0')
                piterman_extreme_fix = (argv[argi][3] == '1');
          }
          break;
        case 'h': // Horn
        case 'H':
          method = horn;
          break;
#ifdef USE_MATLAB
        case 'm':
        case 'M':
          method = matlab;
          ++argi;
          if (argi < argc) matlab_script_path = argv[argi];
          break;
#endif // USE_MATLAB
        case 'w': // winning set
        case 'W':
          if (argv[argi][2] != '\0')
            solve_winning_set = (argv[argi][2] == '1');
          break;
        case 's': // strategy
        case 'S':
          if (argv[argi][2] != '\0')
            solve_strategy = (argv[argi][2] == '1');
          break;
        case 'o': // output filename
        case 'O':
          ++argi;
          if (argi < argc) output_filename = argv[argi];
          break;                     
      }
    } else { // input file
      if (argv[argi] != NULL) input_filenames.push_back(argv[argi]);      
    }
  }

  RabinGameSolver *solver(NULL);
  switch (method) {
    case piterman:
      solver = new PitermanRabinGameSolver(piterman_store,
          piterman_extreme_fix);
      cout << "method=Piterman" << endl;
      cout << "optimalisation: store=" << piterman_store << endl;
      cout << "optimalisation: extreme fix=" << piterman_extreme_fix << endl;
      break;
    case horn:
      solver = new HornRabinGameSolver();
      cout << "method=Horn" << endl;
      break;
#ifdef USE_MATLAB
    case matlab:
      solver = new MatlabRabinGameSolver(matlab_script_path);
      cout << "method=MATLAB" << endl;
      break;
#endif // USE_MATLAB
  }

  for (vector<const char*>::const_iterator i = input_filenames.begin();
      i != input_filenames.end(); ++i) {
    const char* input_filename = *i;
    cout << "input filename=" << input_filename << endl;
    RabinGame game;
    bool file_error(false);
    ifstream ifs(input_filename, ifstream::in);
    if (ifs.is_open() && !ifs.fail()) {
      cout << "Loading...";
      ifs >> game;
      cout << " done." << endl;
    } else file_error = true;
    ifs.close();
    if (file_error) {
      cerr << "Loading failed." << endl;
      continue;
    }
    
    solver->game(game);
    clock_t clock_solution;
    cout << "Solving...";
    clock_solution = clock();
    solver->Solve(solve_winning_set, solve_strategy);
    clock_solution = clock() - clock_solution;
    cout << " done." << endl;
    cout << "clock=" << clock_solution << endl;
    if (solve_winning_set)
      cout << "winning set=" << solver->winning_set() << endl;
    if (solve_strategy)
      cout << "strategy:" << endl << solver->strategy() << endl;
    if (output_filename != NULL && strlen(output_filename) > 0) {
      stringstream ss;
      ss << input_filename;
      ss << output_filename;
      cout << "output filename=" << ss.str() << endl;
      ofstream ofs(ss.str().c_str(), ofstream::out);
      if (ofs.is_open() && ofs.good()) {
        cout << "Writing...";
        ofs << game << endl;
        if (solve_winning_set) ofs << solver->winning_set() << endl;
        if (solve_strategy) ofs << solver->strategy() << endl;
        ofs << input_filename << endl;
        ofs << clock_solution << endl;
        ofs << method << endl;
        ofs << solve_winning_set << solve_strategy << endl;
        cout << " done." << endl;
      } else cout << "Writing failed" << endl; 
      ofs.close();
    }   
  }
  if (solver != NULL) delete solver;
  return 0;
}
