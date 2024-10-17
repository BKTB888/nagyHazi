//
// Created by bktivadar on 11/22/23.
//
#include "econio.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "jatek.h"
#include "kiiras.h"
#include "debugmalloc.h"
#include "fajlkezeles.h"
static void generalasUI(int difficulty);
static bool ujjatek(void);
static void feltoltesUI();
void menu(void){
    bool exit=false;
    while (!exit){
        econio_rawmode();
        econio_clrscr();
        printf("1. New Game\n"
               "2. Create Own Game\n"
               "3. Continue Game\n"
               "Q. Exit Game\n");
        int key = econio_getch();
        switch (key) {
            case '1':
                if(!ujjatek())
                    exit=true;
                break;
            case '2':
                generalasUI(4);
                break;
            case '3':
                feltoltesUI();
                break;
            case 'Q':
            case 'q':
                exit=true;
                break;
            default:
                break;

        }
    }
}

static bool ujjatek(void){// returns false if it wants to exit
    bool exit=false;
    while (!exit){
        econio_rawmode();
        econio_clrscr();
        printf("1. Easy\n"
               "2. Normal\n"
               "3. Hard\n"
               "B. Back to the Main Menu\n"
               "Q. Exit Game\n");
        int key = econio_getch();
        switch (key) {
            case '1':
                generalasUI(1);
                break;
            case '2':
                generalasUI(2);
                break;
            case '3':
                generalasUI(3);
                break;
            case 'B':
            case 'b':
                exit=true;
                break;
            case 'Q':
            case 'q':
                return false;
            default:
                break;
        }
    }
    return true;
}

static bool negyzetszam(unsigned number);
static void generalas(unsigned size, int difficulty);
static void generalasUI(int difficulty){
    econio_clrscr();
    printf("Type B if you want to go back\n");
    printf("Desired size of the sudoku (WILL BE VERY SLOW FOR ANYTHING ABOVE 16): ");
    unsigned size;
    char str='0';

    while (true) {
        econio_normalmode();
        if (scanf("%d", &size) == 1) {
            if (negyzetszam(size)) {
                generalas(size, difficulty);
                return;
            }
            else
                printf("Can only be a squared number.\nTry again: ");
        } else if (scanf("%c", &str) == 1) {
            if(str=='b' || str=='B')
                return;
        }
    }
}
static bool negyzetszam(unsigned number) {
    if (number==1 || number==0)
        return false;
    double sqrtResult = sqrt((double)number);
    int sqrtIntPart = (int)sqrtResult;

    return sqrtResult == sqrtIntPart;
}

static char *namePicker(void);
static void jatekUI(sudoku *sudoku1);
static void feltoltesUI(){
    econio_normalmode();
    char *filename=namePicker();
    if (filename!=NULL){
        sudoku *sudoku1=open(filename);
        free(filename);
        if (sudoku1!=NULL){
            jatekUI(sudoku1);
            table_dest(sudoku1);
        }
    }
}


