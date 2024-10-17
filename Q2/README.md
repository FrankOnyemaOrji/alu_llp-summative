# Supply Chain Blockchain System

## Overview
This project is a straightforward implementation of a blockchain-based supply chain tracking system using C. It showcases fundamental blockchain concepts, including blocks, transactions, mining, and cryptographic hashing, adhering to specific rules for blockchain operations.

## Features
- **Blockchain Crypto:** Uses SHA-256 hashing to ensure block integrity.
- **Blockchain Data Structures:** Includes Block and Blockchain structures, with a pending block for transactions.
- **Transactions:** Structured with item ID, description, and a basic digital signature.
- **Block Mining:** Utilizes a proof-of-work algorithm with defined rules for adding blocks.
- **CLI:** Features a menu-driven command-line interface for blockchain interaction.

## Rules and Workflow
1. **Initialize the blockchain** before any other operations.
2. **Add transactions** to a pending block rather than directly to the blockchain.
3. Ensure there is at least **one transaction in the pending block** before mining.
4. **Add blocks to the blockchain** exclusively through the mining process.

## Dependencies
This project depends on the OpenSSL library for cryptographic functions. Ensure OpenSSL is installed on your system.

## Compilation
To compile the program, use the following command:

```sh
gcc -o supply_chain_blockchain main.c -lssl -lcrypto
```

This will create an executable named `supply_chain_blockchain`.

## Running the Program
To run the program, execute:

```sh
./supply_chain_blockchain
```

## Using the Menu-Driven CLI
The program offers a menu-driven command-line interface with the following options:

1. **Create new blockchain:** Initialize a new blockchain (only if not already initialized).
2. **Add transaction:** Add a new transaction to the pending block (only after blockchain initialization).
3. **Mine block:** Mine the pending block and add it to the blockchain (only when there are pending transactions).
4. **Print blockchain:** Display the entire blockchain (only after blockchain initialization).
5. **Exit:** Exit the program.

Enter the number corresponding to your desired action when prompted. Available options will change based on the blockchain’s current state.

## Blockchain Implementation
This system implements a basic blockchain with the following characteristics:
- Each block includes transactions, a timestamp, the previous block's hash, and a nonce.
- Transactions encompass an item ID, description, and a basic digital signature.
- SHA-256 hashing ensures block integrity.
- A proof-of-work algorithm is employed for mining new blocks, requiring a certain number of leading zeros in the block hash.
- A pending block holds transactions until they are mined into a new block.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
