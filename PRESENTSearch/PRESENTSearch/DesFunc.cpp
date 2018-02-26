#include "DesFunc.h"
#include "Types.h"
#include "Tables.h"
#include "GlobalV.h"
#include <stdio.h>
u8 substitution(u8 x){
	return S[x];
}

void word642word8(u8* y,u64 x){
	for(int i=0;i<16;i++){
		*(y+i)=(x>>(60-4*i))&0xf;
	}
}

void word82word64(u64* y,u8* x){
	u64 temp=0;
	for(int i=0;i<15;i++){
		temp+=(*(x+i));
		temp<<=4;
	}
	temp+=(*(x+15));
	*y=temp;
}

void word642bit(bool* y,u64 x){
	for(int i=0;i<64;i++){
		*(y+i)=(x>>(63-i))&0x1;
	}
}

void bit2word64(u64* y,bool* x){
	u64 temp=0;
	for(int i=0;i<63;i++){
		temp+=(*(x+i));
		temp<<=1;
	}
	temp+=(*(x+63));
	*y=temp;
}

u64 permutation(u64 x){
	u64 t=x;
	bool ipbits[64],opbits[64];
	word642bit(ipbits,t);
	for(int i=0;i<64;i++){
		opbits[i]=ipbits[PTable[i]];
	}
	bit2word64(&t,opbits);
	return t;
}

u64 unPermutation(u64 x){
	u64 t=x;
	bool ipbits[64],opbits[64];
	word642bit(ipbits,t);
	for(int i=0;i<64;i++){
		opbits[i]=ipbits[unPTable[i]];
	}
	bit2word64(&t,opbits);
	return t;
}

u64 permutationTL(u64 x){
	u64 t=0;
	for(int i=0;i<8;i++){
		printf("%llx\n\n",(x>>(56-8*i))&0xff);
		printf("%llx\n\n",PTableLookUp[1][1]);
		t^=PTableLookUp[i][(x>>(56-8*i))&0xff];//////////////////////////这里查表查不出
	}
	return t;
}
	