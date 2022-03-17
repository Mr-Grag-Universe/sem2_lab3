//
// Created by Stephan on 11.03.2022.
//

#ifndef LAB3_TABLE_FUNCTIONS_H
#define LAB3_TABLE_FUNCTIONS_H
void free_table(Table * T);
void clear_table(Table * T);
void add_info_dialog(Table * table);
void print_table(const Table table);
void free_item(Item * item);
void find_el_k1_k2_dialog(Table * table);
void delete_el_k1_k2_dialog(Table * table);
KeySpace1 * getKey1(KeySpace1 * KS, KeyType1 key, int size);
KeySpace2 * getKey2(KeySpace2 * KS, KeyType2 key, int size);
#endif //LAB3_TABLE_FUNCTIONS_H
