//
// Created by Stephan on 11.03.2022.
//

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "time.h"
#include "table_structs.h"
#include "Errors.h"

void free_ks1(KeySpace1 * ks1) {
    for ()
}

KeySpace1 create_KS1(int i) {
    KeySpace1 KS1;// = malloc(sizeof(KeySpace1));
    KS1.key.key = i;
    KS1.key.busy = false;
    KS1.node = NULL;
    return KS1;
}

KeyType1 chose_key1(Item item) {
    srand(time(NULL));
    KeyType1 key;
    key.key = rand()%10;
    key.busy = 1;
    return key;
}

IndexType1 create_ind1() {
    IndexType1 ind1;
    ind1.index = 0;
    return ind1;
}

bool keys1_eq(KeyType1 key1, KeyType1 key2) {
    if (key1.key == key2.key)
        return true;
    return false;
}

KeySpace1 * get_KS1(Table table, KeyType1 key) {
    for (int i = 0; i < table.msize1.index; ++i) {
        if (keys1_eq(table.ks1[i].key, key)) {
            return table.ks1 + i;
        }
    }
    return NULL;
}

void add_el_in_KS1(Table * table, Item * item) {
    KeySpace1 * key = get_KS1(*table, item->key1);
    printf("%p: %p\n", key, key->node);
    if (key == NULL) {
        fprintf(stderr, "\nImpossible key.\n");
        exit(IMPOSSIBLE_KEY);
    }

    Node1 * node = key->node;
    Node1 * pr_node = key->node;
    int i = 0;
    while (node) {
        printf("i: %d\n", i);
        pr_node = node;
        node = node->next;
        i++;
    }
    if (pr_node) {
        pr_node->next = malloc(sizeof(Node1));
        //node = pr_node->next;
        pr_node->next->next = NULL;
        pr_node->next->info = item;
        pr_node->next->release.numberOfRelease = i;
    }
    else {
        key->node = malloc(sizeof(Node1));
        key->key.busy = true;
        //node = key->node;
        key->node->next = NULL;
        key->node->info = item;
        key->node->release.numberOfRelease = i;
    }
}
