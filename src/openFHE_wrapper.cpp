#include "../include/openFHE_wrapper.h"

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

// decrypts a given ciphertext and returns a vector of its contents
Ciphertext<DCRTPoly> OpenFHEWrapper::encryptFromVector(CryptoContext<DCRTPoly> cc, PublicKey<DCRTPoly> pk, vector<double> vec) {
  Plaintext ptxt = cc->MakeCKKSPackedPlaintext(vec);
  return cc->Encrypt(pk, ptxt);
}


// decrypts a given ciphertext and returns a vector of its contents
vector<double> OpenFHEWrapper::decryptToVector(CryptoContext<DCRTPoly> cc, PrivateKey<DCRTPoly> sk, Ciphertext<DCRTPoly> ctxt) {
  Plaintext ptxt;
  cc->Decrypt(sk, ctxt, &ptxt);
  return ptxt->GetRealPackedValue();
}

// performs any rotation on a ciphertext using 2log_2(batchsize) rotation keys and (1/2)log_2(batchsize) rotations
Ciphertext<DCRTPoly> OpenFHEWrapper::binaryRotate(CryptoContext<DCRTPoly> cc, Ciphertext<DCRTPoly> ctxt, int factor) {
  int batchSize = cc->GetEncodingParams()->GetBatchSize();

  vector<int> neededRotations;
  int factorSign;
  int binaryCounter;
  int currentRotation;

  while(factor != 0) {
    factorSign = factor / abs(factor);

    binaryCounter = pow(2, round(log2(abs(factor))));
    currentRotation = (binaryCounter * factorSign) % batchSize;
    if(currentRotation != 0) {
      neededRotations.push_back(binaryCounter * factorSign);
    }

    factor -= binaryCounter * factorSign;
  }

  for(long unsigned int i = 0; i < neededRotations.size(); i++) {
    ctxt = cc->EvalRotate(ctxt, neededRotations[i]);
  }

  return ctxt;
}

void OpenFHEWrapper::plaintextNormalize(vector<double> &vec, const size_t dim) {
  double magnitude = 0.0;
  for(size_t i = 0 ; i < dim; i++) {
    magnitude += (vec[i] * vec[i]);
  }
  magnitude = sqrt(magnitude);

  if (magnitude != 0) {
    for(size_t i = 0 ; i < dim; i++) {
      vec[i] /= magnitude;
    }
  }
}

// matrix-vector multiplication 
Ciphertext<DCRTPoly> OpenFHEWrapper::matrixMultiply(CryptoContext<DCRTPoly> cc, std::vector<Ciphertext<DCRTPoly>> conc_matrix, Ciphertext<DCRTPoly> query, PublicKey<DCRTPoly> pk) {
    
    Ciphertext<DCRTPoly> rotated_query; 
    Ciphertext<DCRTPoly> product;

    vector<double> resultInit = {0}; 
    Ciphertext<DCRTPoly> result = encryptFromVector(cc, pk, resultInit);

    for (size_t i=0; i < VECTOR_DIM; i++) {
        // rotate the by i and add it to the unrotated
        rotated_query = binaryRotate(cc, query, i);
        product = cc->EvalMult(conc_matrix[i], rotated_query);
        result = cc->EvalAdd(result, product);
    }
    return result; 
}