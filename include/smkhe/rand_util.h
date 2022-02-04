#include "smkhe/util.h"
#include "smkhe/math/polynomial.h"
#include "smkhe/math/ntt_transformer.h"
#include <random>

#ifndef SMKHE_RAND_UTIL_H
#define SMKHE_RAND_UTIL_H

uint64_t getRandom(uint64_t end);

void sampleHWT(vector<Polynomial<uint64_t>> &poly, vector<uint64_t> &primes);

long long getRandomError();

void samplePolynomial(vector<Polynomial<uint64_t>> &poly, vector<uint64_t> &primes, bool isForPublic);

void sampleErrorAndAdd(vector<Polynomial<uint64_t>> &poly, vector<uint64_t> &primes, NTTTransformer &transformer);

#endif //SMKHE_RAND_UTIL_H
