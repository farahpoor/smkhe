
# SMKHE - Secure Multi-Key Homomorphic Encryption

This repository contains the extended and customized version of the [SMKHE project](https://github.com/andru47/smkhe), which implements multi-key homomorphic encryption. This version has been customized for use in scenarios like personalized medicine and SNP data encryption and decryption with patterned data input.

## Features

- **Homomorphic Encryption**: Supports encoding, encryption, and decryption using multi-key homomorphic encryption (MKHE).
- **Dynamic Input Size**: The project has been extended to allow for dynamic input size using patterned data or random data, allowing testing at various scales.
- **SNP Data Simulation**: Implements custom pattern-based data input for SNPs and weights in healthcare applications.

## Requirements

To use this project, you will need:

- **CMake**: For building the project.
- **GTest**: For running the tests.
- **A C++17 compatible compiler**: For compiling the code.
- **Git**: For cloning and managing the repository.

## Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/your-username/smkhe.git
   ```

2. Navigate to the project directory:

   ```bash
   cd smkhe
   ```

3. Build the project:

   ```bash
   cmake -B build
   cmake --build build
   ```

4. Run the tests:

   ```bash
   ./build/tests
   ```

## Usage

This project includes a test suite based on `Google Test` for validating the encryption, decryption, and evaluation logic.

### Example Test: Dynamic Input Size

This test encrypts and decrypts a dataset, ensuring the result matches the original input:

```cpp
TEST(Encoder, DynamicInputSizeTest) {
    // Initialize Parameters
    Parameters parameters(pow(2.0, 50), 16384, {1152921504606748673}, {1152921504606748673});
    MKKeygen keygen(parameters, 12345);
    SecretKey secretKey = keygen.generateSecretKey();
    MKPublicKey publicKey = keygen.generatePublicKey();
    MKEvaluationKey evalKey = keygen.generateEvaluationKey(publicKey);

    // Generate dynamic input size data with the pattern
    int inputSize = 1000;  // Adjust this value for different input sizes
    vector<double> data = generatePatternedData(inputSize);

    // Encoding, encrypting, and decrypting logic follows...
}
```

You can adjust the input size to scale the encryption and test larger datasets.

## License

This project follows the [MIT License](LICENSE).
