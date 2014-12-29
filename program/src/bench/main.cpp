// Program solves random games for various n and k with various solvers.

// Parameters:
// edge probability
// player probability
// g probability
// r probability
// nbase: start, end, function
// kbase: start, end, function
// seed: begin, num
// trace order of n, k
// save solutions?
// compare solutions?
// output file for table n*k, averaged across seeds, fixed probabilities
// output file for all the results
// output file for warnings and errors
// input file for settings 
// solve strategy? - yes, no, both
// trace method - diagonal, square-static, square-dynamic 

#include <ctime> // clock_t, clock

#include <sstream>
using std::stringstream;

#include <fstream> // ofstream, ifstream
using std::ofstream;
using std::ifstream;

#include <iostream> // endl
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::ostream;

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <utility>
using std::pair;
using std::make_pair;

#include "nullstream.hpp" // null_stream

#include "boost/program_options.hpp"
namespace po = boost::program_options;

#include "boost/tuple/tuple.hpp" // tuple, get
using ::boost::tuple;
using ::boost::get;

#include "nullstream.hpp"

#include "rgs.hpp"
using rabingamesolver_global::NType;
using rabingamesolver_global::KType;
using rabingamesolver_global::BitsetType;


#include "RandomRabinGameGenerate.hpp"

#include "redirecter.hpp"

#include "TwoDTracer.hpp"
#include "GeneralTwoDTracer.hpp"
#include "DiagonalTwoDTracer.hpp"
#include "DynamicTwoDTracer.hpp"
#include "SquareTwoDTracer.hpp"

#include "ListGnuplot.hpp"

#include "Scaler.hpp"
#include "GeneralScaler.hpp"
#include "ConstantScaler.hpp"
#include "LinearScaler.hpp"
#include "PolynomialScaler.hpp"
#include "ExponentialScaler.hpp"
#include "FactorialScaler.hpp"

#include "responsefile.hpp"
// relevant to program options

typedef pair<RabinGameSolver*, string/*name*/> namedrabingamesolver;
typedef pair<namedrabingamesolver, bool/*solve strategy*/> taggedrabingamesolver;

// ON stands for "option name"
#define ON_HELP "help,H"
#define ON_RESPONSEFILE "response-file"
#define ON_FOUT_ERR "fout-err"
#define ON_FOUT_DEBUG "fout-debug"
#define ON_FOUT_LIST "fout-list"
#define ON_FIN_LIST "fin-list"
#define ON_FIN_LIST_VALUECOLUMN "fin-list-valuecolumn"
#define ON_FOUT_TABLE "fout-table"
#define ON_FOUT_GAME "fout-game"
#define ON_FOUT_GAME_PREFIX "fout-game-prefix"
#define ON_FOUT_GAME_SUFFIX "fout-game-suffix"
#define ON_SOLVE_FULL "solve-full"
#define ON_SOLVE_JUSTWINNINGSET "solve-justwinningset"
#define ON_SOLVER_PITERMAN_STOREOPT_ON "solver-piterman-storeopt-on"
#define ON_SOLVER_PITERMAN_STOREOPT_OFF "solver-piterman-storeopt-off"
#define ON_SOLVER_HORN "solver-horn"
#ifdef USE_MATLAB
#define ON_SOLVER_MATLAB "solver-matlab"
#define ON_SOLVER_MATLAB_PATH "solver-matlab-path"
#endif // USE_MATLAB
#define ON_N_BEGIN "n-begin"
#define ON_N_END "n-end"
#define ON_N_SCALE_TYPE "n-scale-type" // constant, linear, polynomial, exponential, factorial
#define ON_N_SCALE_PARAM "n-scale-param"
#define ON_K_BEGIN "k-begin"
#define ON_K_END "k-end"
#define ON_K_SCALE_TYPE "k-scale-type" // constant, linear, polynomial, exponential, factorial
#define ON_K_SCALE_PARAM "k-scale-param"
#define ON_NK_TRACE_METHOD "nk-trace-method" // diagonal, square, dynamic
#define ON_NK_TRACE_DIRECTION "nk-trace-direction" // n, k - along axis
#define ON_SEED_BEGIN "seed-begin"
#define ON_SEED_NUM "seed-num"

