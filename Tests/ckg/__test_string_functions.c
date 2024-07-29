#include "../../ckg.h"

void test_ckg_cstr_length() {
    const int expected_size = 5;
    char str1[] = "Hello";
    int actual_size = ckg_cstr_length(str1);
    ckg_assert_msg(actual_size == expected_size, "Test ckg_cstr_length failed: expected %d, got %d\n", expected_size, actual_size);

    char str2[] = "";
    ckg_assert(ckg_cstr_length(str2) == 0);

    char str3[] = "Hello World";
    ckg_assert(ckg_cstr_length(str3) == 11);

    char str4[] = "Hi!\n";
    ckg_assert(ckg_cstr_length(str4) == 4);

    CKG_LOG_SUCCESS("Test ckg_cstr_length passed.\n");
}

void test_ckg_string_equal() {
    char str1[] = "Hello";
    char str2[] = "Hello";
    char str3[] = "World";
    ckg_assert(ckg_cstr_equal(str1, str2) && !ckg_cstr_equal(str1, str3));

    char str4[] = "";
    ckg_assert(!ckg_cstr_equal(str1, str4));

    ckg_assert(ckg_cstr_equal(str4, str4));

    char str5[] = "Hello ";
    ckg_assert(!ckg_cstr_equal(str1, str5));

    CKG_LOG_SUCCESS("Test ckg_cstr_equal passed.\n");
}

void test_ckg_cstr_insert_char() {
    #define STR_CAP1 7
    char str1[STR_CAP1] = "Hello";
    ckg_cstr_insert_char(str1, STR_CAP1, 'V', 2);
    ckg_assert_msg(ckg_cstr_equal(str1, "HeVllo"), "Test: ckg_cstr_insert_char failed -> got: %s | expected: %s\n", str1, "HeVllo");

    ckg_cstr_copy(str1, STR_CAP1, "Hello");
    ckg_cstr_insert_char(str1, STR_CAP1, 'X', 0);
    ckg_assert(ckg_cstr_equal(str1, "XHello"));

    ckg_cstr_copy(str1, STR_CAP1, "Hello");
    ckg_cstr_insert_char(str1, STR_CAP1, 'Y', 5);
    ckg_assert(ckg_cstr_equal(str1, "HelloY"));

    char str2[STR_CAP1] = "";
    ckg_cstr_insert_char(str2, STR_CAP1, 'Z', 0);
    ckg_assert(ckg_cstr_equal(str2, "Z"));

    CKG_LOG_SUCCESS("Test ckg_cstr_insert_char passed.\n");
}

void test_ckg_cstr_insert() {
    #define STR_CAP2 16
    char str1[STR_CAP2] = "Hello";
    ckg_cstr_insert(str1, STR_CAP2, "|TESTINGS|", 2);
    ckg_assert(ckg_cstr_equal(str1, "He|TESTINGS|llo"));

    ckg_cstr_copy(str1, STR_CAP2, "Hello");
    ckg_cstr_insert(str1, STR_CAP2, "Start-", 0);
    ckg_assert(ckg_cstr_equal(str1, "Start-Hello"));

    ckg_cstr_copy(str1, STR_CAP2, "Hello");
    ckg_cstr_insert(str1, STR_CAP2, "-End", 5);

    ckg_assert_msg(ckg_cstr_equal(str1, "Hello-End"), "Test: ckg_cstr_insert failed at end s1: %s\n", str1);

    char str2[STR_CAP2] = "";
    ckg_cstr_insert(str2, STR_CAP2, "Inserted", 0);
    ckg_assert(ckg_cstr_equal(str2, "Inserted"));

    CKG_LOG_SUCCESS("Test ckg_cstr_insert passed.\n");
}

