# Blockchain Job Directory Application

## Overview

The Blockchain Job Directory Application is a decentralized platform for posting and searching job listings. Utilizing blockchain principles, this application ensures that job postings are secure, transparent, and immutable. Each job listing is stored as a block in a linked list structure, allowing for easy verification and integrity checks.

## Compilation and Running Instructions

To compile and run the program, follow these steps:

1. **Clone the repository** (if applicable):
    ```bash
    git clone https://github.com/FrankOnyemaOrji/alu_llp-summative.git
    cd [repository_name]
    ```

2. **Install OpenSSL**:
   Ensure that you have OpenSSL installed on your machine. You can install it via:
   - For Ubuntu:
     ```bash
     sudo apt-get install libssl-dev
     ```
   - For macOS:
     ```bash
     brew install openssl
     ```

3. **Compile the program**:
   Run the following command to compile the program:
   ```bash
   gcc -o job_directory job_directory.c job_block.c -lssl -lcrypto
   ./job_directory

4. **Test the program**:
   Run the following command to compile the program:
   ```bash
   gcc -o test_job_directory test_job_directory.c job_block.c -lssl -lcrypto
   ./test_job_directory

5. **To check for memory leaks using Valgrind**:
   Run the following command to compile the program:
   ```bash
   valgrind --leak-check=full ./test_job_directory

6. **To check for memory leaks using AddressSanitizer**:
   Run the following command to compile the program:
   ```bash
   gcc -o test_job_directory test_job_directory.c job_block.c -lssl -lcrypto -fsanitize=address
