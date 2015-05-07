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
void test_thread();

struct thread_params{
    Node* pq;  // Global PatternQueue
    char* fb;  // Buffer of test.txt
};
pthread_mutex_t pop_lock = PTHREAD_MUTEX_INITIALIZER;

patternChain* Pc;  // Array of out pattern_chain in patter.txt order

void thread_to_work(void*);

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
    int len_pq = pq_length(pq);

    // Initiallize of PatternChain
    Pc = malloc(len_pq*sizeof(patternChain));

    struct thread_params tp;
    tp.pq = pq;
    tp.fb = file_buf;

    for(int i = 0; i < num_of_thread; i++){
        pthread_create(&threads[i], NULL, (void*)&thread_to_work, &tp);
        // There is a bug while put pthread_join() inside this loop.
    }
    
    for(int i = 0; i < num_of_thread; i++){
        pthread_join(threads[i], NULL);
    }

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

void thread_to_work(void* par){
    struct thread_params* tp = par;
    struct pattern_chain* pc;
    pthread_t t = pthread_self();
    while(!pq_isEmpty(tp->pq)){
        // Pop a pattern and match
        pthread_mutex_lock(&pop_lock);
        Node* node = pq_pop(tp->pq);
        pthread_mutex_unlock(&pop_lock);
        const char* pattern = node->str;
        int id = node->id;        
        pc = mpMatch(tp->fb, pattern); 
        printf("Pattern:%s\tTid:%u\tNid:%d\n", pc->pattern, (unsigned int)t, id);

        // Put each pattern chain into Pc
        Pc[id] = *pc;
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
