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

void free_node1(Node1 * node) {
    if (node == NULL) {
        //return;
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
    KS1.key.key = 0;
    KS1.key.busy = false;
    KS1.node = NULL;
    return KS1;
}

KeyType1 get_key1() {
    ///srand(time(NULL));
    KeyType1 key;
    printf("enter your key for the new element in the lookthrough table.\n");
    key.key = get_int(); // rand()%10;
    key.busy = true;
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

int comparator(const void * ks1, const void * ks2) {
    KeySpace1 * KS1 = (KeySpace1*) ks1;
    KeySpace1 * KS2 = (KeySpace1*) ks2;

    if (KS1->key.key < KS2->key.key) {
        return 1;
    }
    if (KS1->key.key > KS2->key.key) {
        return -1;
    }
    return 0;
}

int number_of_ks1_b(Table table) {
    for (int i = 0; i < table.msize1.index; ++i) {
        if (table.ks1[i].key.busy == false)
            return i;
    }
    return table.msize1.index;
}

int binarySearch(Table * table, KeyType1 key) {

    int left = 0, right = number_of_ks1_b(*table);
    while (left != right) {
        int m = (left+right)/2;
        KeySpace1 * p = (table->ks1+m);
        if (p->key.busy) {
            if (comparator(&key, p) < 0) {
                //if (comparator(el, p) > 0) left = m + 1;
                left = m + 1;
            }
            else if (comparator(&key, p) == 0)
                return m;
            else
                right = m;
        }
        else right = m;
        //else if (comparator(&key, p) < 0) right = m;
        //else return m;
    }
    return left;
}

void cope_key1(KeyType1 * res, KeyType1 key) {
    *(res) = (KeyType1) {key.busy, key.key};
}

KeySpace1 * get_KS1(Table * table, KeyType1 key) {
    if (table->ks1[0].key.busy == false) {
        cope_key1(&(table->ks1->key), key);
        return table->ks1;
    }

    KeySpace1 * ks = &((KeySpace1) {key, NULL});
    //getchar();
    size_t x = binarySearch(table, key);
    if (table->ks1[x].key.busy) {
        if (table->ks1[x].key.key == key.key)
            return table->ks1+x;
    }
    //getchar();
    KeySpace1 * ks1 = table->ks1 + x;//table.ks1, ks, sizeof(*ks), table.msize1.index, comparator);
    if (table->ks1 + table->msize1.index-1 == ks1) {
        if (keys1_eq(ks1->key, ks->key)) {
            return ks1;
        }
        else
            return NULL;
    }

    int num = number_of_ks1_b(*table);
    memmove(table->ks1 + x+1, table->ks1 + x, sizeof(KeySpace1) * (num-x));
    print_table(*table);
    cope_key1(&(table->ks1[x].key), key);
    table->ks1[x].key.busy = true;
    table->ks1[x].node = NULL;

    return ks1;
}

void add_el_in_KS1(Table * table, Item * item) {
    KeySpace1 * key = get_KS1(table, item->key1);
    getchar();
    //item->p1 = key;
    //printf("%p: %p\n", key, key->node);
    if (key == NULL) {
        fprintf(stderr, "\nImpossible key.\n");
        exit(IMPOSSIBLE_KEY);
    }
    key->key.busy = true;

    //Node1 * node = key->node;
    Node1 * node = malloc(sizeof(Node1));
    node->info = item;
    node->next = key->node;
    key->node = node;
    if (node->next)
        node->release.numberOfRelease = node->next->release.numberOfRelease;
    else
        node->release.numberOfRelease = 0;

    /*
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
        pr_node->next->next = NULL;
        pr_node->next->info = item;
        pr_node->next->release.numberOfRelease = i;
        item->p1 = pr_node->next;
    }
    else {
        key->node = malloc(sizeof(Node1));
        key->key.busy = true;
        key->node->next = NULL;
        key->node->info = item;
        item->p1 = key->node;
        key->node->release.numberOfRelease = i;
    }*/
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
            if (key.key == table->ks1[i].key.key)
                return true;
            //return false;
        }
    }
    return false;
}
