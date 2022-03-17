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

enum TableCondition {
    TABLE_IS_OVER,
    THERE_IS_NO_FREE_KS,
    THERE_IS_FREE_KS,
};

enum TableCondition table_condition(Table * table) {
    enum TableCondition tc = TABLE_IS_OVER;
    for (int i = 0; i < table->msize1.index; ++i) {
        if (table->ks1[i].key.busy) {
            Node1 * node = table->ks1[i].node;
            int n = 0;
            while (node) {
                node = node->next;
                n++;
            }
            if (n == table->msize2.index)
                continue;
            else {
                tc = THERE_IS_NO_FREE_KS;
                continue;
            }
        }
        else
            return THERE_IS_FREE_KS;
    }
    return tc;
}

void add_el(Table * table, Item * item) {
    //KeyType1 key1 = item->key1;
    //KeyType2 key2 = item->key2;

    if (table_condition(table) == TABLE_IS_OVER) {
        printf("This table is overflow\n");
        exit(TABLE_IS_OVERFLOW);
    }

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

Item * create_item(Table * table, InfoType * info, KeyType1 key1, KeyType2 key2) {
    Item * item = malloc(sizeof(Item));
    item->info = info;
    if (key1.key < 0 || key1.key > table->msize1.index) {
        free_item(item);
        printf("Unresolved key");
        return NULL;
    }
    cope_key1(&(item->key1), key1);
    cope_key2(&(item->key2), key2);

    if (el_k1_k2_in_table(table, item->key1, item->key2)) {
        free(item);
        return NULL;
    }

    item->ind1 = create_ind1();
    item->ind2 = create_ind2();
    return item;
}

InfoType * get_info() {
    InfoType * info = malloc(sizeof(InfoType));
    printf("Enter your data here:\n-> ");
    info->data = get_line();
    return info;
}

void add_info_dialog(Table * table) {
    printf("enter your info.\n");
    InfoType * info = get_info();       // считываем введенную информацию

    ///get_line();

    KeyType1 key1 = get_key1();
    KeyType2 key2 = get_key2();

    Item * item = create_item(table, info, key1, key2);    // на основе полученных данных формируем итем

    if (item == NULL) {
        free(info);
        printf("this table is empty or your key is wrong\n");
        exit(TABLE_IS_OVERFLOW);
    }
    printf("item was created\n");
    add_el(table, item);                // добавляем итем в таблицу
}

void print_item(const Item item) {
    printf("key1: %d; key2: %d\n", item.key1.key, item.key2.key);
    // printf("release: %d\n", item.);
    printf("data: %s\n", item.info->data);
}

void print_node1(Node1 node) {
    printf("key1: %d, key2: %d\n", node.info->key1.key, node.info->key2.key);
    printf("data: %s\n", node.info->info->data);
}
void print_node2(Node2 node) {
    printf("key1: %d, key2: %d\n", node.info->key1.key, node.info->key2.key);
    printf("data: %s\n", node.info->info->data);
}

void print_table(const Table table) {
    int size1 = table.msize1.index;
    int size2 = table.msize2.index;

    printf("The first table.\n");
    for (int i = 0; i < size1; ++i) {
        printf("key: %d, busy: %d\n", table.ks1[i].key.key, table.ks1[i].key.busy);
        if (table.ks1[i].key.busy) {
            Node1 * node = table.ks1[i].node;
            while (node) {
                print_node1(*node);
                node = node->next;
            }
        }
        printf("\n");
    }
    printf("The second table.\n");
    for (int i = 0; i < size2; ++i) {
        printf("hash: %d, key: %d, busy: %d\n", i, table.ks2[i].key.key, table.ks2[i].key.busy);
        if (table.ks2[i].key.busy) {
            Node2 * node = table.ks2[i].node;
            while (node) {
                print_node2(*node);
                node = node->next;
            }
        }
        printf("\n");
    }
}

/*
 * find item with 2 keys
 */

void find_item(Table table, KeyType1 key1, KeyType2 key2) {
    if (!el_k1_k2_in_table(&table, key1, key2)) {
        printf("There is no element with such keys.\n");
        return;
    }

    for (int i = 0; i < table.msize1.index; ++i) {
        if (table.ks1[i].key.busy) {
            if (table.ks1[i].key.key == key1.key) {
                Node1 * node = table.ks1[i].node;
                while (node) {
                    if (keys2_eq(node->info->key2, key2)) {
                        printf("release1: %d; release2: %d\n", node->release.numberOfRelease, node->info->p2->release.numberOfRelease);
                        print_item(*(node->info));
                        return;
                    }
                    node = node->next;
                }
            }
        }
    }

    printf("Something came wrong. Kik any programmer to fix this.\n");
}

void find_items_k1(Table table, KeyType1 key) {
    KeySpace1 * ks = getKey1(table.ks1, key, table.msize1.index);
    if (ks) {
        Node1 * node = ks->node;
        while (node) {
            print_node1(*node);
            node = node->next;
        }
        return;
    }
    printf("There is not items with such key.\n");
}
void find_items_k2(Table table, KeyType2 key) {
    KeySpace2 * ks = getKey2(table.ks2, key, table.msize2.index);
    if (ks) {
        Node2 * node = ks->node;
        while (node) {
            print_node2(*node);
            node = node->next;
        }
        return;
    }
    printf("There is not items with such key.\n");
}

void find_el_k1_k2_dialog(Table * table) {
    printf("What are you searching for?\n1) El with 2 keys\n2) Els with 1 key");
    int x = -1;
    while (x < 1 || x > 2) {
        printf("\n");
        x = get_int();
    }

    switch (x) {
        case 1: {
            printf("enter your keys:\n");
            printf("key1:\n");
            int k1 = get_int();
            printf("key2:\n");
            int k2 = get_int();
            KeyType1 key1 = {1, k1};
            KeyType2 key2 = {1, k2};
            find_item(*table, key1, key2);
            break;
        }
        case 2: {
            printf("Which key do you have?\n1) key1\n2) key2\n");
            x = 0;
            while (x < 1 || x > 2) {
                printf("\n");
                x = get_int();
            }
            printf("enter your key:\n");
            int k = get_int();
            if (x == 1) {
                KeyType1 key = {1, k};
                find_items_k1(*table, key);
            } else if (x == 2) {
                KeyType2 key = {1, k};
                find_items_k2(*table, key);
            } else
                return;
            break;
        }
        default:
            break;
    }
}

/*
 * delete an item with two keys
 */

KeySpace1 * getKey1(KeySpace1 * KS, KeyType1 key, int size) {
    for (int i = 0; i < size; ++i) {
        if (KS[i].key.busy) {
            if (KS[i].key.key == key.key) {
                return KS+i;
            }
        }
    }
    return NULL;
}
KeySpace2 * getKey2(KeySpace2 * KS, KeyType2 key, int size) {
    for (int i = 0; i < size; ++i) {
        if (KS[i].key.busy) {
            if (KS[i].key.key == key.key) {
                return KS+i;
            }
        }
    }
    return NULL;
}

int number_of_nodes1(Node1 * node) {
    if (node == NULL) {
        return 0;
    }
    int x = number_of_nodes1(node->next);
    node->release.numberOfRelease = x;
    return x+1;
}
int number_of_nodes2(Node2 * node) {
    if (node == NULL) {
        return 0;
    }
    int x = number_of_nodes2(node->next);
    node->release.numberOfRelease = x;
    return x+1;
}

void delete_node1(KeySpace1 * ks1, KeyType1 key1, KeyType2 key2) {
    Node1 * node = ks1->node;
    Node1 * pr_node = ks1->node;
    while (node && node->info->key2.key != key2.key) {
        pr_node = node;
        node = node->next;
    }
    if (node == NULL) {
        fprintf(stderr, "NULL prt in searching for node\n");
        exit(333);
    }
    if (pr_node == node)
        ks1->node = ks1->node->next;
    else
        pr_node->next = node->next;

    free(node);

    number_of_nodes1(ks1->node);
}
void delete_node2(KeySpace2 * ks2, KeyType1 key1, KeyType2 key2) {
    Node2 * node = ks2->node;
    Node2 * pr_node = ks2->node;
    while (node && node->info->key1.key != key1.key) {
        pr_node = node;
        node = node->next;
    }
    if (node == NULL) {
        fprintf(stderr, "NULL prt in searching for node\n");
        exit(333);
    }
    if (pr_node == node)
        ks2->node = ks2->node->next;
    else
        pr_node->next = node->next;
    free(node);

    number_of_nodes2(ks2->node);
}

Item * get_item(Table * table, KeyType1 key1, KeyType2 key2) {
    for (int i = 0; i < table->msize1.index; ++i) {
        if (table->ks1[i].key.busy) {
            if (table->ks1[i].key.key == key1.key) {
                Node1 * node = table->ks1[i].node;
                while (node && node->info->key2.key != key2.key) {
                    node = node->next;
                }
                if (node == NULL)
                    return NULL;
                return node->info;
            }
        }
    }
    return NULL;
}

void delete_el(Table * table, KeyType1 key1, KeyType2 key2) {
    if (!el_k1_k2_in_table(table, key1, key2)) {
        printf("There is no element with such keys.\n");
        return;
    }

    Item * item = get_item(table, key1, key2);
    if (item == NULL) {
        fprintf(stderr, "Something came wrong. Kik any programmer to fix this.\n");
        return;
    }

    KeySpace1 * ks1 = getKey1(table->ks1, key1, table->msize1.index);
    KeySpace2 * ks2 = getKey2(table->ks2, key2, table->msize2.index);

    delete_node1(ks1, key1, key2);
    if (ks1->node == NULL) {
        memmove(ks1, ks1+1, ((size_t)(table->ks1+table->msize1.index-1) - (size_t)ks1));
    }
    delete_node2(ks2, key1, key2);
    if (ks2->node == NULL) {
        ks2->key.busy = false;
    }
    free_item(item);
}

void delete_el_k1_k2_dialog(Table * table) {
    printf("What do you wanna to delete?\n1) El with 2 keys\n2) Els with 1 key");
    int x = -1;
    while (x < 1 || x > 2) {
        printf("\n");
        x = get_int();
    }

    switch (x) {
        case 1: {
            printf("enter keys of this element:\n");
            printf("key1:\n");
            int k1 = get_int();
            printf("key2:\n");
            int k2 = get_int();

            KeyType1 key1 = {1, k1};
            KeyType2 key2 = {1, k2};

            if (!el_k1_k2_in_table(table, key1, key2)) {
                printf("there is not such element in table\n");
                return;
            }

            delete_el(table, key1, key2);
            break;
        }
        case 2: {
            printf("Which key do you have?\n1) key1\n2) key2\n");
            x = 0;
            while (x < 1 || x > 2) {
                printf("\n");
                x = get_int();
            }

            printf("enter your key:\n");
            int k = get_int();

            if (x == 1) {
                KeyType1 key = {1, k};
                KeySpace1 * ks = getKey1(table->ks1, key, table->msize1.index);
                int number = number_of_nodes1(ks->node);
                int i = 0;
                KeyType2 * keys = malloc(sizeof(KeyType2) * number);
                Node1 * node = ks->node;
                while (node) {
                    keys[i].key = node->info->key2.key;
                    i++;
                    node = node->next;
                }
                for (int j = 0; j < number; ++j)
                    delete_el(table, key, keys[j]);
                free(keys);
            } else if (x == 2) {
                KeyType2 key = {1, k};
                KeySpace2 * ks = getKey2(table->ks2, key, table->msize2.index);
                int number = number_of_nodes2(ks->node);
                int i = 0;
                KeyType1 * keys = malloc(sizeof(KeyType1) * number);
                Node2 * node = ks->node;
                while (node) {
                    keys[i].key = node->info->key1.key;
                    i++;
                    node = node->next;
                }
                for (int j = 0; j < number; ++j)
                    delete_el(table, keys[j], key);
                free(keys);
            } else
                return;
            break;
        }
        default:
            break;
    }
}