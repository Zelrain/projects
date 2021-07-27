#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_LEN 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define MAX_INPUT_LEN 1000
#define SCRAMBLE_LEN 24

int main(int argc, char *argv[]) {
	int cipher[ALPHABET_LEN];
	int i, j, input_len, last_dupe, last_output;
	char alpha[ALPHABET_LEN] = ALPHABET;
	char input[MAX_INPUT_LEN]; 
	int output[MAX_INPUT_LEN];
	int output2[SCRAMBLE_LEN], dupes[SCRAMBLE_LEN];
	char lower;

	for (i = 0; i < ALPHABET_LEN; i++) {
		cipher[i] = i + 1;
	}

	last_dupe = 0;
	last_output = 0;
	if (argc != 2) {
		fprintf(stderr,"%s expects a single string argument.\n", argv[0]);
		exit(1);
	} 
	else {
		strcpy(input, argv[1]);
		input_len  = strlen(input);
		for (i = 0; i < input_len; i++) {
			if (isalpha(input[i])) {
				lower = tolower(input[i]);
				for (j = 0; j < ALPHABET_LEN; j++) {
					if (alpha[j] == lower) {
						output[i] = j + 1;
					}
				}
			} 
			else output[i] = 0;
		}
	}

	for (i = 0; i < input_len; i++) {
		printf("%c %d\n", input[i], output[i]);
	}

	return 0;
}