// OV stands for "option variable"
#define OV_RESPONSEFILE "bench.rsp"
#define OV_FOUT_ERR "error.log"
#define OV_FIN_LIST_VALUECOLUMN 5
#define OV_FOUT_GAME false
#define OV_SOLVE true
#define OV_SOLVE_FULL true
#define OV_SOLVE_JUSTWINNINGSET false
#define OV_SOLVER_PITERMAN_STOREOPT_ON true
#define OV_SOLVER_PITERMAN_STOREOPT_OFF false
#define OV_SOLVER_HORN true
#ifdef USE_MATLAB
#define OV_SOLVER_MATLAB false
#endif // USE_MATLAB
#define OV_N_BEGIN 0
#define OV_N_END 0
#define OV_N_SCALE_TYPE "linear"
#define OV_N_SCALE_PARAM 1
#define OV_K_BEGIN 0
#define OV_K_END 0
#define OV_K_SCALE_TYPE "linear"
#define OV_K_SCALE_PARAM 1
#define OV_NK_TRACE_METHOD "diagonal"
#define OV_NK_TRACE_DIRECTION "n"
#define OV_SEED_BEGIN 1
#define OV_SEED_NUM 1

enum tracemethod {tm_diagonal, tm_square, tm_dynamic};
#define TRACEMETHOD_DEF tm_diagonal
tracemethod string_tracemethod(const string& s) {
  if (s[0] == 'd') {
    if (s[1] == 'i') return tm_diagonal;
    if (s[1] == 'y') return tm_dynamic;
  }
  if (s[0] == 's') return tm_square;
  return TRACEMETHOD_DEF;
}

enum direction {direction_n, direction_k};
#define DIRECTION_DEF direction_n
direction string_direction(const string& s) {
  if (s[0] == 'n') return direction_n;
  if (s[0] == 'k') return direction_k;
  return DIRECTION_DEF;
}

// scale_constant doesn't use base
// scale_factorial doesn't use scale-param
enum scale {scale_constant, scale_linear/*default*/, scale_polynomial, scale_exponential, scale_factorial};
#define SCALE_DEF scale_linear
scale string_scale(const string& s) {
  if (s[0] == 'c') return scale_constant;
  if (s[0] == 'l') return scale_linear;
  if (s[0] == 'p') return scale_polynomial;
  if (s[0] == 'e') return scale_exponential;
  if (s[0] == 'f') return scale_factorial;
  return SCALE_DEF;
}