void test_ckg_cstr_append() {
    #define STR_CAP3 13
    char str1[STR_CAP3] = {0};
    ckg_cstr_copy(str1, STR_CAP3, "Hello");
    ckg_cstr_append(str1, STR_CAP3, " World!");
    ckg_assert(ckg_cstr_equal(str1, "Hello World!"));

    char str2[STR_CAP3] = "";
    ckg_cstr_append(str2, STR_CAP3, "Appended");
    ckg_assert(ckg_cstr_equal(str2, "Appended"));

    ckg_cstr_copy(str1, STR_CAP3, "Hello");
    ckg_cstr_append(str1, STR_CAP3, "");
    ckg_assert(ckg_cstr_equal(str1, "Hello"));

    CKG_LOG_SUCCESS("Test ckg_cstr_append passed.\n");
}

void test_ckg_cstr_append_char() {
    char str1[50] = {0};
    ckg_cstr_copy(str1, 50, "Hello");
    ckg_cstr_append_char(str1, 50, '!');
    ckg_assert(ckg_cstr_equal(str1, "Hello!"));

    char str2[50] = "";
    ckg_cstr_append_char(str2, 50, 'A');
    ckg_assert(ckg_cstr_equal(str2, "A"));

    ckg_cstr_copy(str1, 50, "Hi");
    ckg_cstr_append_char(str1, 50, ' ');
    ckg_cstr_append_char(str1, 50, 'A');
    ckg_cstr_append_char(str1, 50, '!');
    ckg_assert(ckg_cstr_equal(str1, "Hi A!"));

    CKG_LOG_SUCCESS("Test ckg_cstr_append_char passed.\n");
}

void test_ckg_cstr_clear() {
    char str1[50] = "Hello";
    ckg_cstr_clear(str1);
    ckg_assert(ckg_cstr_equal(str1, ""));

    char str2[50] = "";
    ckg_cstr_clear(str2);
    ckg_assert(ckg_cstr_equal(str2, ""));

    CKG_LOG_SUCCESS("Test ckg_cstr_clear passed.\n");
}

void test_ckg_cstr_copy() {
    char str1[50] = {0};
    ckg_cstr_copy(str1, 50, "Hello!");
    ckg_assert(ckg_cstr_equal(str1, "Hello!"));

    CKG_LOG_SUCCESS("Test ckg_cstr_copy passed.\n");
}

void test_ckg_cstr_contains() {
    char* t1 = "hello";
    char* sub_str = ckg_alloc(1);
    ckg_substring(t1, sub_str, 0, 1);
    ckg_assert(ckg_cstr_contains(sub_str, "he"));
    ckg_free(sub_str);

    ckg_assert(!ckg_cstr_contains(t1, ""));
    ckg_assert(ckg_cstr_contains(t1, "h"));
    ckg_assert(ckg_cstr_contains(t1, "he"));
    ckg_assert(ckg_cstr_contains(t1, "o"));
    ckg_assert(!ckg_cstr_contains(t1, ";;;;;;"));
    ckg_assert(ckg_cstr_contains(t1, "hello"));
    ckg_assert(!ckg_cstr_contains(t1, "hllo"));

    CKG_LOG_SUCCESS("Test ckg_cstr_contains passed.\n");
}

void test_ckg_cstr_starts_with() {
    char* t1 = "hello";
	char* sub_str = ckg_alloc(1);
    ckg_substring(t1, sub_str, 0, 1);
	ckg_assert(ckg_cstr_starts_with(sub_str, "he"));
	ckg_free(sub_str);

	ckg_assert(ckg_cstr_starts_with(t1, "hell"));
	ckg_assert(ckg_cstr_starts_with(t1, "hello"));
	ckg_assert(!ckg_cstr_starts_with(t1, "hllo"));
	ckg_assert(!ckg_cstr_starts_with(t1, ""));


	char* t2 = "";
	ckg_assert(ckg_cstr_starts_with(t2, ""));
	

	t2 = "f";
	ckg_assert(!ckg_cstr_starts_with(t2, "g"));
	ckg_assert(ckg_cstr_starts_with(t2, "f"));

    CKG_LOG_SUCCESS("Test ckg_cstr_starts_with passed.\n");
}

