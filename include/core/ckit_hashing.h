#include "../../include/core/ckit_string.h"

#define NUM_PRIMES 25
internal const u32 prime_numbers[NUM_PRIMES] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

u32 get_prime_by_index(u32 index);

u64 hash(const char* cstring_to_hash);

void test_hash_collisions();