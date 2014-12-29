#include "RabinGameSolver.hpp"
#include <algorithm> // fill
#include <vector>

RabinGameSolver::RabinGameSolver() {}

RabinGameSolver::RabinGameSolver(const RabinGame& game) : game_(game),
  winning_set_(n()), strategy_(n(), n()), winning_set_solved_(false),
  strategy_solved_(false) {}

RabinGameSolver::~RabinGameSolver() {}

void RabinGameSolver::game(const RabinGame& game) {
  game_ = game;
  winning_set_.resize(n());
  strategy_.resize(n());
  Unsolve();
}

void RabinGameSolver::Unsolve() {
  winning_set_.reset();
  winning_set_solved_ = false;
  fill(strategy_.begin(), strategy_.end(), n());
  strategy_solved_ = false;
  update_game();
}

const RabinGameSolver::BitsetType/*n*/& RabinGameSolver::winning_set() {
  if (!winning_set_solved_) Solve(true, false);
  return winning_set_;
}

const std::vector<RabinGameSolver::NType>/*n*/& RabinGameSolver::strategy() {
  if (!strategy_solved_) Solve(false, true);
  return strategy_;
}

RabinGameSolver::NType RabinGameSolver::n() const { return game_.n(); }

RabinGameSolver::KType RabinGameSolver::k() const { return game_.k(); }
