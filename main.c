/* Main.c
 * author: xero7689
 * email: volleyp7689@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

#include "patternQueue.h"
#include "strMatch.h"
#include "patternChain.h"

#define DEBUG

char* create_buf(FILE*);
int length(FILE*);

patternChain* Pc;  // Array of out pattern_chain in patter.txt order
double start_utime, finish_utime;
struct timeval tv, tv2;

int main(int argc, char* argv[]){
    int num_threads;
    num_threads = strtol(argv[1], NULL, 10);

    // Initialize Thread Pool
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

    // Create PatternQueue
    Node* pq = patternQueue(pattern_buf);
    int len_pq = pq_length(pq);

    // Initiallize of PatternChain
    Pc = malloc(len_pq*sizeof(patternChain));
    
    // Timing
    gettimeofday(&tv, NULL);
    start_utime = tv.tv_sec * 1000000 + tv.tv_usec;

    // Openmp
    struct pattern_chain* pc;
    Node* node;
    int nid;
    const char* pattern;
    int tid;
#pragma omp parallel shared(pq) private(node, nid, pc, pattern) num_threads(num_threads)
    while(!pq_isEmpty(pq)){
        // Pop a pattern and match
        node = pq_pop(pq);
        pattern = node->str;
        nid = node->id;        
        pc = mpMatch(file_buf, pattern);
        
        //Print
        tid = omp_get_thread_num();
        printf("TID:%d\tPattern:%s\tNid:%d\n", tid, pc->pattern, nid);

        // Put each pattern chain into Pc
        Pc[nid] = *pc;
    }

    // Timing - finish
    gettimeofday(&tv2, NULL);
    finish_utime = tv2.tv_sec * 1000000 + tv2.tv_usec;
    printf("Parallel Execution Time = %f(s)\n", (finish_utime - start_utime)/1000000);
    printf("Thread numbers:%d\n", num_threads);

    // Output
    for(int i = 0; i < len_pq; i++){
        fprintf(output, "%s: (%d)", Pc[i].pattern, pcLength(&Pc[i]));
        while(!isEmpty(&Pc[i]))
            fprintf(output, " %d", pop_index(&Pc[i]));
        fprintf(output, "\n");       
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
