//
// Created by Bánfalvi Katalin on 2023. 11. 22..
//

#ifndef SODOKU2_JATEK_H
#define SODOKU2_JATEK_H
#include <stdbool.h>

typedef struct sudoku{
    unsigned size;
    unsigned **table;
}sudoku;
typedef struct pos{
    unsigned row, col;
}pos;
sudoku *table_init(unsigned size);
void table_dest(sudoku *sudoku1);
void rand_fill(sudoku *sudoku1);
bool checker(unsigned num, unsigned row, unsigned col,sudoku *sudoku1);
#endif //SODOKU2_JATEK_H
