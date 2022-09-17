#ifndef SEND_H
#define SEND_H

#include <stdlib.h>

/* Prompt is cyan. */
#define PROMPT "\x1b[36mmfrc522-mfccli $ \x1b[0m"

#define READ_USAGE                    \
	"Syntax: read <ITEM>\n"           \
	"    <ITEM>  uid atqa sak block"

#define READ_BLOCK_USAGE                               \
	"Syntax: read block <BLOCK NUMBER>\n"              \
	"    <BLOCK NUMBER>  decimal number from 0 to 63."

#define WRITE_USAGE                                                             \
	"Syntax: write <BLOCK NUMBER> <DATA>\n"                                     \
	"    <BLOCK NUMBER>  decimal number from 0 to 63.\n"                        \
	"    <DATA>          16 bytes' worth of hexadecimal data, without spaces."

#define AUTH_USAGE                                                           \
	"Syntax: auth <BLOCK NUMBER> <KEY A> <KEY B>\n"                          \
	"Syntax: auth A <BLOCK NUMBER> <KEY A>\n"                                \
	"Syntax: auth B <BLOCK NUMBER> <KEY B>\n"                                \
	"    <BLOCK NUMBER>  decimal number from 0 to 63.\n"                     \
	"    <KEY A>         6 bytes' worth of key A in hex, without spaces.\n"  \
	"    <KEY B>         6 bytes' worth of key B in hex, without spaces."    \

/* Verifies that some string consists entirely of decimal digits for n many
   characters. It must be unsigned. Returns 0 if false, 1 if true. */
int str_is_udec(char *str, int n)
{
	for (int i = 0; i < n; i++)  if (str[i] < '0' || str[i] > '9')  return 0;
	return 1;
}

/* Verifies that some string consists entirely of hexadecimal digits for n many
   characters. Returns 0 if false, 1 if true. */
int str_is_hex(char *str, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (str[i] < '0' || str[i] > 'f'
		    || (str[i] > '9' && str[i] < 'A')
		    || (str[i] > 'F' && str[i] < 'a'))
		{
			return 0;
		}
	}

	return 1;
}

/* Get a command and perform validation. Write the valid command to the cmd
   buffer. The len input is the maximum size of the cmd buffer. */
void get_command(char *cmd, int len)
{
	char *token;

	/* Return to caller upon valid input. */
	do
	{
		printf(PROMPT);

		/* Get input from the user and strip the newline. */
		fgets(cmd, len, stdin);
		cmd[strcspn(cmd, "\n")] = '\0';

		/* Empty input. */
		if (strlen(cmd) == 0)  continue;

		/* Tokenize. */
		token = strtok(cmd, " ");

		/* Handle exit. */
		if (strcmp(token, "exit") == 0)
		{
			/* Return to the caller. */
			return;
		}
		/* Handle read. */
		else if (strcmp(token, "read") == 0)
		{
			/* Get the next token. */
			if (!(token = strtok(NULL, " ")))
			{
				puts(READ_USAGE);
				continue;
			}

			/* Check that the token is valid. */
			     if (strcmp(token, "uid"  ) == 0)  return;
			else if (strcmp(token, "atqa" ) == 0)  return;
			else if (strcmp(token, "sak"  ) == 0)  return;
			else if (strcmp(token, "block") == 0)
			{
				/* Get the next token. */
				if (!(token = strtok(NULL, " ")))
				{
					puts(READ_BLOCK_USAGE);
					continue;
				}

				/* Validate that the token is in unsigned decimal and is from 0
				   to 63. */
				if (str_is_udec(token, strlen(token)) && atoi(token) < 64)
				{
					return;
				}
				else
				{
					puts(READ_BLOCK_USAGE);
					continue;
				}
			}

			puts(READ_USAGE);
		}
		/* Handle write. */
		else if (strcmp(token, "write") == 0)
		{
			/* Get the next token. */
			if (!(token = strtok(NULL, " ")))
			{
				puts(WRITE_USAGE);
				continue;
			}

			/* Validate that the token is in unsigned decimal and is from 0 to
			   63. */
			if (!(str_is_udec(token, strlen(token)) && atoi(token) < 64))
			{
				puts(WRITE_USAGE);
				continue;
			}

			/* Get the next token. */
			if (!(token = strtok(NULL, " ")))
			{
				puts(WRITE_USAGE);
				continue;
			}

			/* Validate that the token is in hex and is 32 digits long. */
			if (str_is_hex(token, strlen(token)) && strlen(token) == 32)
			{
				return;
			}

			puts(WRITE_USAGE);
		}
		/* Handle auth. */
		else if (strcmp(token, "auth") == 0)
		{
			/* Get the next token. */
			if (!(token = strtok(NULL, " ")))
			{
				puts(AUTH_USAGE);
				continue;
			}

			/* Validate dual-key auth. */
			if (str_is_udec(token, strlen(token)) && atoi(token) < 64)
			{
				/* Get the next token. */
				if (!(token = strtok(NULL, " ")))
				{
					puts(AUTH_USAGE);
					continue;
				}

				/* Validate that key A is in hex and is 12 digits long. */
				if (!(str_is_hex(token, strlen(token)) && strlen(token) == 12))
				{
					puts(AUTH_USAGE);
					continue;
				}

				/* Get the next token. */
				if (!(token = strtok(NULL, " ")))
				{
					puts(AUTH_USAGE);
					continue;
				}

				/* Validate that key B is in hex and is 12 digits long. */
				if (str_is_hex(token, strlen(token)) && strlen(token) == 12)
				{
					return;
				}
			}
			/* Validate single-key auth. */
			else if (strlen(token) == 1 && (token[0] == 'A' || token[0] == 'a'
			    || token[0] == 'B' || token[0] == 'b'))
			{
				/* Get the next token. */
				if (!(token = strtok(NULL, " ")))
				{
					puts(AUTH_USAGE);
					continue;
				}

				/* Validate that the token is in unsigned decimal and is from 0
				   to 63. */
				if (!str_is_udec(token, strlen(token)) && atoi(token) < 64)
				{
					puts(AUTH_USAGE);
					continue;
				}

				/* Get the next token. */
				if (!(token = strtok(NULL, " ")))
				{
					puts(AUTH_USAGE);
					continue;
				}

				/* Validate that the key is in hex and is 12 digits long. */
				if (str_is_hex(token, strlen(token)) && strlen(token) == 12)
				{
					return;
				}

				puts(AUTH_USAGE);
				continue;
			}

			puts(AUTH_USAGE);
			continue;
		}
		/* Unknown command. */
		else  puts("Unknown command.");
	}
	while (1);
}

/* Send a command to specified fd. Input will be written to the command buffer
   up to the specified maximum length. */
void send_command(int fd, char *command, int len)
{
	get_command(command, len);
}

#endif
