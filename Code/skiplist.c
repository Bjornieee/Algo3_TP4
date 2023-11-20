#include <stdlib.h>
#include <assert.h>

#include "skiplist.h"
#include "rng.h"
typedef struct s_LinkedElement LinkedElement;

typedef struct s_LinkedElement{
    int value;
    int level;
    LinkedElement **prev;
    LinkedElement **next;
}LinkedElement;

struct s_SkipList{
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
    //set all pointers on sentinelle
    for(int i=0;i<nblevels;i++){
        skipList->sentinelle->next[i] = skipList->sentinelle;
        skipList->sentinelle->prev[i] = skipList->sentinelle;
    }
	return skipList;
}

void skiplist_delete(SkipList d) {

}

SkipList skiplist_insert(SkipList d, int value) {
	(void)value;
	return d;
}
