// ** Wrapper functions for the OpenFHE library

#pragma once

#include "config.h"
#include "openfhe.h"

using namespace std;
using namespace lbcrypto;

namespace OpenFHEWrapper {

void printSchemeDetails(CCParams<CryptoContextCKKSRNS> parameters, CryptoContext<DCRTPoly> cc);

void printCipherDetails(Ciphertext<DCRTPoly> ctxt);

Ciphertext<DCRTPoly> fheInnerProduct(CryptoContext<DCRTPoly> cc, Ciphertext<DCRTPoly> ca, Ciphertext<DCRTPoly> cb);

Ciphertext<DCRTPoly> fhePolynomial(CryptoContext<DCRTPoly> cc, Ciphertext<DCRTPoly> ctxt);

}