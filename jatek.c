//
// Created by Bánfalvi Katalin on 2023. 11. 22..
//
#include <stdbool.h>
#include <math.h>
#include "debugmalloc.h"
#include "jatek.h"


sudoku *table_init(unsigned size){
    sudoku *sudoku1=malloc(sizeof(sudoku));
    unsigned **table;
    table = (unsigned**) malloc(size * sizeof(unsigned*));
    table[0]= (unsigned*) calloc(size * size, sizeof(unsigned));
    if (table != NULL){
        for (unsigned i=1; i<size;++i){
            table[i]= table[0] + i * size;
        }
        sudoku1->size=size;
        sudoku1->table=table;
        return sudoku1;
    }
    else{
        printf("nem sikerült a foglalás (init)");
        return NULL;
    }
}
void table_dest(sudoku *sudoku1){
    free(sudoku1->table[0]);
    free(sudoku1->table);
    free(sudoku1);
}
bool checker(const unsigned num, const unsigned row, const unsigned col,sudoku *sudoku1){//true if the number can be placed at a location
    unsigned size=sudoku1->size;
    unsigned **table=sudoku1->table;
    if (table[row][col]!=0)
        return false;

    for (unsigned i=0; i < size; ++i){ //van-e már az oszlopban ilyen szám
        if (table[i][col] == num)
            return false;
    }

    for (unsigned i=0; i < size; ++i){ //van-e már az sorban ilyen szám
        if (table[row][i] == num)
            return false;
    }
    size=(unsigned)sqrt(size);//size of the sub squares
    for (unsigned i=0;i<size;++i){
        for (unsigned j=0;j<size;j++){
            if (table[row - (row % size) + i][col - (col % size) + j] == num)
                return false;
        }
    }
    return true;
}

static bool rand_fill_req(sudoku *sudoku1, unsigned row, unsigned col, unsigned num){//start col and row at 0
    unsigned **table=sudoku1->table;
    unsigned size=sudoku1->size;

    if (row==size)
        return true;
    while(table[row][col]!=0){
        ++col;
        if (col==size){
            col=0;
            ++row;
        }
        if (row==size)
            return true;
    }
    for (unsigned i=0;i<size;++i){
        bool right_num=checker(num, row, col, sudoku1);
        if (right_num){
            table[row][col]=num;

            unsigned new_col=col+1;
            unsigned new_row=row;
            if (new_col==size){
                new_col=0;
                ++new_row;
            }
            if (!rand_fill_req(sudoku1, new_row, new_col, rand() % size + 1)){
                table[row][col]=0;
                right_num=false;
            }
            else
                return true;
        }
        if (!right_num){
            ++num;
            if (num>size)
                num=1;
        }
    }
    return false;
}
void rand_fill(sudoku *sudoku1){
    if(!rand_fill_req(sudoku1, 0, 0, rand() % sudoku1->size + 1)){
        printf("tried impossible combination\n");
    }
}