//
// Created by Stephan on 11.03.2022.
//

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "time.h"
#include "table_structs.h"
#include "table_creation.h"
#include "table_functions.h"
#include "Errors.h"

void free_node1(Node1 * node) {
    if (node == NULL) {
        exit(INVALID_FREE);
    }
    Node1 * n_p = node;
    while (node) {
        n_p = n_p->next;
        free_item(node->info);
        free(node);
        node = n_p;
    }
    free(n_p);
}

void free_table1(Table * table) {
    for (int i = 0; i < table->msize1.index; ++i) {
        free_node1(table->ks1[i].node);
    }
    free(table->ks1);
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
    //item->p1 = key;
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
        item->p1 = pr_node->next;
    }
    else {
        key->node = malloc(sizeof(Node1));
        key->key.busy = true;
        //node = key->node;
        key->node->next = NULL;
        key->node->info = item;
        item->p1 = key->node;
        key->node->release.numberOfRelease = i;
    }
}


bool items_eq(Item item1, Item item2) {
    if (strcmp(item1.info->data, item2.info->data))
        return false;
    return true;
}

bool el_in_KS1(KeySpace1 ks, Item item) {
    Node1 * node = ks.node;
    while (node) {
        if (items_eq(item, *(node->info)))
            return true;
        node = node->next;
    }
    return false;
}

bool el_k1_in_table1(Table * table, KeyType1 key) {
        for (int i = 0; i < table->msize1.index; ++i) {
            if (table->ks1[i].key.busy) {
                if (keys1_eq(table->ks1[i].key, key))
                    return true;
                return false;
            }
        }
        return false;
}
