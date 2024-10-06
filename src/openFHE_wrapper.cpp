#include "../include/openFHE_wrapper.h"

// Compute the inner product <ca, cb>
// i.e. treat both ciphertexts as vectors and compute their inner product
// Complete function using only EvalAdd, EvalMult, EvalRotate (no EvalInnerProduct)
// Can be done with only log2(vector length) additions and rotations
Ciphertext<DCRTPoly> OpenFHEWrapper::fheInnerProduct(CryptoContext<DCRTPoly> cc, Ciphertext<DCRTPoly> ca, Ciphertext<DCRTPoly> cb) {
    // TODO
    return ca;
}

// Compute the arbitrary polynomial f(x) = x^5 - 3x^4 + 8x - 2 homomorphically
// i.e. compute f(x) on every slot of the inputted ciphertext
// Complete function using only EvalAdd, EvalMult, EvalRotate (no EvalPoly)
// Can be done with a multiplicative depth of only 3
Ciphertext<DCRTPoly> OpenFHEWrapper::fhePolynomial(CryptoContext<DCRTPoly> cc, Ciphertext<DCRTPoly> ctxt) {
    // TODO
    return ctxt;
}

// Outputs relevant metadata of a given CKKS-RNS crypto context
void OpenFHEWrapper::printSchemeDetails(CCParams<CryptoContextCKKSRNS> parameters, CryptoContext<DCRTPoly> cc) {
    cout << "----- CryptoContext Details -----" << endl;
    cout << "\tBatch Size: " << cc->GetEncodingParams()->GetBatchSize() << endl;
    cout << endl;

    cout << "CKKS default parameters: " << parameters << endl;
    cout << endl;

    cout << "\tScaling Mod Size: " << parameters.GetScalingModSize() << endl;
    cout << "\tRing Dimension: " << cc->GetRingDimension() << endl;
    cout << "\tNoise Estimate: " << parameters.GetNoiseEstimate() << endl;
    cout << "\tMultiplicative Depth: " << parameters.GetMultiplicativeDepth() << endl; 
    cout << "\tNoise Level: " << parameters.GetNoiseEstimate() << endl;
    cout << endl;
}

// Outputs relevant internal details of a given ciphertext
void OpenFHEWrapper::printCipherDetails(Ciphertext<DCRTPoly> ctxt) {
    cout << "----- Ciphertext Details -----" << endl;
    cout << "\tBatch Size: " << ctxt->GetSlots() << endl;
    cout << "\tScaling Degree: " << ctxt->GetNoiseScaleDeg() << "\t(delta = " << ctxt->GetScalingFactor() << ")" << endl;
    cout << "\tLevel: " << ctxt->GetLevel() << endl;
    cout << "\tEncoding Parameters: " << ctxt->GetEncodingParameters() << endl;
    cout << endl;
}