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
    free(item->info->data);
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
    table->ks2 = malloc(sizeof(KeySpace2) * table->msize2.index);

    //printf("%ld", sizeof(KeySpace2));

    for (int i = 0; i < msize1.index; ++i) {
        table->ks1[i] = create_KS1(i);
    }

    for (int i = 0; i < msize2.index-1; ++i) {
        table->ks2[i] = create_KS2(i);
        table->ks2[i].next = table->ks2+i+1;
    }
    table->ks2[msize2.index-1] = create_KS2(msize2.index-1);
    table->ks2[msize2.index-1].next = table->ks2;

    return table;
}
