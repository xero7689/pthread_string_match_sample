/* Main.c
 * author: xero7689
 * email: volleyp7689@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "patternQueue.h"
#include "strMatch.h"
#include "output.h"

#define DEBUG

char* create_buf(FILE*);
int length(FILE*);
void test_thread();  // function for testing.

struct thread_params{
    Node* pq;
    char* fb;
    FILE* op;
};
pthread_mutex_t pop_lock = PTHREAD_MUTEX_INITIALIZER;
void thread_to_work(void*);

clock_t tstart, tfinish;

int main(int argc, char* argv[]){
    if(argc > 2){
        printf("Too many argumnet to supply.\n");
        return 1;
    }

    // Initialize Thread Pool
    int num_of_thread = strtol(argv[1], NULL, 10);
    pthread_t threads[num_of_thread];

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

    struct thread_params tp;
    tp.pq = pq;
    tp.fb = file_buf;
    tp.op = output;
    
    tstart = clock();

    for(int i = 0; i < num_of_thread; i++){
        pthread_create(&threads[i], NULL, (void*)&thread_to_work, &tp);
        // There is a bug while put pthread_join() inside this loop.
    }
    
    for(int i = 0; i < num_of_thread; i++){
        pthread_join(threads[i], NULL);
    }
   
    // File Close
    fclose(file_ptr);
    fclose(pattern_ptr);
    fclose(output);
    
    tfinish = clock();
    float time_spent = (float)(tstart - tfinish)/CLOCKS_PER_SEC ;
    printf("Timing:%f\n", time_spent);

    return 0;
}

void test_thread(){
    pthread_t t = pthread_self();
    printf("%u\n", (unsigned int) t);
}

void thread_to_work(void* par){
    struct thread_params* tp = par;
    struct pattern_chain* pc;
    pthread_t t = pthread_self();
    while(!pq_isEmpty(tp->pq)){
        pthread_mutex_lock(&pop_lock);
        char* pattern = pq_pop(tp->pq);
        pthread_mutex_unlock(&pop_lock);
        pc = mpMatch(tp->fb, pattern); 
#ifdef DEBUG
        printf("Pattern:%s\tTid:%u\n", pc->pattern, (unsigned int)t);
#endif
        fprintf(tp->op, "%s: (%d)", pc->pattern, pcLength(pc));
        while(!isEmpty(pc)){
            fprintf(tp->op, " %d", pop_index(pc));
        }
        fprintf(tp->op, "\n");
    }
    printf("Tid:%u finish!\n", (unsigned int)t);
    pthread_exit(0);
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
