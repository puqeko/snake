# Automake
# Generate a make file for a give target file

# Usage:
# python3 automake.py > Makefile

import re
import os
import datetime

target = 'game.c'
search_paths = ['../../drivers/', '../../drivers/avr/', '../../utils/', './lib/', './']
BUILD_COMMAND = "$(CC) -c $(CFLAGS) $< -o $@"

find_include = re.compile('#include "(.+)"')


def get_includes(path, file):
    f = open(path + file, 'r')
    includes = []

    for line in f:
        result = find_include.match(line)
        if result is not None:
            includes.append(result.group(1))

    f.close()
    return includes


def get_index(search_paths):

    files_in_index = {}

    for path in search_paths:
        for file in next(os.walk(path))[2]:
            files_in_index[file] = path

    return files_in_index


def generate_commands(file_index):
    commands = []
    object_files = []
    up_next = [target]
    c_files_located = set()

    while len(up_next) != 0:
        c_file = up_next.pop(-1)

        if c_file in c_files_located:
            continue

        c_files_located.add(c_file)
        headers_needed = set()

        if c_file in file_index:
            # Update headers_needed and up_next with the headers
            # this file needs. Save any associated .c files for
            # processing later.
            get_headers(c_file, file_index, headers_needed, up_next)
        else:
            print("No .c file {}.".format(c_file))

        # Create an object compile command for the Makefile
        # One is needed for every .c file
        commands.append(get_command(c_file, headers_needed))
        object_files.append(c_file.replace('.c', '.o'))

    return commands, object_files


def get_command(file, headers):
    file_locations = [file_index[file] + file]
    file_locations.extend([file_index[h] + h for h in headers])
    return "{}: {}".format(file.replace('.c', '.o'), " ".join(file_locations))


# Find any headers, and headers in those headers, that c_file needs
# Updates headers_needed and up_next
def get_headers(c_file, file_index, headers_needed, up_next):
    c_includes = get_includes(file_index[c_file], c_file)
    for include in c_includes:
        if include in file_index:

            # Find any headers included with headers
            headers_to_check = [include]
            while len(headers_to_check) != 0:
                cur_include = headers_to_check.pop(-1)
                if cur_include not in headers_needed:
                    headers_needed.add(cur_include)
                    headers_to_check.extend(get_includes(file_index[cur_include], cur_include))

            # Add any headers that match .c extensions
            include = include.replace('.h', '.c')
            if include in file_index:
                up_next.append(include)

        else:
            print("ERROR: Could not find '{}'.".format(file_to_find))


# MAIN

# Keep and index of all the accessible files in the search paths
file_index = get_index(search_paths)

# A line for each object file command,  List of object files needed
commands, object_files = generate_commands(file_index)

print("""# File:   Makefile
# Author: Autogenerated
# Date:   {}
# Descr:  Makefile for {}
""".format(datetime.datetime.now().strftime("%Y-%m-%d"), target))

print("""
# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g {}
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm
""".format(' '.join(["-I" + path for path in search_paths])))

print("""
# Default target.
all: {}
""".format(target.replace('.c', '.out')))

print("\n# Compile: create object files from C source files.")
for line in commands:
    print(line)
    print("\t{}\n".format(BUILD_COMMAND))

print("{}: {}".format(target.replace('.c', '.out'), " ".join(object_files)))
print("\t{}".format("$(CC) $(CFLAGS) $^ -o $@ -lm"))
print("\t$(SIZE) $@\n")

print("""
# Target: clean project.
.PHONY: clean
clean:
\t-$(DEL) *.o *.out *.hex
""")

print("""
# Target: program project.
.PHONY: program
program: {0}
\t$(OBJCOPY) -O ihex {0} {1}
\tdfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash {1}; dfu-programmer atmega32u2 start
""".format(target.replace('.c', '.out'), target.replace('.c', '.hex')))