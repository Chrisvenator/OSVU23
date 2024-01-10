# @file Makefile
# @author Christopher Scherling 12119060 <e12119060@student.tuwien.ac.at>
# @date 01.11.2023
#
# @brief Makefile for cpair
## $@: Das, was vor dem Doppelpunkt steht
## $^: Das, was nach dem Doppelpunkt steht

project_name = server
compiler = gcc
c_flags = -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g


all: $(project_name)

# Define a generic rule to build an executable from its corresponding object file (%.o)
$(project_name): %: %.o
	$(compiler) $(c_flags) -o $@ $^ $(libs)

# Define a generic rule to build object files from source files
# %.o: %.c: This line specifies a pattern rule where the target (%.o) matches any file with the .o extension, and the prerequisite (%.c) matches any file with the .c extension.
# In other words, it tells Make that for any .o file, there should be a corresponding .c file.
%.o: %.c
	$(compiler) $(c_flags) -o $@ -c $< $(libs)

clean:
	rm -f $(project_name) *.o