int main(int argc, char* argv[]) {
  null_stream ns;

  string ov_fout_err(OV_FOUT_ERR);
  string ov_fout_debug;
  string ov_fout_list;
  string ov_fin_list;
  unsigned int ov_fin_list_valuecolumn;
  string ov_fout_table;
  bool ov_fout_game;
  string ov_fout_game_prefix;
  string ov_fout_game_suffix;
  bool ov_solve_full(OV_SOLVE_FULL);
  bool ov_solve_justwinningset(OV_SOLVE_JUSTWINNINGSET);
  bool ov_solver_piterman_storeopt_on(OV_SOLVER_PITERMAN_STOREOPT_ON);
  bool ov_solver_piterman_storeopt_off(OV_SOLVER_PITERMAN_STOREOPT_OFF);
  bool ov_solver_horn(OV_SOLVER_HORN);
#ifdef USE_MATLAB
  bool ov_solver_matlab(OV_SOLVER_MATLAB);
  string ov_solver_matlab_path;
#endif // USE_MATLAB
  NType ov_n_begin(OV_N_BEGIN);
  NType ov_n_end(OV_N_END);
  string ov_n_scale_type(OV_N_SCALE_TYPE);
  NType ov_n_scale_param(OV_N_SCALE_PARAM);
  KType ov_k_begin(OV_K_BEGIN);
  KType ov_k_end(OV_K_END);
  string ov_k_scale_type(OV_K_SCALE_TYPE);
  NType ov_k_scale_param(OV_K_SCALE_PARAM);
  string ov_nk_trace_method(OV_NK_TRACE_METHOD);
  string ov_nk_trace_direction(OV_NK_TRACE_DIRECTION);
  unsigned int ov_seed_begin(OV_SEED_BEGIN);
  unsigned int ov_seed_num(OV_SEED_NUM);

  po::options_description desc("Allowed options");
  desc.add_options()
    (ON_HELP, "produce help message")
    (ON_RESPONSEFILE, po::value<string>()->default_value(OV_RESPONSEFILE),
        "can be specified with '@name', too")
    (ON_FOUT_ERR, po::value<string>(&ov_fout_err)->default_value(OV_FOUT_ERR),
        "a file for output of all errors and warnings")
    (ON_FOUT_DEBUG, po::value<string>(&ov_fout_debug),
        "a file for output of debug information")
    (ON_FOUT_LIST, po::value<string>(&ov_fout_list),
        "a CSV file for putting results of all solutions")
    (ON_FIN_LIST, po::value<string>(&ov_fin_list),
        "a CSV file with results of solutions for generating table")
    (ON_FIN_LIST_VALUECOLUMN,
        po::value<unsigned int>(&ov_fin_list_valuecolumn)->
        default_value(OV_FIN_LIST_VALUECOLUMN),
        "column in list file that contains the values that are used to fill the table")
    (ON_FOUT_TABLE, po::value<string>(&ov_fout_table),
        "a CSV file for table output")
    (ON_FOUT_GAME, po::value<bool>(&ov_fout_game)->
        default_value(OV_FOUT_GAME),
        "save games in files?")
    (ON_FOUT_GAME_PREFIX, po::value<string>(&ov_fout_game_prefix),
        "when saving games, this string is prepended to the name of the game in filename")
    (ON_FOUT_GAME_SUFFIX, po::value<string>(&ov_fout_game_suffix),
        "when saving games, this string is apended to the name of the game in filename")
    (ON_SOLVE_FULL, po::value<bool>(&ov_solve_full)->
        default_value(OV_SOLVE_FULL),
        "solve both winning set and strategy?")
    (ON_SOLVE_JUSTWINNINGSET, po::value<bool>(&ov_solve_justwinningset)->
        default_value(OV_SOLVE_JUSTWINNINGSET),
        "solve just winning set?")
    (ON_SOLVER_PITERMAN_STOREOPT_ON,
        po::value<bool>(&ov_solver_piterman_storeopt_on)->
        default_value(OV_SOLVER_PITERMAN_STOREOPT_ON),
        "solve with Piterman's solver with store optimization?")
    (ON_SOLVER_PITERMAN_STOREOPT_OFF,
        po::value<bool>(&ov_solver_piterman_storeopt_off)->
        default_value(OV_SOLVER_PITERMAN_STOREOPT_OFF),
        "solve with Piterman's solver without store optimization?")
    (ON_SOLVER_HORN, po::value<bool>(&ov_solver_horn)->
        default_value(OV_SOLVER_HORN),
        "solve with Horn's solver?")
#ifdef USE_MATLAB
    (ON_SOLVER_MATLAB, po::value<bool>(&ov_solver_matlab)->
        default_value(OV_SOLVER_MATLAB),
        "solve with MATLAB solver?")
    (ON_SOLVER_MATLAB_PATH, po::value<string>(&ov_solver_matlab_path),
        "path with MATLAB solver, relative to the default MATLAB engine working directory or absolute, must include MATLAB function Main")
#endif // USE_MATLAB
    (ON_N_BEGIN, po::value<NType>(&ov_n_begin)->default_value(OV_N_BEGIN),
        "initial value of n (number of vertices)")
    (ON_N_END, po::value<NType>(&ov_n_end)->default_value(OV_N_END),
        "final value of n plus 1, 0 for infinite")
    (ON_N_SCALE_TYPE, po::value<string>(&ov_n_scale_type)->
        default_value(OV_N_SCALE_TYPE),
        "n scale: type (constant, linear, polynomial, exponential, factorial)")
    (ON_N_SCALE_PARAM, po::value<NType>(&ov_n_scale_param)->
        default_value(OV_N_SCALE_PARAM),
        "n scale: parameter; scale type fact doesn't use parameter")
    (ON_K_BEGIN, po::value<KType>(&ov_k_begin)->default_value(OV_K_BEGIN),
        "initial value of k (number of basic conditions)")
    (ON_K_END, po::value<KType>(&ov_k_end)->default_value(OV_K_END),
        "final value of k plus 1, 0 for infinite")
    (ON_K_SCALE_TYPE, po::value<string>(&ov_k_scale_type)->
        default_value(OV_K_SCALE_TYPE),
        "k scale: type (constant, linear, polynomial, exponential, factorial)")
    (ON_K_SCALE_PARAM, po::value<KType>(&ov_k_scale_param)->
        default_value(OV_K_SCALE_PARAM),
        "k scale: parameter; scale type fact doesn't use parameter")
    (ON_NK_TRACE_METHOD, po::value<string>(&ov_nk_trace_method)->
        default_value(OV_NK_TRACE_METHOD),
        "tracing: method (diagonal, square, dynamic)")
    (ON_NK_TRACE_DIRECTION, po::value<string>(&ov_nk_trace_direction)->
        default_value(OV_NK_TRACE_DIRECTION),
        "tracing: heading direction (n, k)")
    (ON_SEED_BEGIN, po::value<unsigned int>(&ov_seed_begin)->
        default_value(OV_SEED_BEGIN), "initial value of seed")
    (ON_SEED_NUM, po::value<unsigned int>(&ov_seed_num)->
        default_value(OV_SEED_NUM),
        "number of seeds to use, seeds (seed-begin + k) for k = 0..(seed-num - 1) are used")
  ;
  
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).extra_parser(at_option_parser).run(), vm);
  po::notify(vm);
  
  if (vm.count("help")) {
    cout << desc << endl;
    return 1;
  }
  
  responsefile(vm, desc, ON_RESPONSEFILE);

  optofstream os_fout_err(ov_fout_err.c_str());
  teestream teestream_err(cerr, os_fout_err);
  redirecter redirecter_err(teestream_err, cerr);

  optofstream os_fout_debug(ov_fout_debug.c_str());
  ostream os_debug(os_fout_debug.rdbuf());
