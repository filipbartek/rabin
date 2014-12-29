#include "GeneralScaler.hpp"
#include <cstring> // NULL

GeneralScaler::GeneralScaler() : scaler_(NULL) {}

GeneralScaler::~GeneralScaler() {
  TearDown();
}

GeneralScaler::ReturnType GeneralScaler::operator()(BaseType base) {
  if (scaler_ != NULL) return scaler_->operator()(base);
  else return ReturnType();
}

void GeneralScaler::TearDown() {
  if (scaler_ != NULL) delete scaler_;
}

void GeneralScaler::scaler_update() {
  scaler_->parameter_ = parameter_;
}

void GeneralScaler::scaler(Scaler *scaler) {
  TearDown();
  scaler_ = scaler;
  scaler_update();
}

void GeneralScaler::parameter(ParameterType parameter) {
  parameter_ = parameter;
  scaler_update();
}
