# Ckits's Core Thesis
- The point of the ckit is to provide a nice library to build c projects with.
- Ckit is built with the granular api ckit granular [CKG](https://github.com/superg3m/ckg) (OUT DATED FIX THIS!)
- This library will be completly cross-platform (Will be windows exclusive for a while ngl)
- Collection of tools, data structures, and algorithms that I find useful

# How to build
1. ./bootstrap.ps1
2. ./build.ps1 or ./build.ps1 -debug
3. ./debug.ps1
4. ./run.ps1

### key
- '[x]': Finished
- '[/]': Started working on it
- '[ ]': Haven't started working on it

### Known issues/bugs
1. - [x] Queue might not be able to hold more than 8 characters (the sizeof(char*)) easy fix
2. - [ ] Hashmap is not very robust I should offer a compare funciton hashmap and give default presets for primatives.

### Next Steps
1. - [x] Rewrite the memory tagging system
	- [x] Register custom Tags
	- [x] ckit_alloc(size_in_bytes) // No longer requiring a tag it will just be a tag call user_unknown.
	- [x] It should be trivial to put file and function information from where an allocation originated. (be careful for reallocations)
	- [x] Introduce "MEMORY_TAG_MAGIC" in the header to catch bugs early

2. MASSIVE DIRECTIONAL CHANGE (removing ckg entirely) (NEW BRANCH)

2. - [/] Make logging system more robust with better way to color certain text
	- [ ] CKIT_SUCCESS("Well: ${Testings}\n") | surrounding text with ${} makes it green?
		- [ ] CKIT_DEBUG("If there is no brackets everything is blue!\n")
3. - [ ] Parse the message better so you can intuitively \n something.
4. - [ ] Make string stuff a bit more robust I would like to impose a cap on it. (just make the capacity not be able to pass a max)

5. - [ ] Clean up arenas and init pre-exisiting arenas for things like: Strings
6. - [ ] Rewrite all Core utilities using new tracker system, and then Write a bunch of tests validating and verifying all the utilities and logic.
7. - [ ] Start writing EventSystem
8. - [ ] Start writing the **GAME ENGINE** you have everything you need in order to succeed

Thoughts:
	I need to insentivise arenas way more than I currently do it makes lifetimes siginificantly easier.

	Also almost certainly there is problems with puting pointers in arenas or dynamic allocated memory because you might just lose those. Thankfully for
	the most part because of the tagging system I can catch all of these. One thing I need to do is really go through the entire lifetime of some data
	and see if there is any possible way for it to not get picked up by the tag system I think not I think its literally impossible for that to be the case.
	So if I have no memeory leaks according to the tag system then I can be reasonably confident that there are no memory bugs.

	But in regards to dangling pointers maybe I can just give back a index indead of a pointer (Handles are the better pointers)
	Another Thought I should give some credence to is Zii (Zero is Initalization)

	- Memory Life Times -
    1. Extremely short lived. Can be thrown away by end of function. 	  					 (Malloc|Arena)
    2. Short lived + well defined lifetime. Memory allocated "per frame". 					 (Arena)
    3. Long lived + well defined owner. Uniquely owned by a subsystem.    					 (Arena)
    4. Long lived + unclear owner. Heavily shared, unknown when it may be accessed or freed. (Malloc)


# Goals

## CORE
- [x] ckit_types.c

### Assert (Cross-Platform) (Fully inherits CKG impl)
- [x] ckit_assert.h
	- [x] ckit_assert_msg(expression, msg, ...)
	- [x] ckit_assert(expression)

### Logger (Cross-Platform) (Fully inherits CKG impl)
- [x] ckit_logger.h
	- [x] LOG_FATAL(msg, ...)
	- [x] LOG_ERROR(msg, ...)
	- [x] LOG_WARN(msg, ...)
	- [x] LOG_DEBUG(msg, ...)
	- [x] LOG_SUCCESS(msg, ...)
	- [x] LOG_PRINT(msg, ...)

### OS
- [ ] ckit_os.h
	- [ ] ckit_os_get_cwd();
	- [ ] ckit_os_ls();
	- [ ] ckit_os_get_items();
	- [ ] ckit_os_chdir();
	- [ ] ckit_os_mkdir();
	- [ ] ckit_os_create_file();
	- [ ] ckit_os_exists();
	- [ ] ckit_os_run_subprocess();
	- [ ] ckit_os_get_file_info();
	- [ ] ckit_os_system();
	- [ ] ckit_os_path_join();
	- [ ] ckit_os_push();
	- [ ] ckit_os_pop();
	- [ ] ckit_os_dir_append_subdir(directory, sub_directory);
	- [ ] ckit_os_dir_append_file(directory, file);

### Collection 
- [X] ckit_hashmap.h (WANT TO REVISIT THIS BECAUSE I CAN MAKE IT BETTER 100%)
	- [x] ckit_hashmap_create(capacity type, is_pointer_type)
	- [x] ckit_hashmap_put(hashmap, key, element)
		- [x] ckit_hashmap_grow(hashmap)
	- [x] ckit_hashmap_has(hashmap, key)
	- [x] ckit_hashmap_get(hashmap, key, returned_element)
	- [X] ckit_hashmap_free(hashmap)

- [X] ckit_hashset.h 
	- [x] ckit_hashset_create(capacity, type)
	- [x] ckit_hashset_put(hashmap, key, element)
		- [x] ckit_hashset_grow(hashmap)
	- [x] ckit_hashset_has(hashmap, key)
	- [X] ckit_hashset_free(hashmap)


- [x] ckit_linked_list.h
    - [x] ckit_linked_list_create(element_size_in_bytes, is_pointer_type);
    - [x] ckit_node_get(node, returned_value);
    - [x] ckit_linked_list_insert(linked_list, index,  data);
    - [x] ckit_linked_list_get_node(linked_list, index);
    - [x] ckit_linked_list_get(linked_list, index,  data);
    - [x] ckit_linked_list_push(linked_list, node);
    - [x] ckit_linked_list_pop(linked_list, node);
    - [x] ckit_linked_list_remove(linked_list, index);

- [x] ckit_stack.h (Probably will get rid of this because it can be built with Linked Lists/Vectors)
	- [x] MACRO_ckit_stack_create(size_in_bytes, is_pointer_type);
	- [x] ckit_stack_push(data);
	- [x] ckit_stack_pop();
	- [x] ckit_stack_free();

- [x] ckit_queue.h (Probably will get rid of this because it can be built with Linked Lists/Vectors)
	- [x] ckit_queue_create(inital_capacity, element_size_in_bytes, is_pointer_type);
	- [x] ckit_queue_free(queue);
	- [x] ckit_enqueue(queue, element);
	- [x] ckit_dequeue(queue);
	- [x] ckit_queue_capacity(queue);
	- [x] ckit_queue_count(queue);

- [x] ckit_vector.h
	- [x] ckit_vector_length(vector)
	- [x] ckit_vector_capacity(vector)
	- [x] ckit_vector_push(vector, element)
	- [x] ckit_vector_remove_at(vector, element, index)
	- [x] ckit_vector_insert_at(vector, element, index)
	- [x] ckit_vector_free(vector)

### String
- [x] ckit_string.h
	- [x] ckit_str_create(str)
	- [x] ckit_str_append(str, string_buffer_capacity, to_append)
	- [x] ckit_str_append_char(str, string_buffer_capacity, char to_append)
	- [x] ckit_str_insert(str, string_buffer_capacity, to_insert, index)
	- [x] ckit_str_insert_char(str, string_buffer_capacity, char to_insert, index)
	- [x] ckit_str_copy(str, string_buffer_capacity, to_copy)
	- [x] ckit_str_random(dest, length)
	- [x] ckit_str_equal(s1, s2)
	- [x] ckit_cstr_length(c_string or ckit_string)
	- [x] ckit_str_length(ckit_string only)
	- [x] ckit_str_clear(str)
	- [x] ckit_substring(str, start_range, end_range)
	- [x] ckit_str_contains(str, contains)
	- [x] ckit_str_index_of(str, sub_string)
	- [x] ckit_str_last_index_of(str, sub_string)
	- [x] ckit_str_split(str, delimitor)
	- [x] ckit_str_starts_with(str, starts_with)
	- [x] ckit_str_ends_with(str, ends_with)
	- [x] ckit_str_reverse(str)
	- [x] ckit_str_int_to_str(int)
	- [/] ckit_str_between_delimiters(str, str, str) (Not tested really)

### Memory
- [x] ckit_memory.h
	- [x] ckit_bind_alloc_callback(func_allocator)
	- [x] ckit_bind_free_callback(func_allocator)
	- [x] ckit_alloc(allocation_size)
	- [x] ckit_realloc(data, old_allocation_size, new_allocation_size)
	- [x] ckit_free(data)
	- [x] ckit_memory_compare(buffer_one, buffer_two, b1_allocation_size, b2_allocation_size)
	- [x] ckit_memory_copy(source, destination, source_size, destination_capacity)
	- [x] ckit_memory_move(source, destination, source_payload_size)
	- [x] ckit_memory_zero(data, data_size_in_bytes)
	- [x] ckit_memory_delete_index(data, data_capacity, index)
	- [x] ckit_memory_insert_index(data, data_capacity, element, index)

- [x] ckit_memory_tracker.h
	- [x] ckit_tracker_init();
	- [x] ckit_tracker_register_tag_pool(tag_id, name);
    - [x] MACRO_ckit_tracker_insert_header(data, CKIT_MemoryHeader header);

    - [x] ckit_tracker_header_create(tag_id, allocation_size);
	- [x] memory_insert_header(data, CKIT_MemoryHeader header);
    - [x] ckit_tracker_add(header);
    - [x] ckit_tracker_remove(header);
    - [x] ckit_tracker_get_header(data);

    - [x] ckit_tracker_print_header(header, log_level);
    - [x] ckit_tracker_print_pool(pool, log_level);

- [x] ckit_arena.h (NOT TESTED)
	- [x] ckit_arena_create(allocation, name)
	- [x] ckit_arena_create_custom(allocation, name, flag)
	- [x] ckit_arena_push(arena, type, byte_alignment = 8)
	- [x] ckit_arena_free(arena)
	- [x] ckit_arena_clear(arena)

### FileIO (Finished, but I dont like it gonna rewrite it)
- [x] ckit_file_io.h
	- [x] ckit_file_system_create(file_name)
	- [x] ckit_file_open(file_system)
	- [x] ckit_file_close(file_system)
	- [x] ckit_file_size(file_system)
	- [x] ckit_file_get_next_line(file_system)
	- [x] ckit_file_get_next_char(file_system)

### Threading
- [ ] ckit_locks.h
	- [ ] ckit_threading_interlock_exchange()
- [ ] ckit_job_system.h
- [ ] ckit_semaphores.h
- [ ] ckit_sleep.h


## MODULES

### Hot_Reloader
- [ ] ckit_file_watcher.h
- [ ] ckit_dynamic_libary_loader.h

### Introspection
- [ ] ckit_struct_introspection.h

### Native_Window
- [ ] ckit_native_window_open()


### Lexer
- [ ] ckit_lexer.h
	- [x] void ckit_lexer_load_file_data(lexer, file_path);
	- [x] void ckit_lexer_load_string(lexer, string);
	- [x] char* ckit_lexer_token_to_string(token);
	- [x] CKIT_Tokens ckit_lexer_generate_next_token(lexer);
	- [ ] CKIT_Tokens* ckit_lexer_peek_next_token(lexer);
	- [x] CKIT_Tokens* ckit_lexer_generate_token_stream(lexer);
	- [ ] CKIT_Tokens* ckit_lexer_consume_token_stream(lexer);
	- [ ] CKIT_Tokens* ckit_lexer_free(lexer);

### Networking
- [ ] ckit_client.h
- [ ] ckit_server.h

### Intrinsics (PROB CORE)
- [ ] compiler_memory_fences
- [ ] interlock_exchange
- [ ] math_functions
	- sin
	- cos
	- tan

### SIMD
make vector operations easier

### Parser
- Asset
	- [x] BMP
	- [ ] PNG
	- [ ] OBJ
	- [ ] PDF
- JSON

## Tests
- [ ] test_memory_functions()
- [ ] test_arena_functions()
- [ ] test_vector_functions()
- [ ] test_string_functions()

