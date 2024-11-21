#include "../include/config.h"
#include "../include/openFHE_wrapper.h"
#include "openfhe.h"

using namespace lbcrypto;
using namespace std;

// Entry point of the application that orchestrates the flow

int main(int argc, char *argv[]) {

    // The only parameter you will need to modify is multiplicative depth
    // Which is located in ../include/config.h

    // ----- Don't touch anything in the section below -----
    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetSecurityLevel(HEStd_128_classic);
    parameters.SetMultiplicativeDepth(MULT_DEPTH);
    parameters.SetScalingModSize(45);

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);
    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);
    cc->Enable(ADVANCEDSHE);
    // ----- Don't touch anything in the section above -----

    // Key generation operations
    auto keyPair = cc->KeyGen();
    auto pk = keyPair.publicKey;
    auto sk = keyPair.secretKey;
    cc->EvalMultKeyGen(sk);
    cc->EvalRotateKeyGen(sk, {1, 2, 4, 8}); // Additional rotation keys must be added as needed

    // Example of encryption process
    vector<double> input = {-3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0};
    Plaintext ptxt = cc->MakeCKKSPackedPlaintext(input);
    Ciphertext<DCRTPoly> ctxt = cc->Encrypt(pk, ptxt);

    // Homomorphic operations upon ctxt would go here
    ctxt = OpenFHEWrapper::fhePolynomial(cc, ctxt);

    // Example of decryption process
    cc->Decrypt(sk, ctxt, &ptxt);
    vector<double> result = ptxt->GetRealPackedValue();
    for(size_t i = 0; i < input.size(); i++) {
        cout << result[i] << endl;
    }

    return 0;
}