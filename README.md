### Taking a small break like a month so!

# Ckits's Core Thesis
- The point of the ckit is to provide a nice library to build c projects with.
- ckit is built with the granular api ckit granular [CKG](https://github.com/superg3m/ckg)
- This library will be completly cross-platform (Will be windows exclusive for a while ngl)
- Collection of tools, data structures, and algorithms that I find useful

- cloc . --fullpath --not-match-d='.*External_Libraries.*'

# CKIT Plugins
- CKIT_Graphics allows you to create native windows and draw to the screen! [CKIT_Graphics](https://github.com/superg3m/ckit_graphics)

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
1. - [ ] Hashmap is not very robust I should offer a compare funciton hashmap and give default presets for primatives.
2. - [ ] Parse the message better so you can intuitively \n something. (Maybe add some string in the beginning of a message ot ommit the <WARN:> for example)

### Next Steps
1. - [ ] forward declare types to avoid redefs
		- [ ] enums
		- [ ] defines
		- [ ] structs
2. - [ ] Hot_Reloading

Thoughts:
	But in regards to dangling pointers maybe I can just give back a index indead of a pointer (Handles are the better pointers)
	Another Thought I should give some credence to Zii (Zero is Initalization)

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

### Logger (Cross-Platform)
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
	- [/] ckit_os_chdir();
	- [ ] ckit_os_mkdir();
	- [x] ckit_os_create_file(path); // prob just assert if path doesn't exists
	- [x] ckit_os_path_exists(path);
	- [ ] ckit_os_run_subprocess();
	- [ ] ckit_os_get_file_info();
	- [x] ckit_os_path_join(path, to_join);
	- [x] ckit_os_system(command);
	- [x] ckit_os_query_performance_counter();
	- [ ] ckit_os_file_open(path);
	- [ ] ckit_os_file_read_next_line(file);
	- [ ] ckit_os_file_read_next_integer(file);
	- [ ] ckit_os_file_read_next_float(file);
	- [ ] ckit_os_file_close(file);
	- [ ] ckit_os_close_file(file);
	- [x] ckit_os_read_entire_file(path);
	- [x] ckit_os_get_mouse_position(mouse_x, mouse_y)
	- [ ] ckit_os_push();
	- [ ] ckit_os_pop();
	- [ ] ckit_os_dir_append_subdir(directory, sub_directory);
	- [ ] ckit_os_dir_append_file(directory, file);
	- [ ] ckit_os_timer_start()
	- [ ] ckit_os_timer_end()

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
	- [x] ckit_str_between_delimiters(str, str, str)

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

### Native_Window (Only really works for windows os right now)
- [/] ckit_native_window.h
	- [x] ckit_window_create(width, height, name);
	- [x] ckit_window_free(window);
	- [x] ckit_window_bind_icon(resource_path);
	- [x] ckit_window_bind_cursor(resource_path);
	- [x] ckit_window_should_quit(window);
	- [x] ckit_window_clear_color(window, color);
	- [x] ckit_window_draw_quad(window, start_x, start_y, width, height, color);
	- [/] ckit_window_draw_circle(window, start_x, start_y, radius, is_filled, color);
	- [x] ckit_window_draw_bitmap(window);
	- [x] ckit_window_get_mouse_position(window, mouse_x, mouse_y);
	- [x] ALPHA BLENDING FOR DRAW OPPERATIONS!


### Lexer
- [ ] ckit_lexer.h
	- [x] ckit_lexer_load_file_data(lexer, file_path);
	- [x] ckit_lexer_load_string(lexer, string);
	- [x] ckit_lexer_token_to_string(token);
	- [x] ckit_lexer_generate_next_token(lexer);
	- [ ] ckit_lexer_peek_next_token(lexer);
	- [x] ckit_lexer_generate_token_stream(lexer);
	- [ ] ckit_lexer_consume_token_stream(lexer);
	- [ ] ckit_lexer_free(lexer);

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


# Develoepr Notes: (For me)
- [ ] Arenas need to have an option to desigate memory such as stack memory or heap memory.
	- This is for stuff like embbeded systems if you don't have access to a virtual alloc.
	- CKIT_Arena* arena = ckit_arena_create(0); // Optional 
	- u8 memory_buffer[MB(40)];
	- ckit_arena_assign_memory(arena, memory_buffer);
	- I need to string view a lot more...
	- string should be a last ditch thing, you should always be returned string views and then you can allocate a `String` if you really need it...
	- Use arenas for much more... vectors need some type of way to input an allocator.
	- Any function that allocats should have a version that uses an allocator/arena

- [ ] Start phasing out standard library use native functions for everything.
	- [ ] printf() -> ckit_printf():
		- [ ] ckit_printf("%iv\n") // (int vector) -> [5, 6, 2]
		- [ ] ckit_printf("%fv\n") // (float vector) [1.0, 3.24]
		- [ ] ckit_printf("%sv\n") // (string vector) ["Stuff in here", "Hello"]
		- [ ] ckit_printf("%cv\n") // (char vector) ['y', 'f']