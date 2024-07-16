#include "../ckit.h"

void test_vector_operations() {
	// Test types
	int intArray[] = {1, 2, 3, 4, 5};
	Boolean boolArray[] = {TRUE, FALSE, TRUE, TRUE, FALSE};
	char charArray[] = {'a', 'b', 'c', 'd', 'e'};
	char* stringArray[] = {"Hello", "World", "!", "OpenAI", "GPT-3"};

	// Create vector of int
	int* intVector = ckit_vector_reserve(5, int);
	for (int i = 0; i < 5; i++) {
		ckit_vector_push(intVector, intArray[i]);
	}

	// Create vector of bool
	Boolean* boolVector = ckit_vector_reserve(5, Boolean);
	for (int i = 0; i < 5; i++) {
		ckit_vector_push(boolVector, boolArray[i]);
	}

	// Create vector of char
	char* charVector = ckit_vector_reserve(5, char);
	for (int i = 0; i < 5; i++) {
		ckit_vector_push(charVector, charArray[i]);
	}

	// Create vector of strings
	char** stringVector = NULLPTR;
	for (int i = 0; i < 5; i++) {
		ckit_vector_push(stringVector, stringArray[i]);
	}

	// Test get
	for (int i = 0; i < ckit_vector_count(intVector); i++) {
		int element = intVector[i];
		ckit_assert_msg(element == intArray[i], "Error: Incorrect element value\n");
	}

	for (int i = 0; i < ckit_vector_count(boolVector); i++) {
		Boolean element = boolVector[i];
		ckit_assert_msg(element == boolArray[i], "Error: Incorrect element value\n");
	}

	for (int i = 0; i < ckit_vector_count(charVector); i++) {
		char element = charVector[i];
		ckit_assert_msg(element == charArray[i], "Error: Incorrect element value\n");
	}

	for (int i = 0; i < ckit_vector_count(stringVector); i++) {
		char* element = stringVector[i];
		ckit_assert_msg(ckg_cstr_equal(element, stringArray[i]), "Error: Incorrect element value\n");
	}

	// Test pop
	for (int i = 0; i < ckit_vector_count(intVector); i++) {
		int element = intVector[i];
	}

	int before_popped_int = intVector[ckit_vector_count(intVector) - 1];
	int after_popped_int = ckit_vector_pop(intVector);
	int element_to_push_for_int = 10;
	ckit_vector_push(intVector, element_to_push_for_int);

	for (int i = 0; i < ckit_vector_count(intVector); i++) {
		int element = intVector[i];
	}

	ckit_assert_msg(before_popped_int == after_popped_int, "Error: popped expected: %d | got %d\n", before_popped_int, after_popped_int);

	char* before_popped_string = stringVector[ckit_vector_count(stringVector) - 1];
	char* after_popped_string = ckit_vector_pop(stringVector);
	char* string_to_push = "TRYING TO PUSH A LITERAL!\n";
	ckit_vector_push(stringVector, string_to_push);

	ckit_assert_msg(ckg_cstr_equal(before_popped_string, after_popped_string), "Error: Incorrect popped element value\n");

	// Test free
	ckit_vector_free(intVector);
	ckit_vector_free(boolVector);
	ckit_vector_free(charVector);
	ckit_vector_free(stringVector);

	// Assert that the data is NULL
	ckit_assert_msg(intVector == NULLPTR, "Error: Vector data is not NULL\n");
	ckit_assert_msg(boolVector == NULLPTR, "Error: Vector data is not NULL\n");
	ckit_assert_msg(charVector == NULLPTR, "Error: Vector data is not NULL\n");
	ckit_assert_msg(stringVector == NULLPTR, "Error: Vector data is not NULL\n");

	LOG_INFO("All vector tests passed!\n"); 
	return;
}

void test_string_operations() {
	// Test types


	LOG_INFO("All string tests passed!\n"); 
	return;
}

int main() {
	ckit_init();
	// memory_init();
	test_string_operations();
	test_vector_operations();
	LOG_PRINT("\n");

	String str = ckit_str_create("SOLAR_FLARE");
	LOG_PRINT("String: %s\n", str);
	LOG_PRINT("String Length: %d\n", ckit_str_length(str));

	ckit_str_append_char(str, 'a');
	LOG_PRINT("String: %s\n", str);
	ckit_str_append(str, " | FEEL THE WORLD AROUND ME!");
	LOG_PRINT("String: %s | length: %d | cstrlen: %d\n", str, ckit_str_length(str), ckit_cstr_length(str));
	LOG_PRINT("\n");
	ckit_str_free(str);

	int* int_array = (int*)ckit_alloc(sizeof(int) * 5, MEMORY_TAG_TEMPORARY);
	int* int_array2 = (int*)ckit_alloc(sizeof(int) * 5, MEMORY_TAG_TEMPORARY);
	LOG_ERROR("Element: %d (SHOULD BE ZERO)\n\n", int_array[0]);
	for (int i = 0; i < 5; i++) {
		int_array2[i] = 1432;
	}

	ckg_memory_copy(int_array2, int_array, sizeof(int) * 5, sizeof(int) * 5);
	for (int i = 0; i < 5; i++) {
		ckit_assert_msg(int_array[i] == 1432, "Memory copy is fucked!");
		LOG_INFO("Element: %d\n", int_array[i]);
	}

	LOG_DEBUG("Element: %d\n\n", int_array[0]);

	LOG_FATAL("TESTING\n");
	LOG_ERROR("TESTING\n");
	LOG_WARN("TESTING\n");
	LOG_DEBUG("TESTING\n");
	LOG_INFO("TESTING\n");
	LOG_PRINT("TESTING %s\n\n", str);

	ckit_free(int_array);
	ckit_free(int_array2);

	Vec2 points[4];
	points[0] = (Vec2){0, 0};
	points[1] = (Vec2){1, 2};
	points[2] = (Vec2){3, 0};
	points[3] = (Vec2){4, 2};

	Vec2 ret = vec2_spline_point(points, 4, 0.5f); // has memory leak with vector
	LOG_ERROR("FINAL POINT: (%f, %f)\n", ret.x, ret.y);


	FileSystem file_system = file_system_create("../../assets/Learn_About_BMP.bmp");
	file_open(&file_system);

	u32 offset = 0; 

	BmpHeader bmp_header;
	ckit_memory_copy(file_system.data, &bmp_header, sizeof(bmp_header), file_system.file_size);
	const u32 bytes_per_pixel = 4;
	size_t bitmap_size = (bmp_header.width * bmp_header.height) * bytes_per_pixel;
	u8* bitmap_data = file_system.data + bmp_header.data_offset;

	file_close(&file_system);

	test_hash_collisions();

	CKIT_Arena* frame_boundary_arena = arena_create(500, "Frame Boundary");
	arena_free(frame_boundary_arena);

	ckit_cleanup();
	return 0;
}