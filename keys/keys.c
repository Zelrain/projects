#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <gpgme.h>
#include <locale.h>

#define KEY_FILE "keys.txt"
#define MASTER_PASS_MAXLEN 51
#define DELETE_SEQ {27,91,51,126,0}

size_t get_pass (char *p_pass, size_t l_pass); 
void ignore_delete(int *last_key);
void fail_if_err(gpgme_error_t err);
void init_gpgme (gpgme_protocol_t proto);

int main (int l_argv, char **s_argv) {
	char s_master[MASTER_PASS_MAXLEN] = {0};
	char *p_master = s_master;
	size_t lmax_master = MASTER_PASS_MAXLEN;
	size_t l_master = 0;
	gpgme_ctx_t ctx;
	gpgme_error_t err;
	gpgme_data_t in, out;
	gpgme_encrypt_flags_t flags;

	init_gpgme(GPGME_PROTOCOL_OpenPGP);
	err = gpgme_new (&ctx);
	fail_if_err (err);
	err = gpgme_data_new_from_mem (&in, "Hallo Leute\n", 12, 0);
	fail_if_err (err);
	err = gpgme_data_new (&out);
	fail_if_err (err);
	err = gpgme_op_encrypt(ctx, 0, flags, in, out);
	fail_if_err(err);

	/*printf("Enter master password:\n");
	l_master = get_pass(s_master, lmax_master);
	printf("%s\n", s_master);*/
	return 0;
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
	char s_delete_key[] = DELETE_SEQ;
	char *p_delete_key = s_delete_key;
	if (*last_key == *p_delete_key++) {
		while ((*last_key = getchar()) == *p_delete_key++);
		ignore_delete(last_key);
	}
}

void fail_if_err(gpgme_error_t err){								
	if (err)							
	{							
		fprintf (stderr, "%s: %s\n", gpgme_strsource (err), gpgme_strerror (err));			
		exit (EXIT_FAILURE);						
        }							
}								

void init_gpgme (gpgme_protocol_t proto) {
	gpgme_error_t err;

	gpgme_check_version (NULL);
	setlocale (LC_ALL, "");
	gpgme_set_locale (NULL, LC_CTYPE, setlocale (LC_CTYPE, NULL));
#ifndef HAVE_W32_SYSTEM
	gpgme_set_locale (NULL, LC_MESSAGES, setlocale (LC_MESSAGES, NULL));
#endif

	err = gpgme_engine_check_version (proto);
	fail_if_err (err);
}

