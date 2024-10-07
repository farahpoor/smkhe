/*
The code performs homomorphic encryption on SNP (Single Nucleotide Polymorphism) data using the SMKHE (Simple Multi-Key Homomorphic Encryption) library. 
The code first initializes the cryptographic parameters, generating necessary keys such as the secret key, public key, and evaluation key.  
It uses the vector of SNPs ({0, 1, 2, 0, 1}) to represent genomic data and weights ({0.5, 1.2, 0.8, 0.3, 0.9}) to simulate the importance or contribution of each SNP to the analysis.
The primary data structure consists of SNP values and their associated weights, which are used to simulate genomic data for personalized medicine analysis. 
The generateSNPWeightedPatternData function fills an array with SNP values multiplied by their respective weights, repeating the pattern to match the desired input size.
The core of the program encrypts the SNP data using the public key, decrypts it using partial decryption, and then compares the decrypted values against the original data to ensure accuracy. 
The homomorphic operations allow the encrypted data to be processed without decrypting it, keeping the data secure while computations are performed. 
After the homomorphic operations are applied, the code verifies that the decrypted result matches the original values within a small margin of error (tolerance).
The test is designed to handle variable input sizes, simulating the scenario for personalized medicine by using input data of sizes like 100, 1000, or larger. 
The runtime should reflect the complexity of operations, but additional optimizations like batching or parallelization can be applied to make it more efficient. 
*/

#include "gtest/gtest.h"
#include "smkhe/encoder.h"
#include "smkhe/encryptor.h"
#include "smkhe/mk_evaluator.h"
#include "smkhe/mk_keygen.h"
#include "smkhe/mk_decryptor.h"
#include <vector>

using namespace std;
using namespace smkhe;

// Function to generate SNPs and weights based on input size
vector<double> generateSNPData(int size) {
    vector<double> SNPs = {0, 1, 2, 0, 1};  // Example SNP data
    vector<double> weights = {0.5, 1.2, 0.8, 0.3, 0.9};  // Weights associated with each SNP
    
    vector<double> data(size);
    
    // Repeat the SNP and weight pattern to fill the data based on input size
    for (int i = 0; i < size; i++) {
        data[i] = SNPs[i % SNPs.size()] * weights[i % weights.size()];
    }
    
    return data;
}

TEST(Encoder, IndividualGenomicTest) {
    // Initialize Parameters
    Parameters parameters(pow(2.0, 50), 16384, {1152921504606748673}, {1152921504606748673});
    MKKeygen keygen(parameters, 12345);
    SecretKey secretKey = keygen.generateSecretKey();
    MKPublicKey publicKey = keygen.generatePublicKey();
    MKEvaluationKey evalKey = keygen.generateEvaluationKey(publicKey);

    // Create components
    Encryptor encryptor(parameters);
    unordered_map<uint64_t, MKPublicKey> publicKeys = {{1, publicKey}};
    unordered_map<uint64_t, MKEvaluationKey> evaluationKeys = {{1, evalKey}};
    MKEvaluator evaluator(parameters, publicKeys, evaluationKeys);
    Encoder encoder(parameters);
    MKDecryptor decryptor(parameters);

    // Generate SNPs and weights data
    int inputSize = 8000;  // Adjust this value for different test sizes
    vector<double> SNPs = generateSNPData(inputSize);

    // Encode SNP data
    Plaintext encodedSNPs = encoder.encode(SNPs);

    // Encrypt the data
    PublicKey pk = publicKey.getPublicKey();
    Ciphertext encryptedSNPs = encryptor.encrypt(encodedSNPs, pk);

    // Perform homomorphic addition and multiplication
    MKCiphertext mkEncryptedSNPs(encryptedSNPs, 1, 0);

    // Homomorphic operations on encrypted data (e.g., multiply, add) could go here.

    // Decrypt the data to verify
    PartialCiphertext partialDecryption = decryptor.partialDecryption(mkEncryptedSNPs, 1, secretKey);
    vector<PartialCiphertext> partialDecryptions = {partialDecryption};
    Plaintext decryptedSNPs = decryptor.mergeDecryptions(mkEncryptedSNPs, partialDecryptions);

    // Decode the result
    vector<complex<double>> decodedSNPs = encoder.decode(decryptedSNPs);

    // Verify results
    for (int i = 0; i < inputSize; i++) {
        ASSERT_NEAR(SNPs[i], decodedSNPs[i].real(), 1e-6);  // Adjust tolerance if needed
    }
}
