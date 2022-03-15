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

KeySpace2 * create_KS2(int i) {
    if (i <= 0)
        return NULL;

    KeySpace2 * KS2 = malloc(sizeof(KeySpace2));
    KS2->key.key = i-1;
    KS2->next = create_KS2(i-1);
    // KS2.key.busy = false;
    KS2->node = NULL;
    return KS2;
}

KeyType2 chose_key2(Item item) {
    srand(time(NULL)+2);
    KeyType2 key;
    key.key = rand()%10;
    //key.busy = 1;
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

KeySpace2 * get_KS2(Table table, KeyType2 key) {
    KeySpace2 * ks2 = table.ks2;
    KeySpace2 * ks_pr = table.ks2;
    int i = 0;
    while (ks2 && i < table.msize2.index) {
        if (keys2_eq(ks2->key, key)) {
            return ks2;
        }
        ks_pr = ks2;
        ks2 = ks2->next;
        i++;
    }

    if (i == table.msize2.index) {
        return NULL;
    }

    ks_pr->next = malloc(sizeof(KeySpace2));
    //ks_pr = ks_pr->next;
    ks_pr->next->next = NULL;
    ks_pr->next->node = NULL;
    ks_pr->next->key.key = key.key;
    return ks_pr->next;
}

void add_el_in_KS2(Table * table, Item * item) {
    KeySpace2 * key = get_KS2(*table, item->key2);
    //item->p2 = key;
    if (key == NULL) {
        fprintf(stderr, "\nImpossible key.\n");
        exit(IMPOSSIBLE_KEY);
    }
    printf("%p: %p\n", key, key->node);

    Node2 * node = key->node;
    Node2 * pr_node = key->node;

    printf("start finding a free node\n");

    int i = 0;
    while (node) {
        printf("i: %d\n", i);
        pr_node = node;
        node = node->next;
        i++;
    }
    if (pr_node) {
        pr_node->next = malloc(sizeof(Node2));
        //node = pr_node->next;
        pr_node->next->next = NULL;
        pr_node->next->info = item;
        pr_node->next->release.numberOfRelease = i;
        item->p2 = pr_node->next;
    }
    else {
        key->node = malloc(sizeof(Node2));
        //key->key.busy = true;
        //node = key->node;
        key->node->next = NULL;
        key->node->info = item;
        key->node->release.numberOfRelease = i;
        item->p2 = key->node;
    }
}

bool el_k2_in_table2(Table * table, KeyType2 key) {
    KeySpace2 * ks = table->ks2;
    while (ks) {
        if (ks->node) {
            if (keys2_eq(ks->key, key)) {
                return true;
            }
        }
        ks = ks->next;
    }
    return false;
}
