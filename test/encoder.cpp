#include "gtest/gtest.h"
#include "smkhe/encoder.h"
#include "smkhe/encryptor.h"
#include "smkhe/mk_evaluator.h"
#include "smkhe/mk_keygen.h"
#include "smkhe/mk_decryptor.h"
#include <chrono>
#include <vector>
#include <iostream>

using namespace std;
using namespace smkhe;
using namespace std::chrono;

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

    // Generate SNP data once
    int inputSize2 = 1000;  
    vector<double> SNPs = generateSNPData(inputSize2);




TEST(Encoder, HomomorphicAdditionTest) {
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



    // Encode and encrypt SNP data
    Plaintext encodedSNPs = encoder.encode(SNPs);
    PublicKey pk = publicKey.getPublicKey();  
    Ciphertext encryptedSNPs = encryptor.encrypt(encodedSNPs, pk);
    MKCiphertext mkEncryptedSNPs(encryptedSNPs, 1, 0);


    evaluator.addPlainInPlace(mkEncryptedSNPs, encodedSNPs);  // Perform addition with plaintext


    // Decrypt and verify results
    PartialCiphertext partialDecryption = decryptor.partialDecryption(mkEncryptedSNPs, 1, secretKey);
    vector<PartialCiphertext> partialDecryptions = {partialDecryption};
    Plaintext decryptedSNPs = decryptor.mergeDecryptions(mkEncryptedSNPs, partialDecryptions);
    vector<complex<double>> decodedSNPs = encoder.decode(decryptedSNPs);

    // Verify results
    for (int i = 0; i < inputSize2; i++) {
        ASSERT_NEAR(SNPs[i] + SNPs[i], decodedSNPs[i].real(), 1e-6);
    }
}

TEST(Encoder, HomomorphicMultiplicationTest) {
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


    // Encode SNP data
    Plaintext encodedSNPs = encoder.encode(SNPs);

    // Encrypt the data
    PublicKey pk = publicKey.getPublicKey();
    Ciphertext encryptedSNPs = encryptor.encrypt(encodedSNPs, pk);

    // Perform homomorphic addition and multiplication
    MKCiphertext mkEncryptedSNPs(encryptedSNPs, 1, 0);


    // Decrypt the data to verify
    PartialCiphertext partialDecryption = decryptor.partialDecryption(mkEncryptedSNPs, 1, secretKey);
    vector<PartialCiphertext> partialDecryptions = {partialDecryption};
    Plaintext decryptedSNPs = decryptor.mergeDecryptions(mkEncryptedSNPs, partialDecryptions);

    // Decode the result
    vector<complex<double>> decodedSNPs = encoder.decode(decryptedSNPs);

    // Verify results
    for (int i = 0; i < inputSize2; i++) {
        ASSERT_NEAR(SNPs[i], decodedSNPs[i].real(), 1e-6);  // Adjust tolerance if needed
    }
}
