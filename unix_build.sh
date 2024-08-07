#!/bin/bash

# Default values for parameters
project_to_build="all"
is_debug=true

# Parse command-line arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --project_to_build) project_to_build="$2"; shift ;;
        --is_debug) is_debug="$2"; shift ;;
        *) echo "Unknown parameter passed: $1"; exit 1 ;;
    esac
    shift
done

# Source the vars.sh script
source ./vars.sh

# Validate the project_to_build parameter
if [[ "$project_to_build" != "all" && "$project_to_build" != "ckg" && "$project_to_build" != "ckit" ]]; then
    echo "Invalid project to build. Expected: all, ckg, or ckit. Got: $project_to_build"
    exit 1
fi

# Set the compiler flags
exe_flags="clang -fsanitize=address -Wall -Wextra -std=c11"
lib_flags="clang -c -fsanitize=address -Wall -Wextra -std=c11"
if [[ "$is_debug" == true ]]; then
    exe_flags+=" -g -O0"
    lib_flags+=" -g -O0"
else
    exe_flags+=" -O2"
    lib_flags+=" -O2"
fi

# Function to clean a directory
clean_directory() {
    rm -rf "$1"/*
}

# Function to build a library
build_lib() {
    local output_name="$1"
    local include_paths=("${!2}")
    local source_files=("${!3}")
    local build_dir="$4"

    local include_args=()
    for include in "${include_paths[@]}"; do
        include_args+=("-I$include")
    done

    local source_args=()
    for source in "${source_files[@]}"; do
        source_args+=("$source")
    done

    echo "================================== $output_name =================================="

    pushd "$build_dir" > /dev/null
    clang "${lib_flags[@]}" "${include_args[@]}" "${source_args[@]}"
    ar rcs "$output_name.a" *.o
    popd > /dev/null

    echo "================================== $output_name build complete =================================="
}

# Function to build an executable
build_exe() {
    local output_name="$1"
    local include_paths=("${!2}")
    local source_files=("${!3}")
    local additional_libs=("${!4}")
    local build_dir="$5"

    local include_args=()
    for include in "${include_paths[@]}"; do
        include_args+=("-I$include")
    done

    local source_args=()
    for source in "${source_files[@]}"; do
        source_args+=("$source")
    done

    local lib_args=()
    for lib in "${additional_libs[@]}"; do
        lib_args+=("-l$lib")
    done

    echo "================================== $output_name =================================="

    pushd "$build_dir" > /dev/null
    clang "${exe_flags[@]}" "${include_args[@]}" "${source_args[@]}" "${lib_args[@]}" -o "$output_name"
    popd > /dev/null

    echo "================================== $output_name build complete =================================="
}

# Build projects based on the project_to_build parameter
if [[ "$project_to_build" == "all" || "$project_to_build" == "ckg" ]]; then
    build_directory="./ckg_build_cl"
    test_directory="./Tests/ckg_build_cl"
    clean_directory "$build_directory"

    build_lib "ckg" "('../Include/ckg')" "('../Source/ckg/*.c')" "$build_directory"
    build_exe "ckg_test" "('../../Include/ckg')" "('../ckg/__test_ckg.c')" "('../../ckg_build_cl/ckg.a')" "$test_directory"
fi

if [[ "$project_to_build" == "all" || "$project_to_build" == "ckit" ]]; then
    build_directory="./ckit_build_cl"
    test_directory="./Tests/ckit_build_cl"
    clean_directory "$build_directory"

    build_lib "ckit" "('../Include/ckg' '../Include/ckit')" "('../ckg.c' '../ckit.c')" "$build_directory"
    build_exe "ckit_test" "('../../Include/ckg' '../../Include/ckit')" "('../ckit/ckit_test.c')" "('../../ckit_build_cl/ckit.a')" "$test_directory"
fi
