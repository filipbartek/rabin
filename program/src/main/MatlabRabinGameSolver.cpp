#include "MatlabRabinGameSolver.hpp"

#include <ostream> // cout, endl
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

#include <vector>
using std::vector;

#include <memory>
using std::unique_ptr;

#include <ctime> //temp

// Requires -c++0x compiler option
#include <cinttypes> // uint64_t

// A dirty way to prevent matrix.h:330 from redefining char16_t
#define CHAR16_T
#include "engine.h"

#include "Global.hpp" // BitsetType, bitset_empty, bitset_full, debug_cout
#include "RabinGame.hpp"

#define FIELD_Q_LIST "Q_list"
#define FIELD_SIGMA "Sigma"
#define FIELD_Q_ADV "Q_adv"
#define FIELD_Q_ADV_LIST "Q_adv_list"
#define FIELD_COND "Cond"
#define FIELD_DELTA "delta"
#define FIELD_DELTA_ADV "delta_adv"
#define FIELD_W "W"
#define FIELD_PI "pi"
#define VARIABLE_GAME "game"
#define VARIABLE_SOLUTION "solution"

#define min(a,b) (a<=b?a:b)

// Starts MATLAB engine, changes its working directory to script_path and sets
// MATLAB variables newFolder to script_path and oldFolder to the original
// working directory.
void MatlabRabinGameSolver::ep_open(const char* script_path) {
  cout << "Opening MATLAB engine...";
  ep = engOpen(NULL);
  if (!ep) {
	  cerr << "Error: Can't start MATLAB engine. MATLAB solver won't work."
         << endl;
	} else {
	  cout << " done." << endl;
	  if (!script_path) {
      cerr << "Warning: MATLAB script path wasn't specified. Use '-m path' command line option." << endl;
      return;
    }
    cout << "MATLAB script path = " << script_path << endl;
	  { // ss_command
	    stringstream ss_command;
	    ss_command << "newFolder = '" << script_path << "'";
	    engEvalString(ep, ss_command.str().c_str());
	  }
	  engEvalString(ep, "oldFolder = cd(newFolder)");
	  engEvalString(ep, "clear newFolder");
	  // May be a problem: fieldnames disappears after the constructor ends.
	  const char *fieldnames[] = {FIELD_Q_LIST, FIELD_SIGMA, FIELD_Q_ADV,
        FIELD_Q_ADV_LIST, FIELD_COND, FIELD_DELTA,FIELD_DELTA_ADV};
	  pm_game = mxCreateStructMatrix(1, 1, 7, fieldnames);
	  pm_q_list = mxCreateNumericMatrix(1, 0, UINT_MX, mxREAL);
	  mxSetField(pm_game, 0, FIELD_Q_LIST, pm_q_list);
	  pm_sigma = mxCreateNumericMatrix(1, 0, UINT_MX, mxREAL);
	  mxSetField(pm_game, 0, FIELD_SIGMA, pm_sigma);
	  pm_q_adv = mxCreateNumericMatrix(0, 2, UINT_MX, mxREAL);
	  mxSetField(pm_game, 0, FIELD_Q_ADV, pm_q_adv);
	  pm_q_adv_list = mxCreateNumericMatrix(1, 0, UINT_MX, mxREAL);
	  mxSetField(pm_game, 0, FIELD_Q_ADV_LIST, pm_q_adv_list);
	  //pm_cond = mxCreateCellMatrix(2, 0);
	  //mxSetField(pm_game, 0, FIELD_COND, pm_cond);
	  pm_delta = mxCreateLogicalMatrix(0, 0);
	  mxSetField(pm_game, 0, FIELD_DELTA, pm_delta);
	  pm_delta_adv = mxCreateLogicalMatrix(0, 0);
	  mxSetField(pm_game, 0, FIELD_DELTA_ADV, pm_delta_adv);
	  
	  pm_cond = NULL;
    pr_q_list = NULL;
	  pr_sigma = NULL;
	  pr_q_adv = NULL;
	  pr_q_adv_list = NULL;
	  pr_delta = NULL;
	  pr_delta_adv = NULL;
  }
}

