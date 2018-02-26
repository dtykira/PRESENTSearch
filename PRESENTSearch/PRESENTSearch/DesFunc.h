#pragma once
#include "Types.h"
//#include <fstream>

u8 substitution(u8 x);
void word642word8(u8* y,u64 x);
void word82word64(u64* y,u8* x);
void word642bit(bool* y,u64 x);
void bit2word64(u64* y,bool* x);
u64 permutation(u64 x);
u64 unPermutation(u64 x);
u64 permutationTL(u64 x);