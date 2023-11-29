# @file Makefile
# @author Christopher Scherling 12119060 <e12119060@student.tuwien.ac.at>
# @date 01.11.2023
#
# @brief Makefile for mycompress
## $@: Das, was vor dem Doppelpunkt steht
## $^: Das, was nach dem Doppelpunkt steht

project_name = exercise0
compiler = gcc
c_flags = -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g

all:$(project_name)

$(project_name): $(project_name).o
	$(compiler) $(c_flags) -o $@ $^

$(project_name).o: $(project_name).c
	$(compiler) $(c_flags) -o $@ -c $^

clean:
	rm -f $(project_name) *.o
