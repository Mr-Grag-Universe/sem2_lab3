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
#include "MyString.h"

void free_node2(Node2 * node) {
    if (node == NULL) {
        exit(INVALID_FREE);
    }
    Node2 * n_p = node;
    while (node) {
        n_p = n_p->next;
        free_item(node->info);
        node->info = NULL;
        free(node);
        node = n_p;
    }
    free(n_p);
}

void free_table2(Table * table) {
    KeySpace2 * ks_1 = table->ks2;
    KeySpace2 * ks_2 = table->ks2;
    while (ks_1) {
        ks_2 = ks_2->next;
        free_node2(ks_1->node);
        ks_1->node = NULL;
        free(ks_1);
        ks_1 = ks_2;
    }
    // free(ks_1);
    table->ks2 = NULL;
}

KeySpace2 create_KS2(int i) {
    /*
    if (i <= 0)
        return NULL;

    KeySpace2 * KS2 = malloc(sizeof(KeySpace2));
    KS2->key.key = i-1;
    KS2->next = create_KS2(i-1);
    // KS2.key.busy = false;
    KS2->node = NULL;
    return KS2;
     */
    KeySpace2 ks2;// = malloc(max_size* sizeof(KeySpace2));
    //for (int i = 0; i < max_size; ++i) {
    ks2.key = (KeyType2){false, i};
    ks2.node = NULL;
    ks2.next = NULL;
    //}
    return ks2;
}

int hash_func(KeyType2 key) {
    return key.key % 100;
}

void cope_key2(KeyType2 * res, KeyType2 key) {
    *(res) = (KeyType2) {key.busy, key.key};
}

KeyType2 get_key2() {
    KeyType2 key;
    printf("enter your key for the new element in the hash table.\n");
    key.key = get_int();
    key.busy = true;

    return key;
}

IndexType2 create_ind2() {
    IndexType2 ind2;
    ind2.index = 0;
    return ind2;
}

bool keys2_eq(KeyType2 key1, KeyType2 key2) {
    if (key1.key == key2.key)
        return true;
    return false;
}

KeySpace2 * get_KS2(Table table, Item * item) {
    int ind = hash_func(item->key2)%table.msize2.index;

    KeySpace2 * ks2 = table.ks2+ind;
    KeySpace2 * ks_pr = table.ks2+ind;
    int i = ind;
    item->ind2.index = ind;

    if (ks2->node == NULL) {// || ) {
        return ks2;
    }
    if (keys2_eq(ks2->key, item->key2)) {
        return ks2;
    }
    i++;

    while (table.ks2[i].key.busy) {
        if (keys2_eq(table.ks2[i].key, item->key2)) {
            return table.ks2 + i;
        } else if (i == ind) {
            return NULL;
        }
        //ks_pr = ks2;
        //ks2 = ks2->next;
        i = (i + 1) % table.msize2.index;
    }

    //ks_pr->next = malloc(sizeof(KeySpace2));
    //ks_pr = ks_pr->next;
    //ks_pr->next->next = NULL;
    //ks_pr->next->node = NULL;
    //ks_pr->next->key.key = item.key2.key;
    //return ks_pr->next;
    return table.ks2+i;
}

void add_el_in_KS2(Table * table, Item * item) {
    KeySpace2 * key = get_KS2(*table, item);
    //item->p2 = key;
    if (key == NULL) {
        fprintf(stderr, "\nImpossible key.\n");
        exit(IMPOSSIBLE_KEY);
    }
    printf("%p: %p\n", key, key->node);

    Node2 * node = key->node;
    Node2 * pr_node = key->node;

    printf("start finding a free node\n");

    cope_key2(&(key->key), item->key2);
    key->key.busy = true;

    if (node) {
        key->node = malloc(sizeof(Node2));
        //node = pr_node->next;
        key->node->next = pr_node;
        key->node->info = item;
        key->node->release.numberOfRelease = pr_node->release.numberOfRelease;
        item->p2 = key->node;
    }
    else {
        key->node = malloc(sizeof(Node2));
        //node = key->node;
        key->node->next = NULL;
        key->node->info = item;
        key->node->release.numberOfRelease = 0;
        item->p2 = key->node;
    }
}

bool el_k2_in_table2(Table * table, KeyType2 key) {
    for (int i = 0; i < table->msize2.index; ++i) {
        if (table->ks2[i].key.busy) {
            if (table->ks2[i].key.key == key.key) {
                return true;
            }
        }
    }

    return false;
}