#ifdef DEBUG
  teestream teestream_debug(cout, os_fout_debug);
  redirecter redirecter_debug(teestream_debug, os_debug);
#endif // DEBUG
  os_debug << "Starting debug stream. Debug information will be printed here."
      << endl;

  // Generate a table with mean times from list
  {
    optofstream os_fout_table(ov_fout_table.c_str());
    if (os_fout_table.active()) {
      ifstream ifs_fin_list(ov_fin_list.c_str());
      if (ifs_fin_list.is_open()) {
        ListGnuplot(ifs_fin_list, os_fout_table, 0, 1);
        ifs_fin_list.close();
      }
    }
  }

  // Solvers
  vector<namedrabingamesolver> solvers;
  if (ov_solver_piterman_storeopt_on)
	solvers.push_back(make_pair(
		new PitermanRabinGameSolver(true, true), "PitermanStOptOn"));
  if (ov_solver_piterman_storeopt_off)
	solvers.push_back(make_pair(
		new PitermanRabinGameSolver(true, false), "PitermanStOptOff"));
  if (ov_solver_horn)
	solvers.push_back(make_pair(new HornRabinGameSolver(), "Horn"));
#ifdef USE_MATLAB
  if (ov_solver_matlab) {
    solvers.push_back(make_pair(
      new MatlabRabinGameSolver(ov_solver_matlab_path.c_str()), "MATLAB"));
	}
