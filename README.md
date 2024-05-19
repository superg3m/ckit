# CKit

- Collection of tools, data structures, and algorithms that I find useful
- If cl.exe is not defined in your terminal run vars.bat
- Compile with CUSTOM_PLATFORM_IMPL defined if you want to change how platform specific functions work.

Thoughts:
 - I need to rethink memory allocations
 - so heres the think with arenas I won't have tons of syscalls for memory allocations so that way I can use a function pointer for the allocation function so default stub is malloc and free, you can define you own callback function
 - I need to rethink how arenas work



Next course of action is to remove all memory allocation tracking, the way I did it was really bad so its hard to actually keep working on anything.