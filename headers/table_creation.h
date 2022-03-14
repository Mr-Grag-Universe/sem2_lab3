//
// Created by Stephan on 11.03.2022.
//

#ifndef LAB3_TABLE_CREATION_H
#define LAB3_TABLE_CREATION_H
#include "table_structs.h"
Table * create_table(IndexType1 msize1, IndexType2 msize2);
void free_table(Table * T);
void clear_table(Table * T);
void add_info_dialog(Table * table);
void print_table(const Table table);
#endif //LAB3_TABLE_CREATION_H
