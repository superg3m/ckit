#include "./ckit_hashing.h"

#include "../../Memory/ckit_memory.h"
#include "../../String/ckit_string.h"
#include <stdlib.h>
#include <time.h>

#define NUM_TESTS 500
#define STRING_LENGTH 14

#define NUM_PRIMES 25
internal const u32 prime_numbers[NUM_PRIMES] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

u32 get_prime_by_index(u32 index) {
	return prime_numbers[index % NUM_PRIMES];
}

u64 hash(unsigned char *str) {
    u64 hash = 5381;
    int c;

    u32 str_length = ckit_cstr_length(str);

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

void test_hash_collisions() {
    srand(time(NULL));

    char test_strings[STRING_LENGTH + 1];

    u8* hash_map = ckit_alloc(NUM_TESTS * sizeof(u8), MEMORY_TAG_TEMPORARY);

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

    ckit_free(hash_map);

    LOG_PRINT("Number of collisions: %d / %d\n", collisions, NUM_TESTS);
    LOG_PRINT("Collision rate: %f%%\n", ((double)collisions / (double)NUM_TESTS) * 100);
}