void test_ckg_cstr_ends_with() {
    char* t1 = "hello";
	char* sub_str = ckg_alloc(1);
    ckg_substring(t1, sub_str, 0, 1);
	ckg_assert(ckg_cstr_ends_with(sub_str, "he"));
	ckg_assert(!ckg_cstr_ends_with(sub_str, "llo"));
	ckg_free(sub_str);

	ckg_assert(ckg_cstr_ends_with(t1, ""));
	ckg_assert(ckg_cstr_ends_with(t1, "lo"));
	ckg_assert(ckg_cstr_ends_with(t1, "hello"));
	ckg_assert(ckg_cstr_ends_with(t1, "ello"));

	char* t2 = "";
	ckg_assert(ckg_cstr_ends_with(t2, ""));
	

	t2 = "f";
	ckg_assert(!ckg_cstr_ends_with(t2, "g"));
	ckg_assert(ckg_cstr_ends_with(t2, "f"));

    CKG_LOG_SUCCESS("Test ckg_cstr_ends_with passed.\n");
}

void test_ckg_cstr_reverse() {
    char* t1 = "hello";
	char* sub_str = ckg_alloc(1);
    ckg_substring(t1, sub_str, 0, 1);
    char* reversed_string = ckg_alloc(ckg_cstr_length(sub_str) + 1);
    ckg_cstr_reverse(sub_str, reversed_string, ckg_cstr_length(sub_str) + 1);
	ckg_assert(ckg_cstr_equal(reversed_string, "eh"));
	ckg_free(sub_str);
	ckg_free(reversed_string);

    char* reversed_string2 = ckg_alloc(ckg_cstr_length("Chicken") + 1);
    ckg_cstr_reverse("Chicken", reversed_string2, ckg_cstr_length("Chicken") + 1);
    char* reversed_string3 = ckg_alloc(ckg_cstr_length("Roast") + 1);
    ckg_cstr_reverse("Roast", reversed_string3, ckg_cstr_length("Chicken") + 1);
    char* reversed_string4 = ckg_alloc(ckg_cstr_length("Soup") + 1);
    ckg_cstr_reverse("Soup", reversed_string4, ckg_cstr_length("Chicken") + 1);

	ckg_assert_msg(ckg_cstr_equal(reversed_string2, "nekcihC"), "test_ckg_cstr_reverse failed expected: %s | got: %s", "nekcihC", reversed_string2);
	ckg_assert_msg(ckg_cstr_equal(reversed_string3, "tsaoR"), "test_ckg_cstr_reverse failed expected: %s | got: %s", "tsaoR", reversed_string3);
	ckg_assert_msg(ckg_cstr_equal(reversed_string4, "puoS"), "test_ckg_cstr_reverse failed expected: %s | got: %s", "puoS", reversed_string4);

	ckg_free(reversed_string2);
	ckg_free(reversed_string3);
	ckg_free(reversed_string4);


	char* t2 = "";
    char* reversed_t2 = ckg_alloc(ckg_cstr_length(t2) + 1);
    ckg_cstr_reverse(t2, reversed_t2, ckg_cstr_length(t2)+ 1);
	ckg_assert_msg(ckg_cstr_equal(reversed_t2, ""), "test_ckg_cstr_reverse failed expected: %s | got: %s", "", reversed_t2);
    ckg_free(reversed_t2);

	t2 = "f";
    reversed_t2 = ckg_alloc(ckg_cstr_length(t2) + 1);
    ckg_cstr_reverse(t2, reversed_t2, ckg_cstr_length(t2) + 1);
	ckg_assert_msg(ckg_cstr_equal(reversed_t2, "f"), "test_ckg_cstr_reverse failed expected: %s | got: %s", "f", reversed_t2);
	ckg_assert_msg(!ckg_cstr_equal(reversed_t2, "g"), "test_ckg_cstr_reverse failed expected: %s | got: %s", "f", reversed_t2);
    ckg_free(reversed_t2);

    CKG_LOG_SUCCESS("Test ckg_cstr_reverse passed.\n");
}