MatlabRabinGameSolver::MatlabRabinGameSolver(const char* script_path)
  : RabinGameSolver() {
  ep_open(script_path);
}

MatlabRabinGameSolver::MatlabRabinGameSolver(const RabinGame& game,
    const char* script_path)
  : RabinGameSolver(game) {
  ep_open(script_path);
}

// Closes MATLAB engine.
MatlabRabinGameSolver::~MatlabRabinGameSolver() {
  cout << "Closing MATLAB engine...";
  if (ep) {
    engEvalString(ep, "clear G");
    engEvalString(ep, "cd(oldFolder)");
    engEvalString(ep, "clear oldFolder");
    if(engClose(ep) != 0) {
      cerr << "Warning: engClose(ep) returned not 0 when closing the MATLAB \
engine." << endl;
    } else cout << " done." << endl;
  } else cerr << "Warning: engine is already closed." << endl;
  mxDestroyArray(pm_game);
}

void MatlabRabinGameSolver::update_game() {
  unsigned int n = game_.n();
  
  // Q_list
  unsigned int q_list_size = n + 1;
  mxSetN(pm_q_list, q_list_size);
  pr_q_list = (UINT_T*)mxRealloc(pr_q_list, q_list_size*sizeof(UINT_T));
  mxSetData(pm_q_list, pr_q_list);
  for (unsigned int number_i = 0; number_i < q_list_size; ++number_i) {
    pr_q_list[number_i] = number_i + 1;
  }
  
  // Sigma
  unsigned int sigma_size = n + 1;
  mxSetN(pm_sigma, sigma_size);
  pr_sigma = (UINT_T*)mxRealloc(pr_sigma, sigma_size*sizeof(UINT_T));
  mxSetData(pm_sigma, pr_sigma);
  for (unsigned int number_i = 0; number_i < sigma_size; ++number_i) {
    pr_sigma[number_i] = number_i + 1;
  }
  
  // Q_adv
  unsigned int q_adv_m = q_list_size * sigma_size;
  mxSetM(pm_q_adv, q_adv_m);
  pr_q_adv = (UINT_T*)mxRealloc(pr_q_adv, q_adv_m * 2 * sizeof(UINT_T));
  mxSetData(pm_q_adv, pr_q_adv);
  for (unsigned int i = 0; i < q_list_size; ++i) { // protagonist's vertex
    for (unsigned int j = 0; j < sigma_size; ++j) { // action
      pr_q_adv[i*sigma_size + j] = i + 1;
      pr_q_adv[(q_list_size + i)*sigma_size + j] = j + 1;
    }
  }

  // Q_adv_list
  unsigned int q_adv_list_n = q_adv_m;
  mxSetN(pm_q_adv_list, q_adv_list_n);
  pr_q_adv_list = (UINT_T*)mxRealloc(pr_q_adv_list, q_adv_list_n
    * sizeof(UINT_T));
  mxSetData(pm_q_adv_list, pr_q_adv_list);
  for (unsigned int i = 0; i < q_adv_list_n; ++i) {
    pr_q_adv_list[i] = i + 1;
  }
  
  // Q_Cond
  //mxSetN(pm_cond, k); // This approach oesn't work. I don't know why.
  unsigned int k = game_.k();
  mxDestroyArray(pm_cond);
  pm_cond = mxCreateCellMatrix(2, k);
  unsigned int index = 0;
  for (unsigned int i = 0; i < k; ++i) {
    RabinStreettPair& pair(game_.condition_[i]);
    unsigned int g_count = pair.g_.count();
    mxArray *pm_g = mxCreateNumericMatrix(1, g_count, UINT_MX, mxREAL);
    mxSetCell(pm_cond, index, pm_g);
    UINT_T *pr_g = (UINT_T*)mxCalloc(g_count, sizeof(UINT_T));
    mxSetData(pm_g, pr_g);
    unsigned int g_i = 0;
    for (unsigned int j = 0; j < n; ++j) {
      if (pair.g_.test(j)) {
        pr_g[g_i] = j + 1;
        ++g_i;
      }
    }
    ++index;
    unsigned int r_count = pair.r_.count();
    mxArray *pm_r = mxCreateNumericMatrix(1, r_count, UINT_MX, mxREAL);
    mxSetCell(pm_cond, index, pm_r);
    UINT_T *pr_r = (UINT_T*)mxCalloc(r_count, sizeof(UINT_T));
    mxSetData(pm_r, pr_r);
    unsigned int r_i = 0;
    for (unsigned int j = 0; j < n; ++j) {
      if (pair.r_.test(j)) {
        pr_r[r_i] = j + 1;
        ++r_i;
      }
    }
    ++index;
  }
  mxSetField(pm_game, 0, FIELD_COND, pm_cond);
  
  // delta
  /*unsigned int nzmax = 0;
  for (vector<Vertex>::const_iterator i = game_.game_.begin();
      i != game_.game_.end(); ++i) {
    if (i->player_) {
      nzmax += i->successors_.count();
    } else {
      ++nzmax;
    }
  }
  mxArray *pm_delta = mxCreateSparseLogicalMatrix(n, n + 1, nzmax);*/
  // The original matlab example uses a sparse matrix.
  unsigned int delta_m = q_list_size;
  unsigned int delta_n = sigma_size;
  mxSetM(pm_delta, delta_m);
  mxSetN(pm_delta, delta_n);
  // Frees and newly allocates the memory to clear its contents
  mxFree(pr_delta);
  pr_delta = (mxLogical*)mxCalloc(delta_m * delta_n, sizeof(mxLogical));
  mxSetData(pm_delta, pr_delta);
  for (unsigned int vertex_i = 0; vertex_i < n; ++vertex_i) {
    Vertex& vertex(game_.game_[vertex_i]);
    if (vertex.player_ && vertex.successors_.any()) {
      for (unsigned int vertex_j = 0; vertex_j < n; ++vertex_j) {
        if (vertex.successors_.test(vertex_j))
          pr_delta[vertex_j/*action (target vertex)*/ * q_list_size
            + vertex_i/*source vertex*/] = true;
      }
    } else {
      pr_delta[(sigma_size - 1)/*action (last)*/ * q_list_size
        + vertex_i/*source vertex*/] = true;
      // last action is the extra one for Eve's and empty vertices
    }
  }
  // last vertex:
  pr_delta[(sigma_size - 1)/*action (last)*/ * q_list_size
    + (q_list_size - 1)/*source vertex (last)*/] = true;
  
  // delta_adv
  unsigned int delta_adv_m = q_adv_list_n;
  unsigned int delta_adv_n = q_list_size;
  mxSetM(pm_delta_adv, delta_adv_m);
  mxSetN(pm_delta_adv, delta_adv_n);
  mxFree(pr_delta_adv);
  pr_delta_adv = (mxLogical*)mxCalloc(delta_adv_m * delta_adv_n,
      sizeof(mxLogical));
  mxSetData(pm_delta_adv, pr_delta_adv);
  for (unsigned int vertex_i = 0; vertex_i < q_list_size; ++vertex_i) {
    // i = source vertex
    for (unsigned int vertex_j = 0; vertex_j < sigma_size; ++vertex_j) {
      // j = action or target vertex (Eva)
      if (vertex_i < n && !game_.game_[vertex_i].player_
          && vertex_j < n && game_.game_[vertex_i].successors_.test(vertex_j))
      {
        // j = target vertex, action = last
        pr_delta_adv[vertex_j/*target vertex*/ * delta_adv_m
          + vertex_i/*source vertex*/ * sigma_size
          + (sigma_size - 1)/*(source) action (last)*/] = true;
      }
      if (vertex_j < n // action is common - transition to vertex
          // action is last - transition to last vertex
          || vertex_i == q_list_size - 1 // source vertex is last
          // source vertex is common
          || game_.game_[vertex_i].player_ // source vertex is Adam's
          // source vertex is Eve's
          || game_.game_[vertex_i].successors_.none()) // source vertex has no successors
          // source vertex has a successor (and is common, Eve's, and action is last)
      {
        // j = action and target vertex
        pr_delta_adv[vertex_j/*target vertex*/ * delta_adv_m
          + vertex_i/*source vertex*/ * sigma_size
          + vertex_j/*(source) action*/] = true;
      }
    }
  }
  
  if (ep) {
    engEvalString(ep, "clear G");
    if (engPutVariable(ep, "G", pm_game) != 0) {
      cerr << "Error: Cannot submit game to MATLAB engine." << endl;
    }
	} else cerr << "Error: Updating game: MATLAB engine is closed." << endl;
}

