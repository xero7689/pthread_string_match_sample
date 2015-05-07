/*  Queue for pattern
 *  author: xero7689
 *  email: volleyp7689@gmail.com
 */
#ifndef _PATTERNQUEUE
#define _PATTERNQUEUE
typedef struct queue_node{
    int id;  // node id for output order
    const char* str;  // string of pattern
    struct queue_node* next;  // link of next pattern
}Node;

Node* patternQueue(const char* pattern_buf);
Node* pq_pop(Node* pq);
//int pq_push(Node* pq, const char* str);
int pq_isEmpty(const Node* pq);
int pq_length(const Node* pq);

#endif
