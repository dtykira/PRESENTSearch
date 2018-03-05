#pragma once
#include "Types.h"
//#include <fstream>

u8 substitution(u8 x);
void word64_2_word4(u8* y,u64 x);
void word4_2_word64(u64* y,u8* x);
void word64_2_bit(bool* y,u64 x);
void bit_2_word64(u64* y,bool* x);
u64 permutation(u64 x);
u64 unPermutation(u64 x);
u64 permutationTL(u64 x);