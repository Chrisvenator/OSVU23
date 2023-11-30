#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
	char *iban = "ABCD1234";
	printf("%s\n", iban);

	int length = strlen(iban);

	char newban [length+1];

	memcpy(newban, iban + 4, length - 4);
	memcpy(newban + length - 4, iban, 4);
	newban[length] = '\0';


	printf("%s\n\n", newban);

	char *firstName = "John";
	char *secondName = "Cenaaaaaaaaaaaa";
	char name[strlen(firstName) + strlen(secondName) + 2];

	memcpy(name, firstName, strlen(firstName));
	name[strlen(firstName)] = ' ';
	memcpy(name + 1 + strlen(firstName), secondName, strlen(secondName));
	name[strlen(name) + strlen(secondName)] = '\0';

	printf("%s\n", name);
	char name2 [strlen(firstName) + strlen(secondName) + 2];
	strcpy(name2, firstName);


	printf("%s\n", name2);


        exit(0);
}
