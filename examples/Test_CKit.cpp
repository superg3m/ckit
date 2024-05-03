#include "../CKit.h"

int main() {
    String str = string_create("JESTER");
    
    LOG_PRINT("String: %s\n", str.data);
    LOG_PRINT("String Length: %d\n", str.length);
	//console_write_memory_tags();
    
    //string_free(&str);
    
    console_write_memory_tags();

	LOG_FATAL("TESTING\n");
    LOG_ERROR("TESTING\n");
    LOG_WARN("TESTING\n");
    LOG_DEBUG("TESTING\n");
    LOG_INFO("TESTING\n");

    return 0;
}