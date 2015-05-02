/* Main.c
 * author: xero7689
 * email: volleyp7689@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "patternQueue.h"
#include "strMatch.h"
#include "output.h"

#define DEBUG

char* create_buf(FILE*);
int length(FILE*);

int main(int argc, char* argv[]){
    if(argc > 2){
        printf("Too many argumnet to supply.\n");
        return 1;
    }
    
    // Initialize Thread Pool
    int num_of_thread = argv[1];
    pthread_t thread_array[num_of_thread]; 

    FILE* file_ptr;
    FILE* pattern_ptr;
    FILE* output;
    
    char* file_buf;
    char* pattern_buf;

    // Open file.
    file_ptr = fopen("test.txt", "r");
    pattern_ptr = fopen("term.txt", "r");
    output = fopen("output.txt", "w+");

    // Check if file doesn't exist.
    if(file_ptr == NULL || pattern_ptr == NULL){
        perror("Error while openning the file.");
        exit(EXIT_FAILURE);
    }

    // Create Buffer for test.txt and term.txt.
    file_buf = create_buf(file_ptr);
    pattern_buf = create_buf(pattern_ptr);

    // buf_length
    //printf("file_buf length:%lu\n", strlen(file_buf));
    //printf("pattern_buf length:%lu\n", strlen(pattern_buf));
    
    // Testing
    Node* patterns = patternQueue(pattern_buf);
    Node* next_pattern = patterns;
    struct pattern_chain* pc;
    
    // Match Every Pattern
    while(next_pattern->next != NULL){
        pc = mpMatch(file_buf, next_pattern->str);
        fprintf(output, "%s: (%d)", next_pattern->str, pcLength(pc));
        while(!isEmpty(pc)){
            fprintf(output, " %d", pop_index(pc));
        }
        fprintf(output, "\n");
        next_pattern = next_pattern->next;
    }
    
    // File Close
    fclose(file_ptr);
    fclose(pattern_ptr);
    fclose(output);

    return 0;
}

int length(FILE* fp){
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

char* create_buf(FILE* src){
    char* src_buf = malloc(length(src)*sizeof(char)); 
    char ch;
    int i = 0;
    while((ch = fgetc(src)) != EOF)
        src_buf[i++] = ch;
    return src_buf;
}
