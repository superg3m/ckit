#include "../../CKit.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int main() {
	if(AttachConsole(ATTACH_PARENT_PROCESS)){
        freopen("CONOUT$","wb",stdout);
        freopen("CONOUT$","wb",stderr);
    }
	
	logger_init();
	LOG_FATAL("TESTING");

  	return 0;
}
