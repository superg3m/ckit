# Ckits's Core Thesis
- The point of the ckit is to provide a nice library to build c projects with.
- Ckit is built with the granular api ckit granular [CKG](https://github.com/superg3m/ckg)
- This library will be completly cross-platform
- Collection of tools, data structures, and algorithms that I find useful

# How to build
1. ./bootstrap.ps1
2. ./build.ps1 or ./build.ps1 -debug
3. ./debug.ps1
4. ./run.ps1



Thoughts:
	Vectors right now in their conception have a really big problem ckit's dyanmic arrays (vectors) rely on a header component before the actual 
	data. This is a problem because taking a trivial example like a 2d dynamic array, if you push an array into a 2d array there is a high probability that the pointer to the array will get invalidated because when the dynamic array grows it changes address meaning I would somehow have to track references and update those. Updating refrences is a fools errand so what choices do we have?
	OPTIONS
		#1. Mandate that the code be written in a specific way that avoid this problem.
			- very error prone (I litterally made this error myself when writing spline points)
		#2 

	Rethink dynamic allocations I think I need to get rid of the vector completely
	There is a different line of thinking that doesn't use dynamic allocations
	I don't necessarily have to push spline points I can just calculate how much something is needed before hand.

# Goals
### key
- '[x]': Finished
- '[/]': Started working on it
- '[ ]': Haven't started working on it

## CORE
- [x] ckit_types.c

### Assert (Cross-Platform) (Fully inherits CKG impl)
- [x] ckit_assert.h
	- [x] ckit_assert_msg(expression, msg, ...)
	- [x] ckit_assert(expression)

### Logger (Cross-Platform) (Fully inherits CKG impl)
- [x] ckit_logger.h
	- [x] ckit_LOG_FATAL(msg, ...)
	- [x] ckit_LOG_ERROR(msg, ...)
	- [x] ckit_LOG_WARN(msg, ...)
	- [x] ckit_LOG_DEBUG(msg, ...)
	- [x] ckit_LOG_SUCCESS(msg, ...)
	- [x] ckit_LOG_PRINT(msg, ...)

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
- [X] ckit_hashmap.h (Tested on integers, char, string, and structs) (WANT TO REVISIT THIS BECAUSE I CAN MAKE IT BETTER 100%)
	- [x] ckit_hashmap_create(type)
	- [x] ckit_hashmap_put(hashmap, key, element)
		- [x] ckit_hashmap_grow(hashmap)
	- [x] ckit_hashmap_has(hashmap, key)
	- [x] ckit_hashmap_get(hashmap, key, returned_element)
	- [X] ckit_hashmap_free(hashmap)

- [ ] ckit_linked_list.h
    - [x] ckit_linked_list_create(element_size_in_bytes);
    - [x] ckit_node_create(data, element_size_in_bytes);
    - [x] ckit_node_get(node, returned_value);
    - [/] ckit_linked_list_insert(linked_list, index,  data);
    - [ ] ckit_linked_list_get_node(linked_list, index);
    - [/] ckit_linked_list_get(linked_list, index,  data);
    - [ ] ckit_linked_list_push(linked_list, node);
    - [ ] ckit_linked_list_pop(linked_list, node);
    - [x] ckit_linked_list_remove(linked_list, index);








- [ ] ckit_stack.h
- [x] ckit_queue.h (NOT REALLY TESTED) (WANT TO REVISIT THIS BECAUSE I CAN MAKE IT BETTER 100%)
	- [x] ckit_queue_create(inital_capacity, element_size_in_bytes);
	- [x] ckit_queue_free(queue);
	- [x] ckit_enqueue(queue, element);
	- [x] ckit_dequeue(queue, returned_elemenet);
	- [x] ckit_queue_capacity(queue);
	- [x] ckit_queue_count(queue);

- [ ] ckit_binary_tree.h

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

### Memory (REFORMS COMING SOON FOR THE MEMORY TAG SYSTEM NOT EXPRESSIVE ENOUGH I WOULD LIKE TO REGISTER TAGS TBH)
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

- [x] ckit_arena.h (NOT TESTED)
	- [x] ckit_arena_create(allocation, name)
	- [x] ckit_arena_create_custom(allocation, name, flag)
	- [x] ckit_arena_push(arena, type, byte_alignment = 4)
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
	- [/] CKIT_Tokens ckit_lexer_generate_next_token(lexer);

	- [ ] CKIT_Tokens* ckit_lexer_generate_token_stream(lexer);
	- [ ] CKIT_Tokens* ckit_lexer_consume_token_stream(lexer);
	- [ ] CKIT_Tokens* ckit_lexer_peek_token_stream(lexer);

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

