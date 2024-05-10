#include "../CKit.h"

void test_vector_operations() {
  // Test types
  int intArray[] = {1, 2, 3, 4, 5};
  bool boolArray[] = {true, false, true, true, false};
  char charArray[] = {'a', 'b', 'c', 'd', 'e'};
  char* stringArray[] = {"Hello", "World", "!", "OpenAI", "GPT-3"};

  // Create vector of int
  int* intVector = vector_reserve(5, int);
  for (int i = 0; i < 5; i++) {
    vector_push(intVector, intArray[i]);
  }

  // Create vector of bool
  bool* boolVector = vector_reserve(5, bool);
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
    assert_in_function(element == intArray[i], "Error: Incorrect element value\n");
  }

  for (int i = 0; i < vector_size(boolVector); i++) {
    bool element = boolVector[i];
    assert_in_function(element == boolArray[i], "Error: Incorrect element value\n");
  }

  for (int i = 0; i < vector_size(charVector); i++) {
    char element = charVector[i];
    assert_in_function(element == charArray[i], "Error: Incorrect element value\n");
  }

  for (int i = 0; i < vector_size(stringVector); i++) {
    char* element = stringVector[i];
    assert_in_function(string_compare(element, stringArray[i]), "Error: Incorrect element value\n");
  }

  // Test pop
  for (int i = 0; i < vector_size(intVector); i++) {
    int element = intVector[i];
  }

  int before_popped_int = intVector[vector_size(intVector) - 1];
  int after_popped_int = vector_pop(intVector, int);
  vector_push_literal(intVector, 10);

  for (int i = 0; i < vector_size(intVector); i++) {
    int element = intVector[i];
  }

  assert_in_function(before_popped_int == after_popped_int, "Error: popped expected: %d | got %d\n", before_popped_int, after_popped_int);
  LOG_WARN("New PUSH: %d\n", intVector[vector_size(intVector) - 1]);

  char* before_popped_string = stringVector[vector_size(stringVector) - 1];
  char* after_popped_string = vector_pop(stringVector, char*);
  vector_push_literal(stringVector, string_literal_ptr("TRYING TO PUSH A LITERAL!\n"));

  assert_in_function(string_compare(before_popped_string, after_popped_string), "Error: Incorrect popped element value\n");
  LOG_WARN("New PUSH: %s\n", stringVector[vector_size(intVector) - 1]);

  console_write_memory_tags(LOG_LEVEL_ERROR);

  // Test free
  vector_free(intVector);
  vector_free(boolVector);
  vector_free(charVector);
  vector_free(stringVector);

  console_write_memory_tags(LOG_LEVEL_WARN);

  // Assert that the data is NULL
  assert_in_function(intVector == NULL, "Error: Vector data is not NULL\n");
  assert_in_function(boolVector == NULL, "Error: Vector data is not NULL\n");
  assert_in_function(charVector == NULL, "Error: Vector data is not NULL\n");
  assert_in_function(stringVector == NULL, "Error: Vector data is not NULL\n");

  LOG_INFO("All vector tests passed!\n"); 
  return;
}

int main() {
    String str = string_create("aasfhsdfsdfjsdljflsdkf");
    LOG_PRINT("String: %s\n", str);
    LOG_PRINT("String Length: %d\n", string_length(str));
	  console_write_memory_tags(LOG_LEVEL_INFO);
    string_append_char(&str, 'a');
    LOG_PRINT("String: %s\n", str);

    string_append(&str, " | FEEL THE WORLD AROUND ME!");
    LOG_PRINT("String: %s\n", str);

    test_vector_operations();

    return 0;
    
    string_free(&str);

    int* int_array = (int*)memory_allocate(sizeof(int) * 5, MEMORY_TAG_TEMPORARY);
    int* int_array2 = (int*)memory_allocate(sizeof(int) * 5, MEMORY_TAG_TEMPORARY);
    LOG_ERROR("Element: %d\n", int_array[0]);
    for (int i = 0; i < 5; i++) {
        int_array2[i] = 1432;
    }

    memory_copy(sizeof(int) * 5, int_array2, sizeof(int) * 5, int_array);
    for (int i = 0; i < 5; i++) {
        assert_in_macro(int_array[i] == 1432, "Memory copy is fucked!\n");
        LOG_INFO("Element: %d\n", int_array[i]);
    }
    
    //console_write_memory_tags();

    LOG_DEBUG("Element: %d\n", int_array[0]);

	  LOG_FATAL("TESTING\n");
    LOG_ERROR("TESTING\n");
    LOG_WARN("TESTING\n");
    LOG_DEBUG("TESTING\n");
    LOG_INFO("TESTING\n");
    LOG_PRINT("TESTING %s\n", str);

    return 0;
}