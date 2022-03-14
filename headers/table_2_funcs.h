//
// Created by Stephan on 11.03.2022.
//

#ifndef LAB3_TABLE_2_FUNCS_H
#define LAB3_TABLE_2_FUNCS_H
KeySpace2 * create_KS2(int number);
void add_el_in_KS2(Table * table, Item * item);
KeyType2 chose_key2(Item item);
IndexType2 create_ind2();
void free_table2(Table * table);
bool el_k2_in_table2(Table * table, KeyType2 key);
KeySpace2 * get_KS2(Table table, KeyType2 key);
bool keys2_eq(KeyType2 key1, KeyType2 key2);
#endif //LAB3_TABLE_2_FUNCS_H
