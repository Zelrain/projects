#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <search.h>

#define ALPHABET_LEN 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define MAX_INPUT_LEN 1000
#define SCRAMBLE_LEN 24


void process_input(char input[]); 
int chrcmp(const void *a, const void *b); 
void print_scramble(char uniques[], size_t last_unique);
int find_scramble_index(char letter, char *sorted_uniques, int stop); 
void print_unused(char *unused); 

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr,"%s expects a single string argument.\n", argv[0]);
		exit(1);
	} 
	else {
		process_input(argv[1]);
	}
	return 0;
}

void process_input(char input[]) {
	int input_len = strlen(input);
	int i;
	size_t last_unique = 0;
	char uniques[SCRAMBLE_LEN] = {0};
	char lower;

	for (i = 0; i < input_len; i++) {
		if (isalpha(input[i])
		&& (lower = tolower(input[i]))
		&& !lfind(&lower, uniques, &last_unique, sizeof(char), chrcmp)
		&& last_unique < SCRAMBLE_LEN) {
			uniques[last_unique++] = lower;
		}
	}
	print_scramble(uniques, last_unique);
}

int chrcmp(const void *a, const void *b) {

	return *(const char *)a - *(const char *)b;
}

void print_scramble(char uniques[], size_t last_unique) {
	char sorted_uniques[last_unique];
	int scramble[SCRAMBLE_LEN] = {0};
	char not_used[SCRAMBLE_LEN];
	int last_not_used = 0;
	char alphabet[] = ALPHABET;
	int alphabet_len = ALPHABET_LEN;
	int i;

	for (i = 0; i < alphabet_len; i++) {
		if (!lfind(&alphabet[i], uniques, &last_unique, sizeof(char), chrcmp)) {
			not_used[last_not_used++] = alphabet[i];
		}
	}
	strcpy(sorted_uniques, uniques);
	qsort(sorted_uniques, last_unique, sizeof(char), chrcmp);
	for (i = 0; i < last_unique; i++){
		scramble[i] = find_scramble_index(uniques[i], sorted_uniques, last_unique);
	}
	for (i = 0; i < SCRAMBLE_LEN; i++) {
		if (!scramble[i]) {
			printf("%-2d X  ", i + 1);
			print_unused(not_used);
			printf("\n");
		}
		else {
			printf("%-2d %-2d %c\n", i + 1, scramble[i], uniques[i]);
		}
	}
}

int find_scramble_index(char letter, char *sorted_uniques, int stop) {
	int index = 1;

	while (letter != *sorted_uniques++ && index < stop) 
		index++;
	return index;
}

void print_unused(char *unused) {
	char letter;

	if (letter = *unused++) {
		printf("%c", letter);
		while (letter = *unused++) 
			printf(", %c", letter);
	}
}
