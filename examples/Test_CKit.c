#include "../CKit.h"

void test_vector_operations() {
  // Test types
  int intArray[] = {1, 2, 3, 4, 5};
  Boolean boolArray[] = {TRUE, FALSE, TRUE, TRUE, FALSE};
  char charArray[] = {'a', 'b', 'c', 'd', 'e'};
  char* stringArray[] = {"Hello", "World", "!", "OpenAI", "GPT-3"};

  // Create vector of int
  int* intVector = vector_reserve(5, int);
  for (int i = 0; i < 5; i++) {
    vector_push(intVector, intArray[i]);
  }

  // Create vector of bool
  Boolean* boolVector = vector_reserve(5, Boolean);
  for (int i = 0; i < 5; i++) {
    vector_push(boolVector, boolArray[i]);
  }

  // Create vector of char
  char* charVector = vector_reserve(5, char);
  for (int i = 0; i < 5; i++) {
    vector_push(charVector, charArray[i]);
  }

  // Create vector of strings
  char** stringVector = vector_create(char*);
  for (int i = 0; i < 5; i++) {
    vector_push(stringVector, stringArray[i]);
  }

  // Test get
  for (int i = 0; i < vector_size(intVector); i++) {
    int element = intVector[i];
    assert_in_macro(element == intArray[i], "Error: Incorrect element value");
  }

  for (int i = 0; i < vector_size(boolVector); i++) {
    Boolean element = boolVector[i];
    assert_in_macro(element == boolArray[i], "Error: Incorrect element value");
  }

  for (int i = 0; i < vector_size(charVector); i++) {
    char element = charVector[i];
    assert_in_macro(element == charArray[i], "Error: Incorrect element value");
  }

  for (int i = 0; i < vector_size(stringVector); i++) {
    char* element = stringVector[i];
    assert_in_macro(string_compare(element, stringArray[i]), "Error: Incorrect element value");
  }

  // Test pop
  for (int i = 0; i < vector_size(intVector); i++) {
    int element = intVector[i];
  }

  int before_popped_int = intVector[vector_size(intVector) - 1];
  int after_popped_int = vector_pop(intVector, int);
  int element_to_push_for_int = 10;
  vector_push(intVector, element_to_push_for_int);

  for (int i = 0; i < vector_size(intVector); i++) {
    int element = intVector[i];
  }

  assert_in_function(before_popped_int == after_popped_int, "Error: popped expected: %d | got %d\n", before_popped_int, after_popped_int);
  LOG_WARN("New PUSH: %d\n", intVector[vector_size(intVector) - 1]);

  char* before_popped_string = stringVector[vector_size(stringVector) - 1];
  char* after_popped_string = vector_pop(stringVector, char*);
  // Date: May 10, 2024
  // NOTE(Jovanni): This might not work?
  char* string_to_push = "TRYING TO PUSH A LITERAL!\n";
  vector_push(stringVector, string_to_push);

  assert_in_macro(string_compare(before_popped_string, after_popped_string), "Error: Incorrect popped element value");
  LOG_WARN("New PUSH: %s\n", stringVector[vector_size(intVector) - 1]);

  memory_output_allocations(LOG_LEVEL_ERROR);

  // Test free
  vector_free(intVector);
  vector_free(boolVector);
  vector_free(charVector);
  vector_free(stringVector);

  memory_output_allocations(LOG_LEVEL_WARN);

  // Assert that the data is NULL
  assert_in_macro(intVector == NULLPTR, "Error: Vector data is not NULL");
  assert_in_macro(boolVector == NULLPTR, "Error: Vector data is not NULL");
  assert_in_macro(charVector == NULLPTR, "Error: Vector data is not NULL");
  assert_in_macro(stringVector == NULLPTR, "Error: Vector data is not NULL");

  LOG_INFO("All vector tests passed!\n\n"); 
  return;
}

int main() {
  CKit_init();

  String str = string_create("aasfhsdfsdfjsdljflsdkf");
  LOG_PRINT("String: %s\n", str);
  LOG_PRINT("String Length: %d\n", string_length(str));
  memory_output_allocations(LOG_LEVEL_INFO);
  string_append_char(str, 'a');
  LOG_PRINT("String: %s\n", str);

  string_append(str, " | FEEL THE WORLD AROUND ME!");
  LOG_PRINT("String: %s\n", str);

  test_vector_operations();

  string_free(str);

  int* int_array = (int*)memory_allocate(sizeof(int) * 5, MEMORY_TAG_TEMPORARY);
  int* int_array2 = (int*)memory_allocate(sizeof(int) * 5, MEMORY_TAG_TEMPORARY);
  LOG_ERROR("Element: %d (SHOULD BE ZERO)\n\n", int_array[0]);
  for (int i = 0; i < 5; i++) {
      int_array2[i] = 1432;
  }

  memory_copy(int_array2, int_array, sizeof(int) * 5, sizeof(int) * 5);
  for (int i = 0; i < 5; i++) {
      assert_in_macro(int_array[i] == 1432, "Memory copy is fucked!");
      LOG_INFO("Element: %d\n", int_array[i]);
  }

  LOG_DEBUG("Element: %d\n", int_array[0]);

  LOG_FATAL("TESTING\n");
  LOG_ERROR("TESTING\n");
  LOG_WARN("TESTING\n");
  LOG_DEBUG("TESTING\n");
  LOG_INFO("TESTING\n");
  LOG_PRINT("TESTING %s\n\n", str);

  memory_output_allocations(LOG_LEVEL_WARN);

  memory_output_arena_allocations(LOG_LEVEL_INFO);


  memory_free(int_array);
  memory_free(int_array2);
  string_arena_free();
  memory_arena_vector_free();

  memory_output_allocations(LOG_LEVEL_ERROR);


  return 0;
}