static char *namePicker(void){
    econio_clrscr();
    econio_normalmode();
    while (true){
        printf("Name of the File (Shorter then 50 charachters): ");
        char *name=malloc(sizeof(char)*50);
        scanf("%49s",name);
        name=realloc(name,strlen(name)+1);
        bool badName=false;
        for (int i=0;name[i]!='\0';++i){
            if (!((name[i]>='a' && name[i]<='z') || (name[i]>='A' && name[i]<='Z'))){// it returns true if name contains anything but letters
                printf("The name can only contain letters.\n");
                badName=true;
                break;
            }
        }
        if (!badName)
            return name;
    }
}
static void generalas(unsigned const size, int difficulty){
    econio_normalmode();
    bool empty=false;
    unsigned removed;
    sudoku *sudoku1= table_init(size);
    if (difficulty==1)
        removed=size*size/4;
    else if (difficulty==2)
        removed=size*size/3;
    else if (difficulty==3)
        removed=size*size/2;
    else if (difficulty==4)//that means empty sudoku
        empty=true;
    else{
        printf("non valid difficulty. Starting with Easy\n");
        removed=size*size/4;
    }
    if (!empty) {
        rand_fill(sudoku1);
        for (unsigned i = 0; i < removed; ++i) {
            unsigned row;
            unsigned col;
            do {
                row = rand() % size;
                col = rand() % size;
            } while (sudoku1->table[row][col] == 0);
            sudoku1->table[row][col] = 0;
        }
    }
    jatekUI(sudoku1);
    table_dest(sudoku1);
}
static bool inFixedPos(unsigned row, unsigned col,pos *fixedPos, unsigned size);
static void mentesUI(sudoku *sudoku1);
static bool checkWin(sudoku *sudoku1);
static void jatekUI(sudoku *sudoku1){
    bool exit=false;
    unsigned counter=0;
    pos *fixedPos=malloc(0);
    unsigned row, col, num;
    unsigned const size=sudoku1->size;
    unsigned **table=sudoku1->table;
    for (row=0;row<size;++row){
        for (col=0;col<size;++col){
            if(table[row][col]!=0) {
                ++counter;
                fixedPos=(pos*)realloc(fixedPos,counter*sizeof(pos));
                fixedPos[counter-1]=(pos){row,col};
            }
        }
    }
    bool refresh=true;
    while(!exit){
        if (refresh) {
            econio_clrscr();
            printer(sudoku1);
            printf("COMMANDS:\n"
                   "Fill Square: #row.col num   "
                   "Save Game: S    "
                   "Try to Solve: X    "
                   "Back to Menu: B\n");
        }
        char c;
        switch (c=getchar()){
            case '#':
                scanf("%d.%d %d",&row, &col,&num);
                if (row<size && col<size && num<=size && num>=1) {
                    if (inFixedPos(row, col, fixedPos, counter)) {
                        refresh=false;
                        printf("You are trying to overwrite given value.\n");
                    }
                    else {
                        refresh=true;
                        table[row][col] = num;
                        if (checkWin(sudoku1))
                            exit=true;
                    }
                }
                else {
                    refresh = false;
                    printf("Invalid Square\n");
                }
                break;
            case 'S':
            case 's':
                refresh=true;
                mentesUI(sudoku1);
                break;
            case 'X':
            case 'x':
                rand_fill(sudoku1);
                refresh=true;
                break;
            case 'B':
            case 'b':
                exit=true;
                break;
            case '\n':
                refresh=false;
                break;
            default:
                refresh=false;
                printf("Invalid input\n");
        }
    }
    free(fixedPos);
}
static bool inFixedPos(unsigned row, unsigned col,pos *fixedPos, unsigned size){
    for(unsigned i=0;i<size;++i)
        if (fixedPos[i].row==row && fixedPos[i].col==col)
            return true;
    return false;
}
static void mentesUI(sudoku *sudoku1){
    char *name=namePicker();
    mentes(name,sudoku1);
    free(name);
}
static bool checkWin(sudoku *sudoku1){
    unsigned size=sudoku1->size, **table=sudoku1->table;
    for (unsigned row=0; row<size;++row){//check if empty
        for (unsigned col=0; col<size;++col){
            if (table[row][col]==0)
                return false;
        }
    }
    for (unsigned row=0; row<size;++row){//check if empty
        for (unsigned col=0; col<size;++col){
            unsigned num=table[row][col];
            table[row][col]=0;
            if (checker(num,row,col, sudoku1))
                table[row][col]=num;
            else {
                econio_textcolor(COL_RED);
                printf("Made some mistakes.\n");
                econio_textcolor(COL_RESET);
                return false;
            }
        }
    }
    econio_textcolor(COL_GREEN);
    printf("WINNER!");
    econio_textcolor(COL_RESET);
    return true;
}
