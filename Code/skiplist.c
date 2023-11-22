#include <stdlib.h>
#include <assert.h>

#include "skiplist.h"
#include "rng.h"

typedef struct s_LinkedElement LinkedElement;

typedef struct s_LinkedElement {
    int value;
    int level;
    LinkedElement **prev;
    LinkedElement **next;
} LinkedElement;

struct s_SkipList {
    int nblevels;
    RNG rng;
    int size;
    LinkedElement *sentinelle;
};

SkipList skiplist_create(int nblevels) {
    //create list
    SkipList skipList = malloc(sizeof(SkipList));
    LinkedElement *sentinnelle = malloc(sizeof(LinkedElement));
    //initialize values
    skipList->sentinelle = sentinnelle;
    skipList->rng = rng_initialize(0);
    skipList->size = 0;
    skipList->nblevels = nblevels;
    skipList->sentinelle->next = malloc(sizeof(LinkedElement)*nblevels);
    skipList->sentinelle->prev = malloc(sizeof(LinkedElement)*nblevels);
    //set all pointers on sentinelle
    for (int i = 0; i < nblevels; i++) {
        skipList->sentinelle->next[i] = skipList->sentinelle;
        skipList->sentinelle->prev[i] = skipList->sentinelle;
    }
    return skipList;
}

void skiplist_delete(SkipList d) {
    while (d->size) {
        d->sentinelle->next[0] = d->sentinelle->next[0]->next[0];
        free(d->sentinelle->next[0]->prev[0]);
        (d->size)--;
    }
    free(d->sentinelle);
    free(d);
}

unsigned int skiplist_size(SkipList d) {
    return d->size;
}

int skiplist_at(SkipList d, unsigned int i) {
    LinkedElement *e = d->sentinelle;
    while (i--) {
        e = e->next[0];
    }
    return e->value;
}

void skiplist_map(SkipList d, ScanOperator f, void *user_data) {
    for (LinkedElement *e = d->sentinelle->next[0]; e != d->sentinelle; e = e->next[0]) {
        f(e->value, user_data);
    }
}

SkipList skiplist_insert(SkipList d, int value) {

    LinkedElement *e = d->sentinelle;
    LinkedElement *newElem = malloc(sizeof(LinkedElement));
    newElem->value = value;
    newElem->level = rng_get_value(&d->rng,d->nblevels);
    int i;
    debutDeBoucle:
    i = d->nblevels;
    for (int i = e->level; i; i--) {
        if (e->next[i]->value < value) {
            e = e->next[i];
            goto debutDeBoucle;
        } else if (e->next[i]->value > value && e->value < value) {
            goto finDeBoucle;
        } else continue;
    }
    finDeBoucle:
    newElem->prev[0] = e;
    newElem->next[0] = e->next[i];
    for(int j = 1;j<newElem->level;j++){
        if(newElems)
    }
}
