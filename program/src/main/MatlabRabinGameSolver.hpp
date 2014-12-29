// Interface for Tumova's Rabin game solver implemented in MATLAB

#ifndef RABINGAMESOLVER_MATLABRABINGAMESOLVER_HPP_
#define RABINGAMESOLVER_MATLABRABINGAMESOLVER_HPP_

#include <string> // string

// A dirty way to prevent matrix.h:330 from redefining char16_t
#define CHAR16_T
#include "engine.h" // Engine

#include "RabinGameSolver.hpp"

// For other files that include this file
#define USE_MATLAB

#define UINT_MX mxUINT64_CLASS
#define UINT_T uint64_t

class RabinGame;

class MatlabRabinGameSolver : public RabinGameSolver {
 public:
  MatlabRabinGameSolver(const char* script_path);
  MatlabRabinGameSolver(const RabinGame& game, const char* script_path);
  ~MatlabRabinGameSolver();
  
  virtual void update_game();
  
  virtual bool Solve(const bool& winning_set, const bool& strategy);
 private:
  void ep_open(const char* script_path);
  
  // Persistent variables:
  Engine *ep;
  mxArray *pm_game;
  mxArray *pm_q_list;
  mxArray *pm_sigma;
  mxArray *pm_q_adv;
  mxArray *pm_q_adv_list;
  mxArray *pm_delta;
  mxArray *pm_delta_adv;
  
  // Temporary variables:
  mxArray *pm_cond;
  UINT_T *pr_q_list;
  UINT_T *pr_sigma;
  UINT_T *pr_q_adv;
  UINT_T *pr_q_adv_list;
  mxLogical *pr_delta;
  mxLogical *pr_delta_adv;
};

#endif //RABINGAMESOLVER_MATLABRABINGAMESOLVER_HPP_
