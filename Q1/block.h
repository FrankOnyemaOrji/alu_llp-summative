#ifndef BLOCK_H
#define BLOCK_H

#include <time.h>

#define HASH_LENGTH 65
#define DESCRIPTION_LENGTH 500
#define TITLE_LENGTH 100
#define COMPANY_LENGTH 100
#define LOCATION_LENGTH 100
#define JOB_ID_LENGTH 6
#define HASH_SIZE 64 // SHA-256 produces a 64-character hash

typedef struct {
    char id[JOB_ID_LENGTH];         // Unique identifier for the job
    char title[TITLE_LENGTH];       // Job title
    char company[COMPANY_LENGTH];   // Company offering the job
    char location[LOCATION_LENGTH];  // Job location
    char description[DESCRIPTION_LENGTH]; // Job description
} Job;

typedef struct Block {
    int index;                      // Position of the block in the chain
    time_t timestamp;               // Time when the block was created
    Job job;                        // Job data stored in this block
    char previous_hash[HASH_LENGTH]; // Hash of the previous block
    char hash[HASH_LENGTH];         // Hash of this block
    struct Block* next;             // Pointer to the next block
} Block;

// Function prototypes
Block* create_block(Job job, const char* prev_hash);
void add_block(Block** blockchain, Block* new_block);
char* calculate_hash(const Block* block);
int verify_integrity(const Block* blockchain);
int search_job(Block *blockchain, const char *keyword);
void print_menu();
void list_jobs(const Block* blockchain);
int modify_job(Block* blockchain, const char* job_id);
int delete_job(Block** blockchain, const char* job_id);
void get_job_details(Job *job);

#endif
