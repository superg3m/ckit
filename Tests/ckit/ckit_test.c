#include "../../ckit.h"

typedef enum CustomTagPool {
	USER_TEMP_SUPER3M = TAG_CKIT_RESERVED_COUNT
} CustomTagPool;

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
	for (u32 i = 0; i < ckit_vector_count(intVector); i++) {
		int element = intVector[i];
		ckit_assert_msg(element == intArray[i], "Error: Incorrect element value\n");
	}

	for (u32 i = 0; i < ckit_vector_count(boolVector); i++) {
		Boolean element = boolVector[i];
		ckit_assert_msg(element == boolArray[i], "Error: Incorrect element value\n");
	}

	for (u32 i = 0; i < ckit_vector_count(charVector); i++) {
		char element = charVector[i];
		ckit_assert_msg(element == charArray[i], "Error: Incorrect element value\n");
	}

	for (u32 i = 0; i < ckit_vector_count(stringVector); i++) {
		char* element = stringVector[i];
		ckit_assert_msg(ckit_str_equal(element, stringArray[i]), "Error: Incorrect element value\n");
	}

	// Test pop
	for (u32 i = 0; i < ckit_vector_count(intVector); i++) {
		int element = intVector[i];
	}

	int before_popped_int = intVector[ckit_vector_count(intVector) - 1];
	int after_popped_int = ckit_vector_pop(intVector);
	int element_to_push_for_int = 10;
	ckit_vector_push(intVector, element_to_push_for_int);

	for (u32 i = 0; i < ckit_vector_count(intVector); i++) {
		int element = intVector[i];
	}

	ckit_assert_msg(before_popped_int == after_popped_int, "Error: popped expected: %d | got %d\n", before_popped_int, after_popped_int);

	char* before_popped_string = stringVector[ckit_vector_count(stringVector) - 1];
	char* after_popped_string = ckit_vector_pop(stringVector);
	char* string_to_push = "TRYING TO PUSH A LITERAL!\n";
	ckit_vector_push(stringVector, string_to_push);

	ckit_assert_msg(ckit_str_equal(before_popped_string, after_popped_string), "Error: Incorrect popped element value\n");

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

	LOG_SUCCESS("All vector tests passed!\n"); 
	return;
}

void inital_operations() {
	String str = ckit_str_create("SOLAR_FLARE");
	LOG_PRINT("String: %s\n", str);
	LOG_PRINT("String Length: %d\n", ckit_str_length(str));

	ckit_str_append_char(str, 'a');
	LOG_PRINT("String: %s\n", str);
	ckit_str_append(str, " | FEEL THE WORLD AROUND ME!");
	LOG_PRINT("String: %s | length: %d | cstrlen: %d\n", str, ckit_str_length(str), ckit_cstr_length(str));
	LOG_PRINT("\n");
	ckit_str_free(str);

	int* int_array = (int*)ckit_alloc(sizeof(int) * 5);
	int* int_array2 = (int*)ckit_alloc(sizeof(int) * 5);
	LOG_ERROR("Element: %d (SHOULD BE ZERO)\n\n", int_array[0]);
	for (int i = 0; i < 5; i++) {
		int_array2[i] = 1432;
	}

	ckit_memory_copy(int_array2, int_array, sizeof(int) * 5, sizeof(int) * 5);
	for (int i = 0; i < 5; i++) {
		ckit_assert_msg(int_array[i] == 1432, "Memory copy is fucked!");
		LOG_SUCCESS("Element: %d\n", int_array[i]);
	}

	LOG_DEBUG("Element: %d\n\n", int_array[0]);

	LOG_FATAL("TESTING\n");
	LOG_ERROR("TESTING\n");
	LOG_WARN("TESTING\n");
	LOG_DEBUG("TESTING\n");
	LOG_SUCCESS("TESTING\n");
	LOG_PRINT("TESTING %s\n\n", str);

	ckit_free(int_array);
	ckit_free(int_array2);


	LOG_SUCCESS("All string tests passed!\n"); 
	return;
}

