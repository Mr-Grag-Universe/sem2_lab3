//
// Created by Stephan on 14.03.2022.
//

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "time.h"
#include "Errors.h"
#include "KGetLine.h"
#include "MyString.h"
#include "headers/table_structs.h"
#include "headers/table_1_funcs.h"
#include "headers/table_2_funcs.h"
#include "table_1_funcs.h"
#include "table_creation.h"
#include "table_functions.h"


void free_table(Table * T) {
    free_table1(T);
    free_table2(T);
    free(T);
}

void clear_table(Table * T) {
    // clear_ks1(T->ks1);
    // clear_ks2(T->ks2);
    T->msize1.index = 0;
    T->msize2.index = 0;
    T->csize1.index = 0;
    T->csize2.index = 0;
}

/*
 * функции для обнаружения элемента в таблице
 */

void add_el(Table * table, Item * item) {
    //KeyType1 key1 = item->key1;
    //KeyType2 key2 = item->key2;

    add_el_in_KS1(table, item);
    add_el_in_KS2(table, item);
    print_table(*table);
}

/*
 * вспомогательные функции
 * создают диалог
 * считывают инфу
 * преобразуют инфу в итем
 * и передают в основную функцию добавления
 */


bool el_k1_k2_in_table(Table * table, KeyType1 key1, KeyType2 key2) {
    bool x = el_k1_in_table1(table, key1);
    bool y = el_k2_in_table2(table, key2);
    return x && y;
}

Item * create_item(Table * table, InfoType * info) {
    Item * item = malloc(sizeof(Item));
    item->info = info;
    item->key1 = chose_key1(*item);
    item->key2 = chose_key2(*item);

    if (el_k1_k2_in_table(table, item->key1, item->key2)) {
        free(item);
        return NULL;
    }

    item->ind1 = create_ind1();
    item->ind2 = create_ind2();
    return item;
}

InfoType * get_info() {
    char * answer;
    InfoType * info = malloc(sizeof(InfoType));
    printf("Enter your data here:\n-> ");
    info->data = get_line();
    return info;
}

void add_info_dialog(Table * table) {
    printf("enter your info.\n");
    InfoType * info = get_info();       // считываем введенную информацию

    Item * item = create_item(table, info);    // на основе полученных данных формируем итем
    if (item == NULL) {
        free(info);
        printf("this table is empty");
        exit(TABLE_IS_OVERFLOW);
    }
    printf("item was created\n");
    add_el(table, item);                // добавляем итем в таблицу
}

void print_table(const Table table) {
    int size1 = table.msize1.index;
    int size2 = table.msize2.index;

    printf("The first table.\n");
    for (int i = 0; i < size1; ++i) {
        printf("%d, %d", table.ks1[i].key.key, table.ks1[i].key.busy);
        if (table.ks1[i].key.busy) {
            Node1 * node = table.ks1[i].node;
            while (node) {
                printf("%p\t", node);
                node = node->next;
            }
        }
        printf("\n");
    }
    printf("The second table.\n");
    KeySpace2 * ks2 = table.ks2;
    while (ks2) {
        printf("%d: ", ks2->key.key);
        Node2 * n = ks2->node;
        while (n) {
            printf("%p, ", n);
            n = n->next;
        }
        printf("\b\b\n");
        ks2 = ks2->next;
    }
    //printf("\n");
}

void print_item(const Item item) {
    printf("key1: %d; key2: %d\n", item.key1.key, item.key2.key);
    // printf("release: %d\n", item.);
    printf("data: %s\n", item.info->data);
}

void find_item(Table table, KeyType1 key1, KeyType2 key2) {
    if (!el_k1_k2_in_table(&table, key1, key2)) {
        printf("There is no element with such keys.\n");
        return;
    }

    KeySpace1 * ks1 = get_KS1(table, key1);
    KeySpace2 * ks2 = get_KS2(table, key2);

    Node1 * node = ks1->node;
    while (node) {
        if (keys2_eq(node->info->key2, key2)) {
            printf("release1: %d; release2: %d\n", node->release.numberOfRelease, node->info->p2->release.numberOfRelease);
            print_item(*(node->info));
            return;
        }
        node = node->next;
    }
    printf("Something came wrong. Kik any programmer to fix this.\n");
}

void find_el_k1_k2_dialog(Table * table) {
    printf("enter your keys:\n");
    printf("key1:\n");
    int k1 = get_int();
    printf("key2:\n");
    int k2 = get_int();

    KeyType1 key1 = {0, k1};
    KeyType2 key2 = {k2};

    find_item(*table, key1, key2);
}
