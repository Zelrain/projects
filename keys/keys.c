#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <aes.h>

#define KEY_FILE "keys.txt"
#define MASTER_PASS_MAXLEN 50
#define LINE_MAXLEN 1000


size_t get_pass (char *p_pass, size_t l_pass); 
void ignore_delete(int *last_key);

int main (int l_argv, char **s_argv) {
	char s_master[MASTER_PASS_MAXLEN] = {0};
	char *p_master = s_master;
	size_t lmax_master = MASTER_PASS_MAXLEN;
	size_t l_master = 0;


	printf("Enter master password:\n");
	l_master = get_pass(s_master, lmax_master);
	printf("%s\n", s_master);
	return 0;
}

int read_key_file(char *p_lines[]) {
	size_t l_lines = 0;
	FILE *file_keys;
	char file_char;

	if (file_keys = fopen(KEY_FILE, "r")) {
		while ((file_char = getc(file_keys)) != EOF) {
			if (file_char == '\n')
				l_lines++;
		}
		if (l_lines) {
			p_lines = calloc(l_lines, sizeof(char[LINE_MAXLEN]));
			rewind(file_keys);
			*p_lines = 
			while ((file_char = getc(file_keys)) != EOF) {

			}
		}
		fclose(file_keys);
	}
	return l_file_keys;
}

size_t get_pass (char *p_pass, size_t l_pass) {
	struct termios old, new;
	int l_input = 0;
	int last_key = 0;

	/* Turn echoing off and fail if we can’t. */
	if (tcgetattr (fileno(stdin), &old) != 0) {
		perror("");
		exit(EXIT_FAILURE);
	}
	new = old;
	new.c_lflag &= (~ICANON & ~ECHO);
	if (tcsetattr (fileno(stdin), TCSAFLUSH, &new) != 0) {
		perror("");
		exit(EXIT_FAILURE);
	}
	while ((last_key = getchar()) != '\n') {
		ignore_delete(&last_key);
		if (last_key == '\n')
			break;
		if (iscntrl(last_key)) {
			if (last_key == 8 || last_key == 127) {
				if (l_input) {
					*--p_pass = 0;
					printf("\b \b");
				}
			}
		}
		else {
			l_input++;
			*p_pass++ = last_key;
			printf("*");
		}
	}
	printf("\n");

	/* Restore terminal. */
	(void) tcsetattr (fileno(stdin), TCSAFLUSH, &old);

	return l_input;
}

void ignore_delete(int *last_key) {
	char s_delete_key[] = {27,91,51,126,0};
	char *p_delete_key = s_delete_key;
	if (*last_key == *p_delete_key++) {
		while ((*last_key = getchar()) == *p_delete_key++);
		ignore_delete(last_key);
	}
}
