//#pragma once

void genDiffTable();
void printDiffTable();
extern int DDT_int[16][16];
extern int DDT[16][16];
extern u8 DDT_SearchInOrderX[2][128];
extern u8 DDT_SearchInOrderY[2][128];
extern int DDT_SearchInOrderLength[2];
extern u8 DDT_SearchInOrderWithFixedX[2][16][8];
extern u8 DDT_SearchInOrderWithFixedXLength[2][16];
extern u8 DDT_SearchInOrderWithFixedY[2][16][8];
extern u8 DDT_SearchInOrderWithFixedYLength[2][16];