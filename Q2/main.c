#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <stdbool.h>

#define MAX_TRANSACTIONS_PER_BLOCK 10
#define LEADING_ZEROS 4 // Difficulty level: number of leading zeros in hash

// Structure to represent a transaction
typedef struct {
    int id; // Unique identifier for the transaction
    char details[256]; // Description of the transaction
    char hash_signature[65]; // SHA-256 signature as a hex string
} Transaction;

// Structure to represent a block in the blockchain
typedef struct Block {
    int index; // Position of the block in the blockchain
    time_t creation_time; // Timestamp of when the block was created
    Transaction transactions[MAX_TRANSACTIONS_PER_BLOCK]; // Array of transactions
    int transaction_count; // Current number of transactions in the block
    char previous_block_hash[65]; // Hash of the previous block
    char block_hash[65]; // Hash of the current block
    int nonce; // Random value used for mining
    struct Block* next; // Pointer to the next block in the chain
} Block;

// Structure for the blockchain
typedef struct {
    Block* head; // Pointer to the first block in the chain
    Block* pending_block; // Temporary block for pending transactions
    bool initialized; // Initialization status
} Blockchain;

// Function prototypes
void generate_hash(Block* block, char* output_hash);
void mine_new_block(Block* block);
bool append_transaction(Block* block, int id, const char* details);
void initialize_blockchain(Blockchain* blockchain);
void insert_block(Blockchain* blockchain, Block* new_block);
void display_blockchain(const Blockchain* blockchain);
void clear_input_buffer();
int show_menu(bool initialized, bool has_pending);

// Generate SHA-256 hash for the block
void generate_hash(Block* block, char* output_hash) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%d%ld%d%s%d", 
             block->index, block->creation_time, block->transaction_count, 
             block->previous_block_hash, block->nonce);

    unsigned char hash_bytes[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)buffer, strlen(buffer), hash_bytes);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&output_hash[i * 2], "%02x", hash_bytes[i]);
    }
}

// Mine a block by finding a nonce that results in a hash with specified leading zeros
void mine_new_block(Block* block) {
    char hash_output[65];
    do {
        block->nonce++;
        generate_hash(block, hash_output);
    } while (strncmp(hash_output, "0000", LEADING_ZEROS) != 0);

    strcpy(block->block_hash, hash_output);
}

// Add a transaction to a block
bool append_transaction(Block* block, int id, const char* details) {
    if (block->transaction_count >= MAX_TRANSACTIONS_PER_BLOCK) {
        return false; // Block is full
    }

    Transaction* transaction = &block->transactions[block->transaction_count++];
    transaction->id = id;
    strncpy(transaction->details, details, sizeof(transaction->details) - 1);
    
    // Generate a simple signature
    char signature_input[512];
    snprintf(signature_input, sizeof(signature_input), "%d%s", id, details);
    unsigned char signature_bytes[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)signature_input, strlen(signature_input), signature_bytes);
    
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&transaction->hash_signature[i * 2], "%02x", signature_bytes[i]);
    }

    return true;
}

// Initialize a new blockchain
void initialize_blockchain(Blockchain* blockchain) {
    blockchain->head = NULL;
    blockchain->initialized = true;

    // Allocate memory for the pending block
    blockchain->pending_block = (Block*)malloc(sizeof(Block));
    if (!blockchain->pending_block) {
        fprintf(stderr, "Error: Memory allocation failed for pending block.\n");
        exit(1);
    }
    blockchain->pending_block->index = 0;
    blockchain->pending_block->transaction_count = 0;
    strcpy(blockchain->pending_block->previous_block_hash, "0"); // Genesis block
}

// Insert a new block into the blockchain
void insert_block(Blockchain* blockchain, Block* new_block) {
    new_block->creation_time = time(NULL);
    new_block->next = blockchain->head;
    
    if (blockchain->head != NULL) {
        strcpy(new_block->previous_block_hash, blockchain->head->block_hash);
        new_block->index = blockchain->head->index + 1;
    }

    mine_new_block(new_block);
    blockchain->head = new_block;
}

// Display the entire blockchain
void display_blockchain(const Blockchain* blockchain) {
    if (!blockchain->initialized) {
        printf("Blockchain not initialized yet.\n");
        return;
    }

    if (blockchain->head == NULL) {
        printf("Blockchain is empty. No blocks mined yet.\n");
        return;
    }

    Block* current = blockchain->head;
    while (current != NULL) {
        printf("Block %d\n", current->index);
        printf("Creation Time: %ld\n", current->creation_time);
        printf("Transactions:\n");
        for (int i = 0; i < current->transaction_count; i++) {
            printf("  Item ID: %d, Details: %s\n", 
                   current->transactions[i].id, 
                   current->transactions[i].details);
        }
        printf("Previous Block Hash: %s\n", current->previous_block_hash);
        printf("Block Hash: %s\n", current->block_hash);
        printf("Nonce: %d\n\n", current->nonce);
        current = current->next;
    }
}

// Clear the input buffer to avoid issues with fgets
void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }
}

// Display menu options and get user choice
int show_menu(bool initialized, bool has_pending) {
    int option;
    printf("\n--- Supply Chain Blockchain Management ---\n");
    printf("1. Initialize blockchain\n");
    if (initialized) {
        printf("2. Add transaction\n");
        if (has_pending) {
            printf("3. Mine new block\n");
        }
        printf("4. Show blockchain\n");
    }
    printf("5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &option);
    clear_input_buffer();
    return option;
}

// Main function to run the menu-driven command-line interface
int main() {
    Blockchain blockchain = {NULL, NULL, false};
    int user_choice;
    
    while (true) {
        user_choice = show_menu(blockchain.initialized, 
                                 blockchain.pending_block && blockchain.pending_block->transaction_count > 0);
        
        switch (user_choice) {
            case 1:
                if (blockchain.initialized) {
                    printf("Blockchain has already been initialized.\n");
                } else {
                    initialize_blockchain(&blockchain);
                    printf("Blockchain successfully initialized.\n");
                }
                break;
            
            case 2:
                if (!blockchain.initialized) {
                    printf("Please initialize the blockchain first.\n");
                    break;
                }
                int item_id;
                char details[256];
                printf("Enter item ID: ");
                scanf("%d", &item_id);
                clear_input_buffer();
                printf("Enter item description: ");
                fgets(details, sizeof(details), stdin);
                details[strcspn(details, "\n")] = 0; // Remove trailing newline
                
                if (append_transaction(blockchain.pending_block, item_id, details)) {
                    printf("Transaction successfully added to pending block.\n");
                } else {
                    printf("Unable to add transaction. The pending block may be full.\n");
                }
                break;
            
            case 3:
                if (!blockchain.initialized) {
                    printf("Please initialize the blockchain first.\n");
                    break;
                }
                if (!blockchain.pending_block || blockchain.pending_block->transaction_count == 0) {
                    printf("No pending transactions available for mining.\n");
                    break;
                }
                insert_block(&blockchain, blockchain.pending_block);
                printf("New block mined and added to the blockchain.\n");
                
                // Allocate a new pending block
                blockchain.pending_block = (Block*)malloc(sizeof(Block));
                if (!blockchain.pending_block) {
                    fprintf(stderr, "Error: Unable to allocate memory for new pending block.\n");
                    exit(1);
                }
                blockchain.pending_block->transaction_count = 0;
                break;
            
            case 4:
                if (!blockchain.initialized) {
                    printf("Please initialize the blockchain first.\n");
                    break;
                }
                display_blockchain(&blockchain);
                break;
            
            case 5:
                printf("Exiting the program. Goodbye!\n");
                exit(0);
            
            default:
                printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}
