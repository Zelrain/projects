#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_LEN 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define MAX_INPUT_LEN 1000

int main(int argc, char *argv[]){
	int cipher[ALPHABET_LEN];
	int i, j, is_valid, input_len;
	char alpha[ALPHABET_LEN] = ALPHABET;
	char input[MAX_INPUT_LEN]; 
	int output[MAX_INPUT_LEN];
	char lower;

	for(i = 0; i < ALPHABET_LEN; i++){
		cipher[i] = i + 1;
	}

	is_valid = 1;
	if(argc != 2){
		is_valid = 0;
	} else {
		strcpy(input, argv[1]);
		input_len  = strlen(input);
		for(i = 0; i < input_len; i++){
			if(isalpha(input[i])){
				lower = tolower(input[i]);
				for(j = 0; j < ALPHABET_LEN; j++){
					if(alpha[j] == lower){
						output[i] = j + 1;
					}
				}
			} else if(isspace(input[i])){
				output[i] = 0;
			} else {
				is_valid = 0;
				break;
			}
		}
	}

	if(is_valid){
		for(i = 0; i < input_len; i++){
			printf("%c %d\n", input[i], output[i]);
		}
	} else {
		fprintf(stderr,"%s expects a single string argument containing only alphabet letters and spaces.\n", argv[0]);
		exit(1);
	}

	return 0;
}
