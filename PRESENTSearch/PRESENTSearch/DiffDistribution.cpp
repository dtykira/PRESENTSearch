#include "DiffDistribution.h"
#include "GlobalV.h"
#include "DesFunc.h"
#include <stdio.h>

void genDiffTable(){
	DDT_int[0][0]=16;
	for(int base=0;base<16;base++){
		for(int diff=1;diff<16;diff++){
			DDT_int[diff][substitution(base)^substitution(base^diff)]++;
		}
	}
}

void printDiffTable(){
	for(int i=0;i<16;i++){
		for(int j=0;j<16;j++){
			printf("%d\t",DDT_int[i][j]);
		}
		printf("\n");
	}
}