#!/bin/bash

build_dir="build/"
src_dir="src/"
output_file="makefile"

script_dir=$(dirname "$(realpath "$0")")
src_path="$script_dir/$src_dir"

# Clear the output file before starting
> "$output_file"

#Boilerplate
echo "CC=g++
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= main
" >> "$output_file"

# Build default target
echo -n "default:" >> "$output_file"
object_files=""

for file in $src_path*.cpp; do
  # Check if the file exists to avoid errors in case there are no .cpp files
  filename=$(basename "$file" .cpp)
  if [[ -f "$file" ]]; then
	object_files+=" $build_dir$filename.o"
  fi
done

echo -e "$object_files\n\t\$(CC) $object_files -o \$(EXENAME)\n" >> "$output_file"

# Process each source file
process_source_file() {
  local file=$1
  local filename
  filename=$(basename "$file" .cpp)

  # Write first line (object file dependencies)
  printf "%s$build_dir$filename.o: $src_dir$filename.cpp " >> "$output_file"

  # Collect modified lines from the include statements
  local modified_lines=""
  while read -r line; do
    if [[ "$line" =~ ^#include\ \" ]]; then
      # Extract everything after the first '/' and remove the last character (")
      modified_line="${line#*\/}"
      modified_line="${modified_line%\"}"  # Remove the last character (")
      modified_lines+="$modified_line "
    fi
  done < "$file"

  # Write dependencies and object file compilation rule
  printf "%s| build\n\t\$(CC) \$(CFLAGS) $src_dir$filename.cpp -o $build_dir$filename.o\n\n" "$modified_lines" >> "$output_file"
}

# Loop through all .cpp files and process them
for file in $src_path*.cpp; do
  process_source_file "$file"
  # Optionally print file name and modified includes to the terminal
  filename=$(basename "$file")
  echo "$filename"
done

# Boilerplate footer
echo -e "build:
	mkdir -p \$@

clean:
	rm $build_dir*.o \$(EXENAME)

run:
	./\$(EXENAME)
" >> "$output_file"
