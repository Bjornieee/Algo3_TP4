#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

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
    skipList->sentinelle->level = nblevels;
    skipList->sentinelle->value = -1;
    skipList->rng = rng_initialize(0);
    skipList->size = 0;
    skipList->nblevels = nblevels;
    skipList->sentinelle->next = malloc(sizeof(LinkedElement) * nblevels);
    skipList->sentinelle->prev = malloc(sizeof(LinkedElement) * nblevels);
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

LinkedElement *skiplist_find(SkipList d, int value) {
    LinkedElement *e = d->sentinelle;
    if(e==NULL){
        printf("valeur : %d\n",value);
    }
    bool isGood = false;

    if (d->size == 0) {
        return d->sentinelle;
    }

    int i = d->nblevels;
    while (!isGood) {
        if(e==NULL) printf("%d\n", skiplist_size(d));
        if (i == 0 && e->next[i]!=NULL && e->next[i] == d->sentinelle) return e;
        else {
            if (e->next[i]!=NULL && e->next[i]->value > value) isGood = true;
            else if (e->next[i]!=NULL && e->next[i]->value == value) return NULL;
            else if (e->next[i]!=NULL && e->next[i]->value < value) {
                if (e->next[i]!=NULL && e->next[i] == d->sentinelle) {
                    i--;
                }
            }else e = e->next[i];
        }
    }
    return e;
}

SkipList skiplist_insert(SkipList d, int value) {

    LinkedElement *e = skiplist_find(d, value);

    if (e == NULL) return d;
    LinkedElement *newElem = malloc(sizeof(LinkedElement));
    newElem->next = malloc(sizeof(LinkedElement) * newElem->level);
    newElem->prev = malloc(sizeof(LinkedElement) * newElem->level);
    newElem->value = value;
    newElem->level = rng_get_value(&d->rng, d->nblevels);
    newElem->prev[0] = e;
    newElem->next[0] = e->next[0];
    e->next[0] = newElem;

    for (int j = 1; j < newElem->level; j++) {
        e = newElem->next[0];
        for (bool isLinked = false; !isLinked; e = e->next[0]) {
            if (e == d->sentinelle) {
                newElem->next[j] = d->sentinelle;
                isLinked = true;
            } else if (e->level >= j) {
                newElem->next[j] = e;
                isLinked = true;
            }
        }
        e = newElem->prev[0];
        for (bool isLinked = false; !isLinked; e = e->prev[0]) {
            if (e == d->sentinelle) {
                newElem->prev[j] = d->sentinelle;
                isLinked = true;
            } else if (e->level >= j) {
                newElem->prev[j] = e;
                isLinked = true;
            }
        }

    }
    (d->size)++;
    return d;
}

