# @file Makefile
# @author Christopher Scherling 12119060 <e12119060@student.tuwien.ac.at>
# @date 01.11.2023
#
# @brief Makefile for mycompress

all:mycompress

mycompress: mycompress.o
	gcc -o mycompress mycompress.o

mycompress.o: mycompress.c
	gcc -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g -o mycompress.o -c mycompress.c

clean:
	rm -f mycompress *.o