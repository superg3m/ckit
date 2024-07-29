#include "../../ckg.h"

void test_ckg_memory_operations() {
	// Test ckg_alloc
	int* ptr1 = ckg_alloc(sizeof(int));
	ckg_assert(ptr1 != NULLPTR);
	*ptr1 = 5;
	ckg_assert(*ptr1 == 5);
	ckg_free(ptr1);

	// Test ckg_memory_callocate
	int* ptr2 = ckg_alloc(10 * sizeof(int));
	ckg_assert(ptr2 != NULLPTR);
	for (int i = 0; i < 10; i++) {
		ptr2[i] = i;
	}
	for (int i = 0; i < 10; i++) {
		ckg_assert(ptr2[i] == i);
	}
	ckg_free(ptr2);

	// Test ckg_realloc
	int* ptr3 = ckg_alloc(sizeof(int));
	int* ptr4 = ckg_realloc(ptr3, 2 * sizeof(int), 4 * sizeof(int));
	ckg_assert(ptr4 != NULLPTR);
	*ptr4 = 5;
	ckg_assert(*ptr4 == 5);
	ckg_free(ptr4);

	// Test ckg_ckg_memory_copy
	int arr1[] = {1, 2, 3};
	int arr2[3];
	ckg_memory_copy(arr2, arr1, 3 * sizeof(int), 3 * sizeof(int));
	ckg_assert(arr1[0] == arr2[0] && arr1[1] == arr2[1] && arr1[2] == arr2[2]);

	// Test ckg_ckg_memory_zero
	int arr3[3] = {1, 2, 3};
	ckg_memory_zero(arr3, 3 * sizeof(int));
	ckg_assert(arr3[0] == 0 && arr3[1] == 0 && arr3[2] == 0);

	// Test ckg_memory_compare
	int arr4[] = {1, 2, 3};
	int arr5[] = {1, 2, 3};
	int arr6[] = {4, 5, 6};
	ckg_assert(ckg_memory_compare(arr4, arr5, 3 * sizeof(int), 3 * sizeof(int)) == TRUE);
	ckg_assert(!ckg_memory_compare(arr4, arr6, 3 * sizeof(int), 3 * sizeof(int)));

	int test_data[] = {1, 2, 3, 12, 22, 23, 41, 52, 73};
	ckg_memory_delete_index(test_data, 9, 0);
	CKG_LOG_PRINT("[ ");
	for (int i = 0; i < 8; i++) {
		CKG_LOG_PRINT("%d ", test_data[i]);
	}
	CKG_LOG_PRINT("]\n");

	ckg_memory_delete_index(test_data, 9, 5);
	CKG_LOG_PRINT("[ ");
	for (int i = 0; i < 7; i++) {
		CKG_LOG_PRINT("%d ", test_data[i]);
	}
	CKG_LOG_PRINT("]\n");

	ckg_memory_delete_index(test_data, 9, 2);
	CKG_LOG_PRINT("[ ");
	for (int i = 0; i < 6; i++) {
		CKG_LOG_PRINT("%d ", test_data[i]);
	}
	CKG_LOG_PRINT("]\n");

	ckg_memory_delete_index(test_data, 9, 5);
	CKG_LOG_PRINT("[ ");
	for (int i = 0; i < 5; i++) {
		CKG_LOG_PRINT("%d ", test_data[i]);
	}
	CKG_LOG_PRINT("]\n");

	ckg_memory_insert_index(test_data, 9, 15161, 0);
	CKG_LOG_PRINT("[ ");
	for (int i = 0; i < 6; i++) {
		CKG_LOG_PRINT("%d ", test_data[i]);
	}
	CKG_LOG_PRINT("]\n");

	ckg_memory_insert_index(test_data, 9, 51212, 6);
	CKG_LOG_PRINT("[ ");
	for (int i = 0; i < 7; i++) {
		CKG_LOG_PRINT("%d ", test_data[i]);
	}
	CKG_LOG_PRINT("]\n");

	CKG_LOG_SUCCESS("All memory tests passed!\n");
	return;
}