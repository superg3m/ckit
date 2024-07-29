#include "ckg_stack_trace.h"

#include "ckg_logger.h"
#include "ckg_memory.h"
#include <stdarg.h>

#if defined(_MSC_VER )
#include <windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp")
void ckg_stack_trace_dump() {
    CKG_LOG_PRINT("------------------ Error Stack Trace ------------------\n");
    // Date: July 02, 2024
    // NOTE(Jovanni): This only works for windows and when debug symbols are compiled into the program
    void *stack[100];
    unsigned short number_of_captured_frames;
    SYMBOL_INFO *symbol;
    HANDLE process;

    process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);

    number_of_captured_frames = CaptureStackBackTrace(0, 100, stack, NULL);
    symbol = (SYMBOL_INFO *)ckg_alloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char));
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    int count = 0;
    for (int i = number_of_captured_frames - 4; i > 0; i--) {
        DWORD64 displacement = 0;
        if (SymFromAddr(process, (DWORD64)(stack[i]), &displacement, symbol)) {
            DWORD displacementLine = 0;
            IMAGEHLP_LINE64 line;
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            if (SymGetLineFromAddr64(process, (DWORD64)(stack[i]), &displacementLine, &line)) {
                printf("%d: %s - %s:%d\n", count, symbol->Name, line.FileName, line.LineNumber);
            } else {
                printf("%d: %s\n", count, symbol->Name, symbol->Address);
            }
        }
        count++;
    }

    ckg_free(symbol);
    CKG_LOG_PRINT("------------------ Error Stack Trace End ------------------\n");
}
#elif defined(__GNUC__)
void ckg_stack_trace_dump() {
    CKG_LOG_PRINT("------------------ Error Stack Trace ------------------\n");
    // backtrace
    CKG_LOG_PRINT("------------------ Error Stack Trace End ------------------\n");
}
#endif