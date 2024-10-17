#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "block.h"

// Function to display the menu and get user input
void print_menu() {
    printf("\nBlockchain Job Directory Menu:\n");
    printf("1. Add Job Listing\n");
    printf("2. List Jobs\n");
    printf("3. Search Jobs\n");
    printf("4. Modify Job\n");
    printf("5. Delete Job\n");
    printf("6. Verify Integrity\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

// Function to get job details from the user
void get_job_details(Job *job) {
    printf("Enter Job ID (e.g., J0001): ");
    scanf(" %5s", job->id); // Read job ID
    printf("Enter Job Title: ");
    scanf(" %[^\n]%*c", job->title); // Reads input with spaces
    printf("Enter Company: ");
    scanf(" %[^\n]%*c", job->company);
    printf("Enter Location: ");
    scanf(" %[^\n]%*c", job->location);
    printf("Enter Job Description: ");
    scanf(" %[^\n]%*c", job->description);
}

int main() {
    Block* blockchain = NULL;
    Block* last_block = NULL;
    int choice;
    Job job;
    int integrity_verified;

    printf("Welcome to the Blockchain Job Directory Application!\n");

    do {
        print_menu();
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
            case 1:
                // Add job listing
                get_job_details(&job);

                // Create and add a new block
                if (blockchain == NULL) {
                    last_block = create_block(job, "0");
                    blockchain = last_block;
                } else {
                    Block* new_block = create_block(job, last_block->hash);
                    add_block(&blockchain, new_block);
                    last_block = new_block; // Update the last block reference
                }
                printf("Job listing added successfully!\n");
                break;

            case 2:
                // List all jobs
                printf("Listing all job listings:\n");
                list_jobs(blockchain);
                break;

            case 3:
                // Search for jobs by keyword
                printf("Enter keyword to search for jobs: ");
                scanf(" %[^\n]%*c", job.title);
                printf("Searching for jobs with keyword '%s':\n", job.title);
                search_job(blockchain, job.title);
                break;

            case 4:
                // Modify a job
                printf("Enter Job ID to modify: ");
                scanf(" %5s", job.id);
                if (modify_job(blockchain, job.id)) {
                    printf("Job modified successfully!\n");
                } else {
                    printf("Job not found.\n");
                }
                break;

            case 5:
                // Delete a job
                printf("Enter Job ID to delete: ");
                scanf(" %5s", job.id);
                if (delete_job(&blockchain, job.id)) {
                    printf("Job deleted successfully!\n");
                } else {
                    printf("Job not found.\n");
                }
                break;

            case 6:
                // Verify blockchain integrity
                integrity_verified = verify_integrity(blockchain);
                if (integrity_verified) {
                    printf("Blockchain integrity verified. No tampering detected.\n");
                } else {
                    printf("Blockchain integrity compromised!\n");
                }
                break;

            case 7:
                // Exit the program
                printf("Exiting the program. Goodbye!\n");
                break;

            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }

    } while (choice != 7);

    return 0;
}