void test_ckg_cstr_index_of() {
    char* t1 = "hello ";
	char* sub_str = ckg_alloc(1);
    ckg_substring(t1, sub_str, 0, 1);

	ckg_assert(ckg_cstr_index_of(sub_str, "he") == 0);
	ckg_free(sub_str);

	ckg_assert(ckg_cstr_index_of(t1, "") == -1);
	ckg_assert(ckg_cstr_index_of(t1, "ell") == 1);
	ckg_assert(ckg_cstr_index_of(t1, "l") == 2);
	ckg_assert(ckg_cstr_index_of(t1, "o") == 4);
	ckg_assert(ckg_cstr_index_of(t1, "7") == -1);
	ckg_assert(ckg_cstr_index_of(t1, "hello") == 0);
	ckg_assert(ckg_cstr_index_of(t1, "hllo") == -1);

	ckg_assert(ckg_cstr_index_of(t1, " ") == 5);

	char* t2 = "";
	ckg_assert(ckg_cstr_index_of(t2, "") == 0);
	

	t2 = "f";
	ckg_assert(ckg_cstr_index_of(t2, "g") == -1);
	ckg_assert(ckg_cstr_index_of(t2, "f") == 0);

    CKG_LOG_SUCCESS("Test test_ckg_cstr_index_of passed.\n");
}

void test_ckg_string_assertions() {
    char str1[10] = "Overflow";
    ckg_cstr_append(str1, 10, "ingTest");
    ckg_assert(!ckg_cstr_equal(str1, "OverflowingTest"));

    char str2[6] = "Short";
    ckg_cstr_append_char(str2, 6, 'X');
    ckg_assert(!ckg_cstr_equal(str2, "ShortX"));
}

void test_ckg_string_edgecases() {
    ckg_assert(ckg_cstr_length("") == 0);
    ckg_assert(ckg_cstr_length(NULL));

    // Test edge cases for ckg_cstr_equal
    ckg_assert(ckg_cstr_equal(NULL, NULL));
    ckg_assert(!ckg_cstr_equal("Hello", NULL));
    ckg_assert(!ckg_cstr_equal(NULL, "Hello"));

    // Test edge cases for ckg_cstr_insert_char
    char str1[50] = "Hello";
    ckg_cstr_insert_char(str1, 50, 'V', 10); // Insert at a position beyond the string length
    ckg_assert(ckg_cstr_equal(str1, "HelloV"));

    char str2[50] = "";
    ckg_cstr_insert_char(str2, 50, 'Z', 0); // Insert into an empty string
    ckg_assert(ckg_cstr_equal(str2, "Z"));

    // Test edge cases for ckg_cstr_append
    char str3[50] = "Hello";
    ckg_cstr_append(str3, 50, NULL); // Append a null string
    ckg_assert(ckg_cstr_equal(str3, "Hello"));

    char str4[50] = "";
    ckg_cstr_append(str4, 50, "Appended"); // Append to an empty string
    ckg_assert(ckg_cstr_equal(str4, "Appended"));

    CKG_LOG_SUCCESS("Test ckg_string_edgecases passed.\n");

}

void test_ckg_str_operations() { 
    CKG_LOG_PRINT("\n");
    CKG_LOG_DEBUG("======================= Testing String Functions =======================\n");
    test_ckg_cstr_length();
    test_ckg_string_equal();
    test_ckg_cstr_insert_char();
    test_ckg_cstr_insert();
    test_ckg_cstr_append();
    test_ckg_cstr_append_char();
    test_ckg_cstr_clear();
    test_ckg_cstr_copy();
    test_ckg_cstr_contains();
    test_ckg_cstr_starts_with();
    test_ckg_cstr_ends_with();
    test_ckg_cstr_reverse();
    test_ckg_cstr_index_of();

    int test_int_to_cstr = 5112512;
    char* string_buffer = ckg_alloc(9);
    ckg_cstr_int_to_cstr(string_buffer, 9, test_int_to_cstr);
    CKG_LOG_SUCCESS("INT TO STRING: %s\n", string_buffer);

    // test_ckg_string_edgecases();
    // test_ckg_string_assertions();
    CKG_LOG_DEBUG("======================= String Functions All Passed =======================\n");
    CKG_LOG_PRINT("\n");
}