// lab1.cpp: определяет точку входа для приложения.
//
#include <stdio.h>
#include "lab1.h"

using namespace std;


int ParseArray(const char s[], char t, char before[], char after[]) {
    int i = 0, j = 0;
    int found = 0; 

    if (s == NULL || before == NULL || after == NULL) {
        return -1; 
    }

    while (s[i] != '\0') {
        if (s[i] == t) {
            found = 1; 
            i++; 
            break;
        }
        before[i] = s[i];
        i++;
    }
    before[i - 1] = '\0'; 

    if (!found) {
        return -2;
    }

    while (s[i] != '\0') {
        after[j++] = s[i++];
    }
    after[j] = '\0'; 
    return 0;
}

int ParsePointer(const char* s, char t, char* before, char* after) {
    const char* s_ptr = s;
    char* before_ptr = before;
    char* after_ptr = after;
    int found = 0;

    if (s == NULL || before == NULL || after == NULL) {
        return -1; 
    }

    while (*s_ptr != '\0') {
        if (*s_ptr == t) {
            found = 1;
            s_ptr++; 
            break;
        }
        *before_ptr++ = *s_ptr++;
    }
    *before_ptr = '\0'; 

    if (!found) {
        return -2;
    }

    while (*s_ptr != '\0') {
        *after_ptr++ = *s_ptr++;
    }
    *after_ptr = '\0'; 
    return 0;
}


int main() {
    char s[] = "hello@world";
    char before[100], after[100];

    if (ParseArray(s, '@', before, after) == 0) {
        printf("Before: %s, After: %s\n", before, after);
    }
    else {
        printf("Error in ParseArray\n");
    }

    if (ParsePointer(s, '@', before, after) == 0) {
        printf("Before: %s, After: %s\n", before, after);
    }
    else {
        printf("Error in ParsePointer\n");
    }

    return 0;
}

