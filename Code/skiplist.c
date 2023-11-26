#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "skiplist.h"
#include "rng.h"


typedef struct s_LinkedElement {
    int value;
    int level;
    struct s_LinkedElement **prev;
    struct s_LinkedElement **next;
}LinkedElement;

struct s_SkipList {
    int nblevels;
    RNG rng;
    int size;
    LinkedElement *sentinelle;
};

typedef struct s_Tuple {
    LinkedElement *e;
    unsigned int nbOperations;
}Tuple;

SkipList skiplist_create(int nblevels) {
    //create list
    SkipList skipList = malloc(sizeof(struct s_SkipList));
    LinkedElement *sentinnelle = malloc(sizeof(LinkedElement));
    //initialize values
    skipList->sentinelle = sentinnelle;
    skipList->sentinelle->level = nblevels;
    skipList->sentinelle->value = -1;
    skipList->sentinelle->value = -1;
    skipList->rng = rng_initialize(0);
    skipList->size = 0;
    skipList->nblevels = nblevels;
    struct s_LinkedElement **next = malloc(sizeof(LinkedElement) * nblevels);
    struct s_LinkedElement **prev = malloc(sizeof(LinkedElement) * nblevels);
    skipList->sentinelle->next = next;
    skipList->sentinelle->prev = prev;
    //set all pointers on sentinel
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

Tuple *skiplistFind(SkipList d, int value) {
    LinkedElement *e = d->sentinelle;
    Tuple *tuple = malloc(sizeof(Tuple));
    int nbOccurence = 0;
    int i = d->nblevels - 1;
    whileStatement:
    while (e->next[i]->value <= value && e->next[i] != d->sentinelle) {
        if (e->next[i]->value == value) {
            tuple->e = NULL;
            tuple->nbOperations = nbOccurence;
            return tuple;
        }
        e = e->next[i];
        nbOccurence++;
    }
    if (e->next[i] == d->sentinelle) {
        if (i == 0) {
            tuple->e = d->sentinelle->prev[0];
            tuple->nbOperations = nbOccurence;
            return tuple;
        }
        i--;
        goto whileStatement;
    }
    tuple->e = e;
    tuple->nbOperations = nbOccurence;
    return tuple;
}

SkipList skiplist_insert(SkipList d, int value) {
    LinkedElement *e = skiplistFind(d, value)->e;
    // if value is already in the list do nothing
    if (e == NULL) {
        free(e);
        return d;
    }
    //create and initialize the element to insert in the list
    LinkedElement *newElem = malloc(sizeof(LinkedElement));
    newElem->level = rng_get_value(&d->rng, d->nblevels);
    newElem->next = malloc(sizeof(LinkedElement *) * newElem->level);
    newElem->prev = malloc(sizeof(LinkedElement *) * newElem->level);
    newElem->value = value;
    //link at level 0
    newElem->prev[0] = e;
    newElem->next[0] = e->next[0];
    e->next[0] = newElem;
    newElem->next[0]->prev[0] = newElem;
    //link at higher levels
    for (int j = 1; j < newElem->level; j++) {
        //next links
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
        //previous links
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
    //increment size
    (d->size)++;
    return d;
}

bool skiplist_search(SkipList d, int value, unsigned int *nb_operations) {
    Tuple *tuple = skiplistFind(d, value);
    *nb_operations = (!(tuple->e) ? (tuple->nbOperations) : 1);
    return !(tuple->e);
}