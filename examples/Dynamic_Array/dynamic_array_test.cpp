#include "../../CKit.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void test_dynamic_array_operations() {
	LOG_DEBUG("TESING!");
	// Test types
	int intArray[] = {1, 2, 3, 4, 5};
	bool boolArray[] = {true, false, true, true, false};
	char charArray[] = {'a', 'b', 'c', 'd', 'e'};
	char stringArray[][10] = {"Hello", "World", "!", "OpenAI", "GPT-3"};

	// Create vector of int
	int* intVector = dynamic_array_reserve(5, int);
	for (int i = 0; i < 5; i++) {
		dynamic_array_push(intVector, intArray[i]);
	}

	// Create vector of bool
	bool* boolVector = dynamic_array_reserve(5, bool);
	for (int i = 0; i < 5; i++) {
		dynamic_array_push(boolVector, boolArray[i]);
	}

	LOG_DEBUG("TESING!");

	// Create vector of char
	char* charVector = dynamic_array_reserve(5, char);
	for (int i = 0; i < 5; i++) {
		dynamic_array_push(charVector, charArray[i]);
	}

	// Create vector of strings
	char** stringVector = dynamic_array_create(char*);
	for (int i = 0; i < 5; i++) {
		dynamic_array_push(stringVector, stringArray[i]);
	}

	// Test get
	for (int i = 0; i < dynamic_array_size(intVector); i++) {
		assert(intVector[i] == intArray[i], "Error: Incorrect element value");
	}

	LOG_DEBUG("TESING!");

	for (int i = 0; i < dynamic_array_size(boolVector); i++) {
		assert(boolVector[i] == boolArray[i], "Error: Incorrect element value");
	}

	for (int i = 0; i < dynamic_array_size(charVector); i++) {
		assert(charVector[i] == charArray[i], "Error: Incorrect element value");
	}

	LOG_DEBUG("TESING!");

	for (int i = 0; i < dynamic_array_size(stringVector); i++) {
		assert(strcmp(stringVector[i], stringArray[i]) == 0, "Error: Incorrect element value");
	}

	

	// Test pop
	int poppedInt = dynamic_array_pop(intVector, int);

	for (int i = 0; i < dynamic_array_size(intVector); i++) {
		int element = intVector[i];
	}

	assert(poppedInt == intArray[dynamic_array_size(intVector)], "Error: Incorrect popped element value");

	// Test free
	dynamic_array_free(intVector);
	dynamic_array_free(boolVector);
	dynamic_array_free(charVector);
	dynamic_array_free(stringVector);

	// Assert that the data is NULL
	assert(intVector == NULL, "Error: Vector data is not NULL");
	assert(boolVector == NULL, "Error: Vector data is not NULL");
	assert(charVector == NULL, "Error: Vector data is not NULL");
	assert(stringVector == NULL, "Error: Vector data is not NULL");

	LOG_INFO("All vector tests passed!");
}

int main() {
	if(AttachConsole(ATTACH_PARENT_PROCESS)){
        freopen("CONOUT$","wb",stdout);
        freopen("CONOUT$","wb",stderr);
    }
	
	logger_init();

	test_dynamic_array_operations();

  	return 0;
}
