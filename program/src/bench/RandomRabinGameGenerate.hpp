#ifndef RABINGAMESOLVER_RANDOMRABINGAMEGENERATE_HPP_
#define RABINGAMESOLVER_RANDOMRABINGAMEGENERATE_HPP_

#include "Global.hpp" // NType, KType

class RabinGame;

RabinGame RandomRabinGameGenerate(rabingamesolver_global::NType n,
    rabingamesolver_global::KType k, unsigned int seed = 1);

#endif // RABINGAMESOLVER_RANDOMRABINGAMEGENERATE_HPP_
