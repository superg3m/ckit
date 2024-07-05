#include "../../include/core/ckit_hashing.h"
#include "../../include/core/ckit_memory.h"
#include <stdlib.h>
#include <time.h>

#define NUM_TESTS 500
#define STRING_LENGTH 14

u32 get_prime_by_index(u32 index) {
	return prime_numbers[index % NUM_PRIMES];
}

u64 hash(unsigned char *str) {
    u64 hash = 5381;
    int c;

    u32 str_length = ckg_cstr_length(str);

    while (c = *str++) {
        hash = (((hash << 5) + hash) + c) + str_length;
    }

    return hash;
}

internal void rand_str(char *dest, size_t length) {
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        u64 index = rand() % (sizeof(charset) - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

void test_collisions() {
    srand(time(NULL));

    char test_strings[STRING_LENGTH + 1];

    u8* hash_map = memory_allocate(NUM_TESTS * sizeof(u8), MEMORY_TAG_TEMPORARY);

    char test_string[STRING_LENGTH + 1];

    s32 collisions = 0;

    for (s32 i = 0; i < NUM_TESTS; i++) {
        rand_str(test_string, STRING_LENGTH);
        u64 h = hash(test_string);

        u32 hash_index = h % NUM_TESTS;

        // Check for collision
        if (hash_map[hash_index] != 0) {
            collisions++;
        }
        hash_map[h % NUM_TESTS]++;
    }

    memory_free(hash_map);

    LOG_PRINT("Number of collisions: %d / %d\n", collisions, NUM_TESTS);
    LOG_PRINT("Collision rate: %f%%\n", ((double)collisions / (double)NUM_TESTS) * 100);
}