// Returns true on error.
bool MatlabRabinGameSolver::Solve(const bool& winning_set,
    const bool& strategy) {
  if ((winning_set && !winning_set_solved_) || (strategy && !strategy_solved_))
  {
    if (!ep) {
      cerr << "Error: Solving game: MATLAB engine is closed." << endl;
  		return true;
  	}
	  engEvalString(ep, "solution = Main(G)");
	  mxArray *pm_solution = engGetVariable(ep, "solution");
	  engEvalString(ep, "clear solution");
	  if (!pm_solution) {
      cerr << "Error: Couldn't retrieve solution." << endl;
      return true;
    }
    
    unsigned int n = game_.n();
    
    // winning set
    mxArray *pm_winning_set = mxGetField(pm_solution, 0, "W");
	  if (!pm_winning_set) {
      cerr << "Error: Couldn't retrieve winning set (mxArray)." << endl;
      mxDestroyArray(pm_solution);
      return true;
    }
    if (mxGetN(pm_winning_set) > 0) {
  	  UINT_T *pr_winning_set = (UINT_T*)mxGetData(pm_winning_set);
  	  if (!pr_winning_set) {
        cerr << "Error: Couldn't retrieve (nonepmty) winning set (pr)." << endl;
        mxDestroyArray(pm_solution);
        return true;
      }
      for (unsigned int i = 0; i < mxGetN(pm_winning_set); ++i) {
        if (pr_winning_set[i] >= 1 && pr_winning_set[i] <= n)
          winning_set_[pr_winning_set[i]-1] = true;
        else cerr << "Warning: Invalid winning set item retrieved: "
            << pr_winning_set[i]
            << "; vertex (invalid)=" << pr_winning_set[i]-1
            << ", n=" << n << endl;
      }
    }
    winning_set_solved_ = true;
    
    if (strategy && !strategy_solved_) {
      // strategy
      mxArray *pm_strategy = mxGetField(pm_solution, 0, "pi");
      if (!pm_strategy) {
        cerr << "Error: Couldn't retrieve strategy." << endl;
        mxDestroyArray(pm_solution);
        return true;
      }
      if (mxGetN(pm_strategy) != n+1) {
        cerr << "Warning: Invalid strategy size. Expected=" << n+1
            << ", retrieved=" << mxGetN(pm_strategy) << endl;
      }
      if (mxGetN(pm_strategy) > 0) {
        double *pr_strategy = mxGetPr(pm_strategy);
        if (!pr_strategy) {
          cerr << "Error: Couldn't retrieve nonempty strategy (pr)." << endl;
          mxDestroyArray(pm_solution);
          return true;
        }
        for (unsigned int i = 0; i < min(mxGetN(pm_strategy), n); ++i) {
          if (game_.game_[i].player_ && winning_set_.test(i)) {
            unsigned int strategy_item(pr_strategy[i]);
            if (strategy_item >= 1 && strategy_item <= n) {
              strategy_[i] = strategy_item - 1;
            } else {
              cerr << "Error: Invalid strategy item retrieved: "
                << strategy_item << " (as unsigned int), "
                << pr_strategy[i] << " (as double)"
                << "; initial vertex=" << i
                << ", target vertex (invalid)=" << strategy_item - 1
                << ", n=" << n << endl;
              mxDestroyArray(pm_solution);
            }
          }
        }
      }
      strategy_solved_ = true;
    }
    
    mxDestroyArray(pm_solution);
  }
  return false;
}
