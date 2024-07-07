# CKit

- Collection of tools, data structures, and algorithms that I find useful
- If cl.exe is not defined in your terminal run vars.ps1
- Compile with CUSTOM_PLATFORM_IMPL defined if you want to change how platform specific functions work.

Thoughts:
 - I need to rethink memory allocations
 - so heres the think with arenas I won't have tons of syscalls for memory allocations so that way I can use a function pointer for the allocation function so default stub is malloc and free, you can define you own callback function
 - I need to rethink how arenas work

Next course of action is to remove all memory allocation tracking, the way I did it was really bad so its hard to actually keep working on anything.


if I want to keep using header I need to make available the header size with a function so I can then say you must specify the size not just the type


it kind of is the case that you want to init some type of memory arena and just push stuff because of the type system and maybe that is better overall?


- ckg
	- string.c
		- StringView
		- StringBuilder

	- file_io.c
		- open_file()
		- read_entire_file()
		- close_file()
	- assert.c
	- logger.c
	- event_system.c

	- input.c

	- platform_services.c

	- networking.c
		- udp/tcp
		- udp hole punching
		- host and client structs 
		- NAT

	- types.c
		- base types

	- threading.c
		- semaphores
		- job_system
	- simd.c
		- intrinsics
	- math.c
		- Vector2, Vector3
		- simd calculations
		- roundf, sin, cos, tan (use intrinsics)
		-

	asset.c
		- 2D
			- bitmap_loader
			- png_loader
		- 3D
			- obj_loader


	- DSA.c
		- Data Structures
			- vector (Make sure this isn't used everywhere in ckg)
			- hashing
			- hashmap (uses hashing)
			- linked_list



FIX FILE STRUCTURES FIRST