#include "../CKit.h"

int main() {
    String str = string_create("aasfhsdfsdfjsdljflsdkf");
    LOG_WARN("String: %s\n", str.data);
    LOG_WARN("String: %s\n", str.data);
    LOG_WARN("String: %s\n", str.data);
    LOG_WARN("String Length: %d\n", str.length);
    
	console_write_memory_tags();
    
    //string_free(&str);

    //int* int_array = (int*)memory_allocate(sizeof(int) * 5, MEMORY_TAG_TEMPORARY);
    //int* int_array2 = (int*)memory_allocate(sizeof(int) * 5, MEMORY_TAG_TEMPORARY);
    //LOG_ERROR("Element: %d\n", int_array[0]);
    for (int i = 0; i < 5; i++) {
        //int_array2[i] = 1432;
    }

    //memory_copy(sizeof(int) * 5, int_array2, sizeof(int) * 5, int_array);
    for (int i = 0; i < 5; i++) {
        //assert_in_macro(int_array[i] == 1432, "Memory copy is fucked!\n");
       //LOG_INFO("Element: %d\n", int_array[i]);
    }
    
    //console_write_memory_tags();

    //LOG_DEBUG("Element: %d\n", int_array[0]);
    //LOG_INFO("String Length: %d\n", 234);
	LOG_FATAL("TESTING\n");
    LOG_ERROR("TESTING\n");
    LOG_WARN("TESTING\n");
    LOG_DEBUG("TESTING\n");
    LOG_INFO("TESTING\n");

    return 0;
}