#endif // USE_MATLAB

  // Jobs
  vector<taggedrabingamesolver> jobs;
  for (vector<namedrabingamesolver>::const_iterator i(solvers.begin());
      i != solvers.end(); ++i) {
    if (ov_solve_full) jobs.push_back(make_pair(
        make_pair(i->first, i->second + "-full"), true));
    if (ov_solve_justwinningset) jobs.push_back(make_pair(
        make_pair(i->first, i->second + "-justwinningset"), false));
  }

  optofstream os_fout_list(ov_fout_list.c_str());
  os_fout_list << "n_base,k_base,n,k,seed";
  for (vector<taggedrabingamesolver>::iterator i(jobs.begin());
      i != jobs.end(); ++i) {
    os_fout_list << "," << i->first.second;
  }
  os_fout_list << endl;
  
  scale n_scale_type(string_scale(ov_n_scale_type));
  GeneralScaler n_scaler;
  switch (n_scale_type) {
    case scale_constant:
      n_scaler.scaler(new ConstantScaler());
      break;
    case scale_linear:
      n_scaler.scaler(new LinearScaler());
      break;
    case scale_polynomial:
      n_scaler.scaler(new PolynomialScaler());
      break;
    case scale_exponential:
      n_scaler.scaler(new ExponentialScaler());
      break;
    case scale_factorial:
      n_scaler.scaler(new FactorialScaler());
      break;
  }
  n_scaler.parameter(ov_n_scale_param);

  scale k_scale_type(string_scale(ov_k_scale_type));
  GeneralScaler k_scaler;
  switch (k_scale_type) {
    case scale_constant:
      k_scaler.scaler(new ConstantScaler());
      break;
    case scale_linear:
      k_scaler.scaler(new LinearScaler());
      break;
    case scale_polynomial:
      k_scaler.scaler(new PolynomialScaler());
      break;
    case scale_exponential:
      k_scaler.scaler(new ExponentialScaler());
      break;
    case scale_factorial:
      k_scaler.scaler(new FactorialScaler());
      break;
  }
  k_scaler.parameter(ov_k_scale_param);

  tracemethod nk_trace_method(string_tracemethod(ov_nk_trace_method));
  GeneralTwoDTracer tracer;
  switch (nk_trace_method) {
    case tm_diagonal:
      tracer.tracer(new DiagonalTwoDTracer());
      break;
    case tm_dynamic:
      tracer.tracer(new DynamicTwoDTracer());
      break;
    case tm_square:
      tracer.tracer(new SquareTwoDTracer());
      break;
  }
  direction nk_trace_direction(string_direction(ov_nk_trace_direction));
  tracer.parameters(ov_n_begin, ov_n_end,
      ov_k_begin, ov_k_end, nk_trace_direction == direction_n);
  
  while(!tracer.TestEnd()) {
    NType n_base(tracer.x());
    KType k_base(tracer.y());
    NType n_scaled(n_scaler(n_base));
    KType k_scaled(k_scaler(k_base));
    clock_t clock_all(clock());
    for (unsigned int seed(ov_seed_begin); seed < ov_seed_begin + ov_seed_num;
        ++seed) {
      cout << "n_base=" << n_base << endl;
      cout << "k_base=" << k_base << endl;
      cout << "n_scaled=" << n_scaled << endl;
      cout << "k_scaled=" << k_scaled << endl;
      cout << "seed=" << seed << endl;
      stringstream ss;
      ss << n_scaled << "," << k_scaled << "," << seed;
      string game_name(ss.str());
      os_fout_list << n_base << "," << k_base << "," << game_name;
      cout << "Generating game..." << flush;
      clock_t clock_generate(clock());
      RabinGame game(RandomRabinGameGenerate(n_scaled, k_scaled, seed));
      clock_generate = clock() - clock_generate;
      cout << " done in " << clock_generate << " cycles." << endl;
      string game_filename("");
      if (ov_fout_game) {
        stringstream ss;
        ss << ov_fout_game_prefix << n_scaled << "-" << k_scaled << "-"
            << seed << ov_fout_game_suffix;
        game_filename = ss.str();
        cout << "Saving game to file: " << game_filename << endl;
      }
      optofstream os_fout_game(game_filename.c_str());
      if (os_fout_game.active()) {
        cout << "Saving game..." << flush;
        os_fout_game << game << "\n";
        cout << " done." << endl;
      }
      BitsetType winning_set_ref;
      string winning_set_ref_name;
      bool winning_set_ref_set(false);
      vector<NType> strategy_ref;
      string strategy_ref_name;
      bool strategy_ref_set(false);
      BitsetType winning_set_cur;
      vector<NType> strategy_cur;
      for (vector<taggedrabingamesolver>::iterator i(jobs.begin());
          i != jobs.end(); ++i) {
        i->first.first->game(game);
        cout << "solver=" << i->first.second << endl;
        cout << "Solving..." << flush;
        clock_t clock_solve(clock());
        bool response = i->first.first->Solve(true, i->second);
        clock_solve = clock() - clock_solve;
        cout << " done in " << clock_solve << "cycles." << endl;
        os_fout_list << "," << clock_solve;
        if (response) {
          cerr << "Error: Solving failed." << endl;
          cerr << "Game:\n" << game << endl;
        } else {
          BitsetType winning_set_cur = i->first.first->winning_set();
          if (winning_set_ref_set) {
            if (winning_set_cur != winning_set_ref) {
              cerr << "Error: winning set mismatch: ref="
                  << winning_set_ref_name << ", cur=" << i->first.second
                  << ", game=(" << game_name << ")" << endl;
              cerr << "winning_set_cur=" << winning_set_cur << endl;
              cerr << "winning_set_ref=" << winning_set_ref << endl;
            }
          } else {
            winning_set_ref = winning_set_cur;
            winning_set_ref_name = i->first.second;
            winning_set_ref_set = true;
          }
  
          if (i->second) {
            strategy_cur = i->first.first->strategy();
            if (strategy_ref_set) {
              if (strategy_cur != strategy_ref) {
                cerr << "Warning: strategy mismatch: ref="
                    << strategy_ref_name << ", cur=" << i->first.second
                    << ", game=(" << game_name << ")" << endl;
                cerr << "strategy_cur:\n" << strategy_cur << endl;
                cerr << "strategy_ref:\n" << strategy_ref << endl;
              }
            } else {
              strategy_ref = strategy_cur;
              strategy_ref_name = i->first.second;
              strategy_ref_set = true;
            }
          }
        }
      }
      os_fout_list << endl;
    }
    clock_all = clock() - clock_all;
    tracer.Advance(clock_all);
  }
  os_debug << "Destroying solvers..." << flush;
  for (vector<namedrabingamesolver>::iterator i(solvers.begin());
      i != solvers.end(); ++i) {
    delete i->first;
  }
  os_debug << " done." << endl;
  os_debug << "Quitting." << endl;
  return 0;
}
