//
// Created by Stephan on 11.03.2022.
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

void free_item(Item * item) {
    free(item->info);
    //free(item->)
    free(item);
}

Table * create_table(IndexType1 msize1, IndexType2 msize2) {
    Table * table = malloc(sizeof(Table));

    table->msize1.index = msize1.index;
    table->csize1.index = msize1.index*msize2.index;
    table->msize2.index = msize2.index;
    table->csize2.index = msize1.index*msize2.index;

    table->ks1 = malloc(sizeof(KeySpace1) * table->msize1.index);
    table->ks2 = create_KS2(msize2.index); //malloc(sizeof(KeySpace2) * table->msize2.index);

    for (int i = 0; i < msize1.index; ++i)
        table->ks1[i] = create_KS1(i);

    return table;
}

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

bool el_in_table(Table table, Item item) {
    for (int i = 0; i < table.msize1.index; ++i) {
        if (table.ks1[i].key.busy) {
            if (el_in_KS1(*(table.ks1), item))
                return true;
            return false;
        }
    }
    return false;
}

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

Item * create_item(InfoType * info) {
    Item * item = malloc(sizeof(Item));
    item->info = info;
    item->key1 = chose_key1(*item);
    item->key2 = chose_key2(*item);
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
    Item * item = create_item(info);    // на основе полученных данных формируем итем
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