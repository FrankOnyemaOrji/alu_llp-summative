#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "block.h"

// Function to create a new block
Block* create_block(Job job, const char* prev_hash) {
    Block* new_block = (Block*)malloc(sizeof(Block));
    new_block->index = 0; // Will update later when adding to the chain
    new_block->timestamp = time(NULL);
    new_block->job = job;
    strncpy(new_block->previous_hash, prev_hash, HASH_LENGTH);
    strncpy(new_block->hash, calculate_hash(new_block), HASH_LENGTH);
    new_block->next = NULL;
    return new_block;
}

// Function to calculate the hash of a block using SHA-256
char* calculate_hash(const Block* block) {
    static char hash[HASH_LENGTH];
    char data[TITLE_LENGTH + COMPANY_LENGTH + LOCATION_LENGTH + DESCRIPTION_LENGTH + HASH_LENGTH];

    snprintf(data, sizeof(data), "%s%s%s%s%s", 
        block->job.title,         // Access job.title
        block->job.company,       // Access job.company
        block->job.location,      // Access job.location
        block->job.description,   // Access job.description
        block->previous_hash);        // Access the previous hash

    unsigned char temp_hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)data, strlen(data), temp_hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash + (i * 2), "%02x", temp_hash[i]);
    }
    return hash;
}


// Function to add a block to the blockchain
void add_block(Block** blockchain, Block* new_block) {
    Block* current = *blockchain;
    if (current == NULL) {
        *blockchain = new_block;
    } else {
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_block;
    }
}

// Function to verify the integrity of the blockchain
int verify_integrity(const Block* blockchain) {
    // Check if blockchain is empty
    if (blockchain == NULL) {
        printf("Blockchain is empty! No integrity check required.\n");
        return 1; // Consider it intact since there are no blocks
    }

    const Block* current = blockchain;
    while (current != NULL && current->next != NULL) {
        if (strcmp(current->hash, current->next->previous_hash) != 0) {
            return 0; // Integrity check failed
        }
        current = current->next;
    }
    return 1; // Blockchain is intact
}

// Function to list all job listings
void list_jobs(const Block* blockchain) {
    const Block* current = blockchain;
    if (current == NULL) {
        printf("No job listings available.\n");
        return;
    }

    while (current != NULL) {
        printf("Job ID: %s\n", current->job.id);
        printf("Title: %s\n", current->job.title);
        printf("Company: %s\n", current->job.company);
        printf("Location: %s\n", current->job.location);
        printf("Description: %s\n", current->job.description);
        printf("Timestamp: %s", ctime(&current->timestamp));
        printf("Hash: %s\n", current->hash);
        printf("Previous Hash: %s\n", current->previous_hash);
        printf("--------------------\n");
        current = current->next;
    }
}

// Function to search for jobs by keyword
int search_job(Block* blockchain, const char* keyword) {
    int found = 0;
    Block* current = blockchain;
    while (current != NULL) {
        if (strstr(current->job.title, keyword) || strstr(current->job.description, keyword)) {
            printf("Job ID: %s\n", current->job.id);
            printf("Title: %s\n", current->job.title);
            printf("Company: %s\n", current->job.company);
            printf("Location: %s\n", current->job.location);
            printf("Description: %s\n", current->job.description);
            printf("Timestamp: %s", ctime(&current->timestamp));
            printf("Hash: %s\n", current->hash);
            printf("Previous Hash: %s\n", current->previous_hash);
            printf("--------------------\n");
            found = 1;
        }
        current = current->next;
    }
    if (!found) {
        printf("No jobs found for keyword '%s'.\n", keyword);
    }
    return found;
}

// Function to modify a job
int modify_job(Block* blockchain, const char* job_id) {
    Block* current = blockchain;
    while (current != NULL) {
        if (strcmp(current->job.id, job_id) == 0) {
            printf("Modifying job: %s\n", current->job.title);
            get_job_details(&current->job);
            strncpy(current->hash, calculate_hash(current), HASH_LENGTH); // Recalculate the hash
            return 1; // Modification successful
        }
        current = current->next;
    }
    return 0; // Job not found
}

// Function to delete a job
int delete_job(Block** blockchain, const char* job_id) {
    Block* current = *blockchain;
    Block* previous = NULL;

    while (current != NULL) {
        if (strcmp(current->job.id, job_id) == 0) {
            if (previous == NULL) { // First block
                *blockchain = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            return 1; // Deletion successful
        }
        previous = current;
        current = current->next;
    }
    return 0; // Job not found
}
