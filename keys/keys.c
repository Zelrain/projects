#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include "aes.h"
#include <argon2.h>
#include <string.h>

#define KEY_FILE "keys.txt"
#define MASTER_PASS_MAXLEN 50
#define NOTES_MAXLEN 1000
#define DOMAIN_MAXLEN 1000
#define USERNAME_MAXLEN 100
#define PASSWORD_MAXLEN 1000


size_t get_pass (char *p_pass, size_t l_pass); 
void ignore_delete(int *last_key);
void hash_pass (uint8_t hash[AES_KEYLEN], uint8_t *pass, uint32_t pass_len);
void panic(char *error);

typedef struct record {
	char domain[DOMAIN_MAXLEN];
	char username[USERNAME_MAXLEN];
	char password[PASSWORD_MAXLEN];
	char notes[NOTES_MAXLEN];
	struct record *next;
} Record;

int main (int l_argv, char **s_argv) {
	char s_master[MASTER_PASS_MAXLEN] = {0};
	size_t lmax_master = MASTER_PASS_MAXLEN;
	size_t l_master = 0;
	uint8_t key[AES_KEYLEN] = {0}, iv[AES_BLOCKLEN] = {0};
	struct AES_ctx ctx;

	printf("Enter master password:\n");
	l_master = get_pass(s_master, lmax_master);
	hash_pass(key, (uint8_t *)s_master, l_master);

	AES_init_ctx(&ctx, key);
	char testbufp[] = "bia bhia ohoh";
	AES_ctx_set_iv(&ctx, iv);
	AES_CTR_xcrypt_buffer(&ctx, (uint8_t *)testbufp, sizeof(testbufp) / sizeof(testbufp[0]));
	AES_ctx_set_iv(&ctx, iv);
	AES_CTR_xcrypt_buffer(&ctx, (uint8_t *)testbufp, sizeof(testbufp) / sizeof(testbufp[0]));
	return 0;
}

Record *read_key_file(void) {
	FILE *file_keys;

	if ((file_keys = fopen(KEY_FILE, "r"))) {
		fclose(file_keys);
	}
	return NULL;
}

void panic(char *error) {
	perror(error);
	exit(EXIT_FAILURE);
}

void hash_pass (uint8_t hash[AES_KEYLEN], uint8_t *pass, uint32_t pass_len) {
	uint8_t salt[AES_KEYLEN];
	uint32_t t_cost =2;
	uint32_t m_cost = (1<<16);
	uint32_t parallelism = 1;

	memset(salt, 0x00, AES_KEYLEN);
	argon2i_hash_raw(t_cost, m_cost, parallelism, pass, pass_len, salt, AES_KEYLEN, hash, AES_KEYLEN);
}

size_t get_pass (char *p_pass, size_t l_pass) {
	struct termios old, new;
	int l_input = 0;
	int last_key = 0;

	/* Turn echoing off and fail if we can’t. */
	if (tcgetattr (fileno(stdin), &old) != 0) {
		panic("");
	}
	new = old;
	new.c_lflag &= (~ICANON & ~ECHO);
	if (tcsetattr (fileno(stdin), TCSAFLUSH, &new) != 0) {
		panic("");
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
