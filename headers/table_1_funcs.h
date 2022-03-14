//
// Created by Stephan on 11.03.2022.
//

#ifndef LAB3_TABLE_1_FUNCS_H
#define LAB3_TABLE_1_FUNCS_H
KeySpace1 create_KS1(int i);
KeyType1 chose_key1(Item item);
IndexType1 create_ind1();
void add_el_in_KS1(Table * table, Item * item);
void free_table1(Table * table);
#endif //LAB3_TABLE_1_FUNCS_H
