#ifndef CAT_H_
#define CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__APPLE__)
#define OS 1
#elif defined(__linux__)
#define OS 2
#endif


typedef struct {
    int e;
    int E;
    int v;
    int n;
    int t;
    int T;
    int s;
    int b;
} Flags;

typedef struct {
    int e;
    int i;
    int v;
    int c;
    int n;
    int l;
} Flags_grep;

#endif  // CAT_H_