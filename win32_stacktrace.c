// Ripped from @Boostibot's github

#include <stdint.h>
typedef struct Platform_Stack_Trace_Entry {
    char function[256]; //mangled or unmangled function name
    char module[256];   //mangled or unmangled module name ie. name of dll/executable
    char file[256];     //file or empty if not supported
    int64_t line;       //0 if not supported
} Platform_Stack_Trace_Entry;

//Captures the current stack frame pointers. 
//Saves up to stack_size pointres into the stack array and returns the number of
//stack frames captures. If the returned number is exactly stack_size a bigger buffer
//MIGHT be reuqired.
//Skips first skip_count stack pointers. Evem with skip_count = 0 this function should not be
//included within the stack
int64_t platform_debug_capture_stack(void** stack, int64_t stack_size, int64_t skip_count);

//Translates captured stack into helpful entries. Operates on short fixed width strings to guarantee this function
//will never fail yet translate all needed stack frames. This function should never allocate anything.
void platform_debug_translate_stack(Platform_Stack_Trace_Entry* tanslated, const void** stack, int64_t stack_size);


//Implementation
#include <stdbool.h>
#include <windows.h>
#include <Psapi.h>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "dbghelp.lib")

// Some versions of imagehlp.dll lack the proper packing directives themselves
// so we need to do it.
#pragma pack( push, before_imagehlp, 8 )
#include <imagehlp.h>
#pragma pack( pop, before_imagehlp )

int64_t platform_debug_capture_stack(void** stack, int64_t stack_size, int64_t skip_count)
{
    int64_t captured = CaptureStackBackTrace((DWORD) skip_count + 1, stack_size, stack, NULL);
    return captured;
}

#define MAX_MODULES 128 
#define MAX_NAME_LEN 4096

static bool   stack_trace_init = false;
static HANDLE stack_trace_process;
static DWORD  stack_trace_error = 0;

static void _platform_stack_trace_init(const char* search_path)
{
    if(stack_trace_init)
        return;

    stack_trace_process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    if (!SymInitialize(stack_trace_process, search_path, false)) 
    {
        assert(false);
        stack_trace_error = GetLastError();
        return;
    }

    DWORD symOptions = SymGetOptions();
    symOptions |= SYMOPT_LOAD_LINES | SYMOPT_UNDNAME;
    SymSetOptions(symOptions);
    
    DWORD module_handles_size_needed = 0;
    HMODULE module_handles[MAX_MODULES] = {0};
    TCHAR module_filename[MAX_NAME_LEN] = {0};
    TCHAR module_name[MAX_NAME_LEN] = {0};
    EnumProcessModules(stack_trace_process, module_handles, sizeof(module_handles), &module_handles_size_needed);
    
    DWORD module_count = module_handles_size_needed/sizeof(HMODULE);
    for(int64_t i = 0; i < module_count; i++)
    {
        HMODULE module_handle = module_handles[i];
        MODULEINFO module_info = {0};
        GetModuleInformation(stack_trace_process, module_handle, &module_info, sizeof(module_info));
        GetModuleFileNameEx(stack_trace_process, module_handle, module_filename, sizeof(module_filename));
        GetModuleBaseName(stack_trace_process, module_handle, module_name, sizeof(module_name));
        
        bool load_state = SymLoadModuleExW(stack_trace_process, 0, module_filename, module_name, (DWORD64)module_info.lpBaseOfDll, (DWORD) module_info.SizeOfImage, 0, 0);
        if(load_state == false)
        {
            assert(false);
            stack_trace_error = GetLastError();
        }
    }

    stack_trace_init = true;
}

static void _platform_stack_trace_deinit()
{
    SymCleanup(stack_trace_process);
}

void platform_debug_translate_stack(Platform_Stack_Trace_Entry* tanslated, const void** stack, int64_t stack_size)
{
    _platform_stack_trace_init("");
    char symbol_info_data[sizeof(SYMBOL_INFO) + MAX_NAME_LEN + 1] = {0};

    DWORD offset_from_symbol = 0;
    IMAGEHLP_LINE64 line = {0};
    line.SizeOfStruct = sizeof line;

    Platform_Stack_Trace_Entry null_entry = {0};
    for(int64_t i = 0; i < stack_size; i++)
    {
        Platform_Stack_Trace_Entry* entry = tanslated + i;
        DWORD64 address = (DWORD64) stack[i];
        *entry = null_entry;

        if (address == 0)
            continue;

        memset(symbol_info_data, '\0', sizeof symbol_info_data);

        SYMBOL_INFO* symbol_info = (SYMBOL_INFO*) symbol_info_data;
        symbol_info->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol_info->MaxNameLen = MAX_NAME_LEN;
        DWORD64 displacement = 0;
        SymFromAddr(stack_trace_process, address, &displacement, symbol_info);
            
        if (symbol_info->Name[0] != '\0')
        {
            int64_t actual_size = UnDecorateSymbolName(symbol_info->Name, entry->function, sizeof entry->function, UNDNAME_COMPLETE);
        }
           
        IMAGEHLP_MODULE module_info = {0};
        module_info.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
        bool module_info_state = SymGetModuleInfo64(stack_trace_process, address, &module_info);
        if(module_info_state)
        {
            int64_t copy_size = sizeof module_info.ImageName;
            if(copy_size > sizeof entry->module - 1)
                copy_size = sizeof entry->module - 1;

            memmove(entry->module, module_info.ImageName, copy_size);
        }
            
        if (SymGetLineFromAddr64(stack_trace_process, address, &offset_from_symbol, &line)) 
        {
            entry->line = line.LineNumber;
            
            int64_t copy_size = strlen(line.FileName);
            if(copy_size > sizeof entry->file - 1)
                copy_size = sizeof entry->file - 1;

            memmove(entry->file, line.FileName, copy_size);
        }
    }
}

#include <stdio.h>
void func1()
{
    #define STACK_SIZE 16
    Platform_Stack_Trace_Entry entries[STACK_SIZE] = {0};
    void* stack[STACK_SIZE] = {0};

    int64_t stack_size = platform_debug_capture_stack(stack, STACK_SIZE, 1);
    platform_debug_translate_stack(entries, stack, stack_size);
    
    for(int64_t i = 0; i < stack_size; i++)
    {
        printf("%s %s %s %d\n", entries[i].module, entries[i].function, entries[i].file, (int) entries[i].line);
    }
}

void func2()
{
    func1();
}

void func3()
{
    func2();
}

int main()
{
    func3();
}