void middle_ground_opperations() {
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

	CKIT_Arena* frame_boundary_arena = ckit_arena_create(500, "Frame Boundary", 0);
	ckit_arena_push(frame_boundary_arena, Boolean, TAG_CKIT_TEMP);
	ckit_arena_free(frame_boundary_arena);

	// String str_to_be_split = ckit_str_create("They said it couldn't be done. They tried to recite the dark magics to me! THEY DON'T KNOW I WAS THERE WHEN THEY WERE WRITTEN!");
	String* split_strings = ckit_str_split("They said it couldn't be done. They tried to recite the dark magics to me! THEY DON'T KNOW I WAS THERE WHEN THEY WERE WRITTEN!", " ");
	for (u32 i = 0; i < ckit_vector_count(split_strings); i++) {
		LOG_SUCCESS("str #%d | %s\n", i, split_strings[i]);
		ckit_str_free(split_strings[i]);
	}
	ckit_vector_free(split_strings);
}

void hashmap_operations() {
	CKIT_HashMap* name_to_age = ckit_hashmap_create(4, char*, TRUE);

	char* names[5] = {
		"jofhn",
		"john",
		"johhn",
		"johyn",
		"jo234hyn234235325",
	};

	char* value[5] = {
		"10",
		"11",
		"12",
		"13",
		"144353454416231sdfsfdsfs",
	};

	for (int i = 0; i < 5; i++) {
		ckit_hashmap_put(name_to_age, names[i], value[i]);
	}

	for (int i = 0; i < 5; i++) { // growing hashmap is broken
		LOG_DEBUG("(key: %s | value: %s)\n", names[i], (char*)ckit_hashmap_get(name_to_age, names[i]));
		ckit_assert(ckit_hashmap_has(name_to_age, names[i]));
	}

	ckit_hashmap_free(name_to_age);
}

void queue_operations() {
	CKIT_Queue* queue = ckit_queue_create(4, char*, TRUE);
	char* queue_values[] = {"\"1, 2, 3, 4, 5, 6\"", "\"HELLO!\"", "\"HELLO!12345!\"", "\"HEL45!\"", "\"!12345!\"", "\"!6H23dgfa45!\""};
	ckit_enqueue(queue, queue_values[0]);

	char* current_value = NULLPTR;
	current_value = ckit_dequeue(queue);
	LOG_DEBUG("value: %s\n", current_value);

	for (size_t i = 1; i < 4; i++) {
		ckit_enqueue(queue, queue_values[i]);
	}

	for (size_t i = 1; i < 4; i++) {
		char* current_value = NULLPTR;
		current_value = ckit_dequeue(queue);
		LOG_DEBUG("value: %s\n", current_value);
	}

	ckit_enqueue(queue, queue_values[4]);
	current_value = NULLPTR;
	current_value = ckit_dequeue(queue);
	LOG_DEBUG("value: %s\n", current_value);

	ckit_enqueue(queue, queue_values[5]);
	current_value = NULLPTR;
	current_value = ckit_dequeue(queue);
	LOG_DEBUG("value: %s\n", current_value);

	ckit_queue_free(queue);
}

