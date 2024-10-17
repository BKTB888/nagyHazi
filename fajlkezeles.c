//
// Created by bktivadar on 11/25/23.
//
#include <stdio.h>
#include <string.h>
#include "debugmalloc.h"
#include "jatek.h"
static char *path(char *filename) {
    // Construct the full path
    char *filepath=malloc(strlen(filename)+sizeof("./saves/.txt")+1); // név, útvonal
    snprintf(filepath, strlen(filepath)+1, "./saves/%s.txt", filename);
    return filepath;
}
void mentes(char *filename, sudoku *sudoku1){
    unsigned size=sudoku1->size;
    unsigned **table=sudoku1->table;
    FILE *file;
    char *filepath=path(filename);

    // Open the file for writing
    file = fopen(filepath, "w");
    free(filepath);
    // Check if the file was opened successfully
    if (file != NULL) {
        fprintf(file,"%d\n",size);
        for (unsigned i = 0; i < size; ++i) {
            for (unsigned j = 0; j < size; ++j) {
                fprintf(file,"%d;",table[j][i]);
            }
            fprintf(file,"\n");
        }
        printf("File saved successfully!\n");
        fclose(file);
    }
    else
        perror("File couldn't be saved (mentes)\n");
}


sudoku *open(char *filename){
    FILE* file;
    char *filepath=path(filename);
    file = fopen(filepath, "r");
    free(filepath);
    if (file != NULL) {
        unsigned size;
        fscanf(file,"%d",&size);
        sudoku *sudoku1=table_init(size);
        sudoku1->size=size;
        unsigned **table=sudoku1->table;
        for (unsigned i=0;i<size;++i)
            for (unsigned j=0;j<size;++j)
                fscanf(file,"%d;",&table[i][j]);
        fclose(file);
        return sudoku1;
    }
    else {
        perror("File couldn't be opened (open)\n");
        return NULL;
    }
}

