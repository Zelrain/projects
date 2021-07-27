#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_LEN 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define MAX_INPUT_LEN 1000
#define SCRAMBLE_LEN 24

int has_dupe(int val, int dupes[], int *last_dupe);

int main(int argc, char *argv[]) {
	int cipher[ALPHABET_LEN];
	int i, j, k, input_len, last_dupe, last_scramble, alpha_num;
	char alpha[ALPHABET_LEN] = ALPHABET;
	char input[MAX_INPUT_LEN]; 
	int o_scramble[SCRAMBLE_LEN] = {0};
	int dupes[SCRAMBLE_LEN];
	char lower;

	for (i = 0; i < ALPHABET_LEN; i++) {
		cipher[i] = i + 1;
	}
	last_dupe = 0;
	last_scramble = 0;
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
						alpha_num = j + 1;
						if (j < SCRAMBLE_LEN && !has_dupe(alpha_num, dupes, &last_dupe)) {
							o_scramble[last_scramble++] = alpha_num;
						}
					}
				}
			} 
		}
	}
	for (i = 0; i < SCRAMBLE_LEN; i++) {
		if (!o_scramble[i]) {
			printf("%-2d X\n", i + 1);
		}
		else {
			printf("%-2d %-2d %c\n", i + 1, o_scramble[i], alpha[o_scramble[i] - 1]);
		}
	}
	return 0;
}

int has_dupe(int val, int dupes[], int *last_dupe) {
	int i;
	for (i = 0; i < *last_dupe; i++) {
		if (dupes[i] == val) {
			return 1;
		}
	}
	dupes[(*last_dupe)++] = val;
	return 0;
}
