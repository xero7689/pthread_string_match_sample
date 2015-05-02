/* Different String Match Algorithm
 * author: xero7689
 * email: volleyp7689@gmail.com
 */

#ifndef STRMATCH_H
#define STRMATCH_H
#include "output.h"

//void failure(const char* pattern);
//struct pattern_chain* mfMatch(char* text, char* pattern);
struct pattern_chain* mpMatch(const char* text, const char* pattern);
//void kmpMatch(const char* text, const char* pattern);

#endif
