//
// Created by Stephan on 11.03.2022.
//

#ifndef LAB3_TABLE_2_FUNCS_H
#define LAB3_TABLE_2_FUNCS_H
KeySpace2 create_KS2(int i);
void add_el_in_KS2(Table * table, Item * item);
int hash_func(Item item);
IndexType2 create_ind2();
void free_table2(Table * table);
bool el_k2_in_table2(Table * table, KeyType2 key);
KeySpace2 * get_KS2(Table table, KeyType2 key);
bool keys2_eq(KeyType2 key1, KeyType2 key2);
void cope_key2(KeyType2 * res, KeyType2 key);
KeyType2 get_key2();
#endif //LAB3_TABLE_2_FUNCS_H
