#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "block.h"

// Function prototypes
void test_add_job();
void test_list_jobs();
void test_modify_job();
void test_delete_job();
void test_integrity_verification();

Block* blockchain; // Global variable to hold the blockchain for testing

int main() {
    blockchain = NULL; // Initialize the blockchain
    printf("Running tests for Blockchain Job Directory...\n");

    test_add_job();
    test_list_jobs();
    test_modify_job();
    test_delete_job();
    test_integrity_verification();

    printf("All tests completed.\n");
    return 0;
}

// Test adding jobs
void test_add_job() {
    Job job1 = {"J0001", "Software Engineer", "Tech Corp", "New York", "Develop and maintain software."};
    Job job2 = {"J0002", "Data Scientist", "Data Inc", "San Francisco", "Analyze and interpret complex data."};

    // Add job 1
    Block* block1 = create_block(job1, "0");
    add_block(&blockchain, block1);

    // Add job 2
    Block* block2 = create_block(job2, block1->hash);
    add_block(&blockchain, block2);

    printf("Added jobs successfully.\n");
}

// Test listing jobs
void test_list_jobs() {
    printf("\nTesting list jobs:\n");
    list_jobs(blockchain);
}

// Test modifying a job
void test_modify_job() {
    printf("\nTesting modify job:\n");
    if (modify_job(blockchain, "J0001")) {
        printf("Job modified successfully.\n");
    } else {
        printf("Job modification failed.\n");
    }
}

// Test deleting a job
void test_delete_job() {
    printf("\nTesting delete job:\n");
    if (delete_job(&blockchain, "J0002")) {
        printf("Job deleted successfully.\n");
    } else {
        printf("Job deletion failed.\n");
    }
}

// Test verifying the integrity of the blockchain
void test_integrity_verification() {
    printf("\nTesting integrity verification:\n");
    if (verify_integrity(blockchain)) {
        printf("Blockchain integrity verified. No tampering detected.\n");
    } else {
        printf("Blockchain integrity compromised!\n");
    }
}
