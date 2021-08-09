#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <gpgme.h>
#include <locale.h>

#define KEY_FILE "keys.gpg"
#define LINE_LEN 1000

void fail_if_err(gpgme_error_t err);
void init_gpgme (gpgme_protocol_t proto);

int main (int l_argv, char **s_argv) {
	gpgme_ctx_t ctx;
	gpgme_error_t err;
	gpgme_data_t in, out;
	gpgme_encrypt_flags_t flags;
	FILE *file_keys;
	char **s_lines, **p_lines; 
	size_t l_lines;
	char file_char;

	file_keys = fopen(KEY_FILE, "r");
	while ((file_char = getc(file_keys)) != EOF)
		if (file_char == '\n') 
			l_lines++; 
	rewind(file_keys);
	s_lines = calloc(l_lines, sizeof(char*));
	free(s_lines);
	fclose(file_keys);

	init_gpgme(GPGME_PROTOCOL_OpenPGP);
	err = gpgme_new (&ctx);
	fail_if_err (err);
	err = gpgme_data_new_from_mem (&in, "Hallo Leute\n", 12, 0);
	fail_if_err (err);
	err = gpgme_data_new (&out);
	fail_if_err (err);
	err = gpgme_op_encrypt(ctx, 0, flags, in, out);
	fail_if_err(err);

	gpgme_data_release (in);
	gpgme_data_release (out);
	gpgme_release (ctx);
	return 0;
}

void fail_if_err(gpgme_error_t err) {								
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

