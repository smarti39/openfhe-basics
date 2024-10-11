#include "../include/openFHE_wrapper.h"

// Compute the inner product <ca, cb>
// i.e. treat both ciphertexts as vectors and compute their inner product
// Complete function using only EvalAdd, EvalMult, EvalRotate (no EvalInnerProduct)
// Can be done with only log2(vector length) additions and rotations
Ciphertext<DCRTPoly> OpenFHEWrapper::fheInnerProduct(CryptoContext<DCRTPoly> cc, Ciphertext<DCRTPoly> ca, Ciphertext<DCRTPoly> cb) {
    // TODO
    Ciphertext<DCRTPoly> result = cc->EvalMult(ca, cb); 
    size_t batchSize = cc->GetEncodingParams()->GetBatchSize(); 

    for (size_t i = 1; i < batchSize; i <<= 1) {
        // rotate the by i and add it to the unrotated
        result = cc->EvalAdd(result, cc->EvalRotate(result, i));
    }

    return result;
}

// Compute the arbitrary polynomial f(x) = x^5 - 3x^4 + 8x - 2 homomorphically
// i.e. compute f(x) on every slot of the inputted ciphertext
// Complete function using only EvalAdd, EvalMult, EvalRotate (no EvalPoly)
// Can be done with a multiplicative depth of only 3
Ciphertext<DCRTPoly> OpenFHEWrapper::fhePolynomial(CryptoContext<DCRTPoly> cc, Ciphertext<DCRTPoly> ctxt) {
    // TODO
    Ciphertext<DCRTPoly> cpyTxt = ctxt; 
    Ciphertext<DCRTPoly> term2;
    Ciphertext<DCRTPoly> term3;
    
    ctxt = cc->EvalMult(ctxt, ctxt); // squared 
    term2 = cc->EvalMult(ctxt, ctxt); // 4th power 
    ctxt = cc->EvalMult(term2, cpyTxt); // 5th power 

    // -3 x^4
    term2 = cc->EvalMult(term2, -3);

    // 8x
    term3 = cc->EvalMult(cpyTxt, 8);

    // add them all together 
    ctxt = cc->EvalAdd(ctxt, term2);
    ctxt = cc->EvalAdd(ctxt, term3);
    ctxt = cc->EvalAdd(ctxt, -2);
    
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