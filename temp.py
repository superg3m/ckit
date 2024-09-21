import re
import os

def expand_includes(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()

    while True:
        expanded_lines = []
        includes_found = False
        
        for line in lines:
            # Match #include "filename" or #include <filename>
            match = re.match(r'#include\s*["<](.+?)[">]', line)
            if match:
                include_file = match.group(1)
                if line.startswith('#include "'):
                    include_path = os.path.join(os.path.dirname(file_path), include_file)
                    if os.path.exists(include_path):
                        # Found a quoted include, mark that we need another pass
                        includes_found = True
                        with open(include_path, 'r') as include_f:
                            include_lines = include_f.readlines()
                            expanded_lines.extend(include_lines)
                    else:
                        print(f"Warning: Could not find include file '{include_file}'")
                else:
                    # Keep angle bracket includes as they are
                    expanded_lines.append(line)
            else:
                expanded_lines.append(line)

        # If no includes were found in this pass, we're done
        if not includes_found:
            break
        
        # Update lines for the next pass
        lines = expanded_lines

    return expanded_lines

def main():
    file_path = input("Enter the C file path: ")
    expanded_lines = expand_includes(file_path)

    output_file_path = "expanded_" + file_path
    with open(output_file_path, 'w') as f:
        f.writelines(expanded_lines)

    print(f"Expanded file written to '{output_file_path}'")

if __name__ == "__main__":
    main()