void linked_list_operations() {
	CKIT_LinkedList* linked_list = ckit_linked_list_create(char*, TRUE);
	ckit_linked_list_insert(linked_list, 0, "hello");
	ckit_linked_list_push(linked_list, "hello_sailor1!");
	ckit_linked_list_push(linked_list, "hello_sailor2!");
	ckit_linked_list_push(linked_list, "hello_sailor3!");
	ckit_linked_list_push(linked_list, "hello_sailor4!");
	char* test_str = ckit_linked_list_remove(linked_list, 4).data;
	LOG_DEBUG("list value: %s\n", test_str);
	test_str = ckit_linked_list_pop(linked_list).data;
	LOG_DEBUG("list value: %s\n", test_str);
	test_str = ckit_linked_list_pop(linked_list).data;
	LOG_DEBUG("list value: %s\n", test_str);
	test_str = ckit_linked_list_pop(linked_list).data;
	LOG_DEBUG("list value: %s\n", test_str);
	test_str = ckit_linked_list_pop(linked_list).data;
	LOG_DEBUG("list value: %s\n", test_str);
	ckit_linked_list_free(linked_list);

	CKIT_LinkedList* linked_list_int = ckit_linked_list_create(u32, FALSE);	
	u32 value1[5] = {
		1,
		2,
		3,
		4,
		5,
	};

	ckit_linked_list_insert(linked_list_int, 0, &value1[0]);
	ckit_linked_list_push(linked_list_int, &value1[1]);
	ckit_linked_list_push(linked_list_int, &value1[2]);
	ckit_linked_list_push(linked_list_int, &value1[3]);
	ckit_linked_list_push(linked_list_int, &value1[4]);
	u32* test_u32 = ckit_linked_list_pop(linked_list_int).data;
	LOG_DEBUG("list value: %d\n", *test_u32);
	ckit_free(test_u32);

	test_u32 = ckit_linked_list_remove(linked_list_int, 2).data;
	LOG_DEBUG("list value: %d\n", *test_u32);
	ckit_free(test_u32);

	test_u32 = ckit_linked_list_pop(linked_list_int).data;
	LOG_DEBUG("list value: %d\n", *test_u32);
	ckit_free(test_u32);

	test_u32 = ckit_linked_list_pop(linked_list_int).data;
	LOG_DEBUG("list value: %d\n", *test_u32);
	ckit_free(test_u32);

	test_u32 = ckit_linked_list_pop(linked_list_int).data;
	LOG_DEBUG("list value: %d\n", *test_u32);
	ckit_free(test_u32);
	ckit_linked_list_free(linked_list_int);
}

void stack_operations() {
	CKIT_Stack* stack_int = ckit_stack_create(u32, FALSE);	
	u32 int_value[5] = {
		15,
		25,
		35,
		45,
		55,
	};

	for (int i = 0; i < 5; i++) {
		ckit_stack_push(stack_int, &int_value[i]);
	}

	for (int i = 0; i < 5; i++) {
		u32* value_back = ckit_stack_pop(stack_int);
		LOG_SUCCESS("stack_value #%d: %d\n", i + 1, *value_back);
		ckit_free(value_back);
	}
	ckit_stack_free(stack_int);

	CKIT_Stack* stack_str = ckit_stack_create(char*, TRUE);	
	char* names[5] = {
		"jofhn",
		"john",
		"johhn",
		"johyn",
		"jo234hyn234235325",
	};

	for (int i = 0; i < 5; i++) {
		ckit_stack_push(stack_str, names[i]);
	}

	for (int i = 0; i < 5; i++) {
		char* value_back = ckit_stack_pop(stack_str);
		LOG_SUCCESS("stack_value #%d: %s\n", i + 1, value_back);
	}
	ckit_stack_free(stack_str);
}

int main() {
	ckit_init();
	memory_init();

	test_vector_operations();

	LOG_PRINT("\n");
	inital_operations();


	LOG_PRINT("\n");
	middle_ground_opperations();

	LOG_PRINT("\n");
	hashmap_operations();

	LOG_PRINT("\n");
	queue_operations();

	/*
	CKIT_Lexer lexer;
	ckit_lexer_load_string(&lexer, "int x = 5; char* testing = \"hello\";");

	CKIT_Token* token_stream = ckit_lexer_generate_token_stream(&lexer);
	for (int i = 0; i < ckit_vector_count(token_stream); i++) {
		ckit_lexer_print_token(token_stream[i]);
	}
	*/

	// ckit_lexer_free(&lexer);

	linked_list_operations();
	stack_operations();
	u32 test_overflow = 5;
	u32 max_value =  10;
	LOG_SUCCESS("%d\n", CLAMP(65, test_overflow, max_value));

	int test_int_to_str = 5235;

	String str_test = ckit_str_int_to_str(test_int_to_str);
	LOG_ERROR("AT THE END TEST: %s\n\n", str_test);
	ckit_str_free(str_test);

	String str_between_test = ckit_str_between_delimiters("WOW - ${Hello!}", "${", "}");
	LOG_DEBUG("String_Between: %s\n", str_between_test);
	ckit_str_free(str_between_test);

	ckit_cleanup();
	return 0;
}