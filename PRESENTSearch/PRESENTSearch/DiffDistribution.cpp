#include "DiffDistribution.h"
#include "GlobalV.h"
#include "DesFunc.h"
#include <stdio.h>

int DDT_int[16][16]={0};
int DDT[16][16]={0};
u8 DDT_SearchInOrderX[2][128]={0};
u8 DDT_SearchInOrderY[2][128]={0};
int DDT_SearchInOrderLength[2]={0};
u8 DDT_SearchInOrderWithFixedX[2][16][8]={0};
u8 DDT_SearchInOrderWithFixedXLength[2][16]={0};
u8 DDT_SearchInOrderWithFixedY[2][16][8]={0};
u8 DDT_SearchInOrderWithFixedYLength[2][16]={0};

void genDiffTable(){
	DDT_int[0][0]=16;
	for(int base=0;base<16;base++){
		for(int diff=1;diff<16;diff++){
			DDT_int[diff][substitution(base)^substitution(base^diff)]++;
		}
	}
	int temp,freq;
	for(int i=0;i<16;i++){
		for(int j=0;j<16;j++){
			temp=DDT_int[i][j];
			if(temp==2){
				DDT[i][j]=-3;
				freq=1;
			}else if(temp==4){
				DDT[i][j]=-2;
				freq=0;
			}
			if(temp!=0&&temp!=16){
				DDT_SearchInOrderX[freq][DDT_SearchInOrderLength[freq]]=i;
				DDT_SearchInOrderY[freq][DDT_SearchInOrderLength[freq]]=j;
				DDT_SearchInOrderLength[freq]++;
				DDT_SearchInOrderWithFixedX[freq][i][DDT_SearchInOrderWithFixedXLength[freq][i]]=j;
				DDT_SearchInOrderWithFixedXLength[freq][i]++;
				DDT_SearchInOrderWithFixedY[freq][j][DDT_SearchInOrderWithFixedYLength[freq][j]]=i;
				DDT_SearchInOrderWithFixedYLength[freq][j]++;
			}
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
	for(int i=0;i<2;i++){
		for(int j=0;j<DDT_SearchInOrderLength[i];j++){
			printf("(%x,%x)\t",DDT_SearchInOrderX[i][j],DDT_SearchInOrderY[i][j]);
		}
		printf("\n");
	}
}