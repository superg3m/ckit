#include "../../include/core/ckit_hashing.h"
#include "../../include/core/ckit_memory.h"
#include <stdlib.h>
#include <time.h>

#define NUM_TESTS 100
#define STRING_LENGTH 14

u32 get_prime_by_index(u32 index) {
	return prime_numbers[index % NUM_PRIMES];
}

u64 hash(const char* cstring_to_hash) {
	u64 hash = 0;
	u64 cstr_length = cstring_length(cstring_to_hash);
	for (int i = 0; i < cstr_length; i++) {
		uint32_t prime = get_prime_by_index(i);
		hash += (((u64)cstring_to_hash[i]) << prime) * prime * cstr_length;
	}

 	return hash;
}

internal void rand_str(char *dest, size_t length) {
    srand(time(NULL));
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

void test_collisions() {
    u8* hash_map = memory_allocate(NUM_TESTS * sizeof(u8), MEMORY_TAG_TEMPORARY);

    char test_string[STRING_LENGTH + 1];

    s32 collisions = 0;

    for (s32 i = 0; i < NUM_TESTS; i++) {
        rand_str(test_string, STRING_LENGTH);
        u64 h = hash(test_string);

        // Check for collision
        if (hash_map[h % NUM_TESTS] != 0) {
            collisions++;
        }
        hash_map[h % NUM_TESTS]++;
    }

    memory_free(hash_map);

    LOG_PRINT("Number of collisions: %d\n", collisions);
    LOG_PRINT("Collision rate: %f\%\n", ((double)collisions / (double)NUM_TESTS) * 100);
}
