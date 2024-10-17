#include <stdio.h>
#include <math.h>
#include "econio.h"
#include "jatek.h"

static void line_print(unsigned szelesseg, int pos){//pos can be 0,1,2 top middle bottom
    unsigned line_len=0;
    for (unsigned i=0;i<szelesseg;++i){
        ++line_len;
    }
    line_len*=2;     //seperators
    --line_len;      //seperators
    char *begin, *middle, *end;
    if (pos==0){
        begin="┌";
        middle="┬";
        end="┐";
    }
    else if (pos==1){
        begin="├";
        middle="┼";
        end="┤";
    }
    else{
        begin="└";
        middle="┴";
        end="┘";
    }
    printf("%s",begin);
    for(unsigned i=1;i<=line_len;++i){
        if (i%(2*(unsigned)sqrt(szelesseg))==0)
            printf("%s",middle);
        else
            printf("─");
    }
    printf("%s\n",end);
}
static unsigned char converter(unsigned n);
void printer(sudoku *sudoku1){
    unsigned **tomb=sudoku1->table;
    unsigned size=sudoku1->size;
    int pos=0;
    for (unsigned i=0; i < size; i++) {

        if (i%(unsigned)sqrt(size) == 0)
            line_print(size, pos);
        printf("│");
        for (unsigned j = 0; j < size; j++) {
            if (tomb[i][j]==0)
                econio_textbackground(COL_LIGHTBLUE);
            printf("%c",converter(tomb[i][j]));
            econio_textbackground(COL_RESET);
            if ((j+1)%(unsigned)sqrt(size) == 0)
                printf("│");
            else
                printf(" ");
        }
        printf("\n");
        pos=1;
    }
    line_print(size, 2);
    printf("\n");
}
static unsigned char converter(unsigned n){
    if (n>26+9)
        return 'A'+(n-36);
    else if (n>9)
        return 'a'+(n-10);
    else
        return (n==0 ? ' ':'0'+n);
}
