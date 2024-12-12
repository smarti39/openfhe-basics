#include "../include/config.h"
#include "../include/openFHE_wrapper.h"
#include "../include/plaintext.h"
#include "openfhe.h"

using namespace lbcrypto;
using namespace std;
using namespace OpenFHEWrapper;

// Entry point of the application that orchestrates the flow

int main(int argc, char *argv[]) {

  // The only parameter you will need to modify is multiplicative depth
  // Which is located in ../include/config.h

  // ----- Don't touch anything in the section below -----
  cout << "Setting up parameters..." << endl;
  CCParams<CryptoContextCKKSRNS> parameters;
  parameters.SetSecurityLevel(HEStd_128_classic);
  parameters.SetMultiplicativeDepth(MULT_DEPTH);
  parameters.SetScalingModSize(45);

  CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);
  cc->Enable(PKE);
  cc->Enable(KEYSWITCH);
  cc->Enable(LEVELEDSHE);
  cc->Enable(ADVANCEDSHE);

  size_t batchSize = cc->GetEncodingParams()->GetBatchSize();
  // ----- Don't touch anything in the section above -----

  // Begin key generation operations
  cout << "Generating keys..." << endl;
  auto keyPair = cc->KeyGen();
  auto pk = keyPair.publicKey;
  auto sk = keyPair.secretKey;
  cc->EvalMultKeyGen(sk);
  vector<int> binaryRotationFactors;
  for(int i = 1; i < int(batchSize); i *= 2) {
    binaryRotationFactors.push_back(i);
    binaryRotationFactors.push_back(-i);
  }
  cc->EvalRotateKeyGen(sk, binaryRotationFactors);
  // End key generation operations


  // Begin reading in vectors from input file
  cout << "Reading in vectors..." << endl;
  ifstream fileStream;
  if (argc > 1) {
    fileStream.open(argv[1], ios::in);
  } else {
    cerr << "Error: no input file specified" << endl;
    return 1;
  }

  if (!fileStream.is_open()) {
    cerr << "Error: input file not found" << endl;
    return 1;
  }

  size_t numVectors;
  fileStream >> numVectors;

  vector<double> queryVector(VECTOR_DIM);
  for (size_t i = 0; i < VECTOR_DIM; i++) {
    fileStream >> queryVector[i];
  }
  OpenFHEWrapper::plaintextNormalize(queryVector, VECTOR_DIM);

  vector<vector<double>> dbVectors(numVectors, vector<double>(VECTOR_DIM));
  for (size_t i = 0; i < numVectors; i++) {
    for (size_t j = 0; j < VECTOR_DIM; j++) {
      fileStream >> dbVectors[i][j];
    }
    OpenFHEWrapper::plaintextNormalize(dbVectors[i], VECTOR_DIM);
  }
  fileStream.close();
  // End reading in vectors from input file


  // Diagonal MVM implementation goes below
  // queryVector is 1-D of length 512, already normalized
  // dbVectors is 2-D of size 512x1024, all vectors already normalized

  printMatrix(dbVectors);
  std::cout << std::endl;
  std::vector<std::vector<std::vector<double>>> squareMatrices = splitIntoSquareMatrices(dbVectors, VECTOR_DIM);
  /*for (auto &matrix: squareMatrices) { 
      printMatrix(matrix);
      std::cout << std::endl;
  }*/
  
  std::vector<std::vector<std::vector<double>>> allDiagonalMatrices;
  
  for (auto &squareMatrix: squareMatrices) {
      std::vector<std::vector<double>> diagonals = preprocessToDiagonalForm(squareMatrix);
      //printMatrix(diagonals);
      allDiagonalMatrices.push_back(diagonals);
  }

  std::vector<std::vector<double>> concatenatedRows = concatenateRows(allDiagonalMatrices);
  std::cout << "\nConcatenated Rows:" << std::endl;
  for (const auto& row : concatenatedRows) {
      for (double value : row) {
          std::cout << value << " ";
      }
      std::cout << std::endl;
      std::cout << std::endl;
  }

  // encrypt each row 
  std::vector<Ciphertext<DCRTPoly>> en_db; 
  for (const auto& row : concatenatedRows) {
    en_db.push_back(encryptFromVector(cc, pk, row));
  }
  cout << en_db.size() << endl;

  // encrypt query 
  vector<double> query(batchSize); 
  for (size_t i=0; i<batchSize; i += VECTOR_DIM) {
    copy(queryVector.begin(), queryVector.end(), query.begin() + i); 
  }

  Ciphertext<DCRTPoly> enc_query = encryptFromVector(cc, pk, query);

  // Compute the matrix-vector product of dbVectors times queryVector in the encrypted domain
  cout << "Beginning implementation..." << endl;

  // for every ciphertext in the vector en_db call matrixMultiply
  Ciphertext<DCRTPoly> result;
  result = matrixMultiply(cc, en_db, enc_query, pk);

  // decrypt 
  std::vector<double> decrypted_result = decryptToVector(cc, sk, result); 
  //for (int i=0; i<decrypted_result.size(), i++) {
  //  cout << decrypted_result[i] << endl; 
  //}

  cout << "Expected cosine similarity score between the query vector and the first DB vector: " << flush;
  cout << inner_product(queryVector.begin(), queryVector.end(), dbVectors[1].begin(), 0.0) << endl;

  return 0;
}
