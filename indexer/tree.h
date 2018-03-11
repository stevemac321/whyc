#pragma once
#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

struct entry {
        char verse[32];
        char ref[32];
};
struct tnode {
        struct entry * data;
        struct tnode * left;
        struct tnode * right;
};
struct tree {
        struct tnode *root;
};
void tree_init(struct tree *);
void tree_insert(struct tree *, struct entry *);
void tree_delete(struct tree *); 
void tree_print(struct tree *);
int cmp(const char*, const char*);


