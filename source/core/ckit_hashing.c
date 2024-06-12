#include "../../include/core/ckit_hashing.h"

u32 get_prime_by_index(u32 index) {
	return prime_numbers[index % NUM_PRIMES];
}

u64 hash(const char* cstring_to_hash) {
	u64 hash = 0;
	u64 cstr_length = cstring_length(cstring_to_hash);
	for (int i = 0; i < cstr_length; i++) {
		hash += ((u64)cstring_to_hash[i]) * get_prime_by_index(i) * cstr_length;
	}

 	return hash;
}