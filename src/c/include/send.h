#ifndef SEND_H
#define SEND_H

/* Prompt is cyan. */
#define PROMPT "\x1b[36mmfrc522-mfccli $ \x1b[0m"

#define HELP \
	"Syntax: help <COMMAND>\n"               \
	"    <COMMAND>  read write auth detect reset"

#define READ_USAGE \
	"Syntax: read <ITEM>\n"           \
	"    <ITEM>  uid atqa sak block"

#define READ_BLOCK_USAGE \
	"Syntax: read block <BLOCK NUMBER>\n"               \
	"    <BLOCK NUMBER>  decimal number from 0 to 63."

#define WRITE_USAGE \
	"Syntax: write <BLOCK NUMBER> <DATA>\n"                \
	"    <BLOCK NUMBER>  decimal number from 0 to 63.\n"   \
	"    <DATA>          16 bytes' worth of data in hex."

#define AUTH_USAGE \
	"Syntax: auth <BLOCK NUMBER> <KEY A> <KEY B>\n"          \
	"Syntax: auth A <BLOCK NUMBER> <KEY A>\n"                \
	"Syntax: auth B <BLOCK NUMBER> <KEY B>\n"                \
	"    <BLOCK NUMBER>  decimal number from 0 to 63.\n"     \
	"    <KEY A>         6 bytes' worth of key A in hex.\n"  \
	"    <KEY B>         6 bytes' worth of key B in hex."

#define DETECT_USAGE \
	"Syntax: detect card"

#define RESET_USAGE \
	"Syntax: reset"

#define P_HELP()              { puts(HELP)              ; continue; }
#define P_READ_USAGE()        { puts(READ_USAGE)        ; continue; }
#define P_READ_BLOCK_USAGE()  { puts(READ_BLOCK_USAGE)  ; continue; }
#define P_WRITE_USAGE()       { puts(WRITE_USAGE)       ; continue; }
#define P_AUTH_USAGE()        { puts(AUTH_USAGE)        ; continue; }
#define P_DETECT_USAGE()      { puts(DETECT_USAGE)      ; continue; }
#define P_RESET_USAGE()       { puts(RESET_USAGE)       ; continue; }

#define IS_AB(s)       (strlen(s) == 1 && (s[0] == 'a' || s[0] == 'A' || s[0] == 'b' || s[0] == 'B'))
#define IS_DEC(c)      (c >= '0' && c <= '9')
#define IS_HEX(c)      ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
#define IS_HEX_L12(s)  (str_is_hex(s, strlen(s)) && strlen(s) == 12)
#define IS_HEX_L32(s)  (str_is_hex(s, strlen(s)) && strlen(s) == 32)
#define IS_DEC_M64(s)  (str_is_udec(s, strlen(s)) && atoi(s) < 64)

#define FREE_RET(p)    { free(p); return; }

/* Verifies that some string consists entirely of decimal digits for n many
   characters and is unsigned. Returns 0 if false, 1 if true. */
uint8_t str_is_udec(char *str, uint8_t n)
{
	for (uint8_t i = 0; i < n; i++)  if (!IS_DEC(str[i]))  return 0;
	return 1;
}

/* Verifies that some string consists entirely of hexadecimal digits for n many
   characters. Returns 0 if false, 1 if true. */
uint8_t str_is_hex(char *str, uint8_t n)
{
	for (uint8_t i = 0; i < n; i++)  if (!IS_HEX(str[i]))  return 0;
	return 1;
}

/* Convert a hex string of length n into bytes written into the dest. */
void hexstr_to_bytes(char *dest, char *str, uint8_t n)
{
	uint8_t val;
	char buffer;

	for (uint8_t i = 0; i < n; i++)
	{
		buffer = str[i];

		/* Standardize lowercase to capital. */
		if (buffer >= 'a' && buffer <= 'f')  buffer -= 'a' - 'A';

		/* Compute the value of the hex digit. */
		     if (buffer >= '0' && buffer <= '9')  val = buffer - '0';
		else if (buffer >= 'A' && buffer <= 'F')  val = 10 + buffer - 'A';

		/* Write the bytes. */
		if (i % 2 == 0)  dest[i/2] |= val << 4;
		else             dest[i/2] |= val;
	}
}

/* Get a command and perform validation. Write the valid command to the cmd
   buffer. The len input is the maximum size of the cmd buffer. */
void get_command(char *command, uint8_t len)
{
	char *cmd;
	char *token;

	cmd = (char *)calloc(len, sizeof(char));

	/* Return to caller upon valid input. */
	while (1)
	{
		printf(PROMPT);

		/* Get input from the user and strip the newline. */
		fgets(cmd, len, stdin);
		cmd[strcspn(cmd, "\n")] = '\0';

		/* Empty input. */
		if (strlen(cmd) == 0)  continue;

		memset(command, 0, len);
		strncpy(command, cmd, len);

		/* Tokenize. */
		token = strtok(cmd, " ");

		/* exit */
		if (strcmp(token, "exit") == 0)  FREE_RET(cmd)
		/* help */
		else if (strcmp(token, "help") == 0 || strcmp(token, "?") == 0)
		{
			/* Get the next token. */
			if (!(token = strtok(NULL, " ")))
				P_HELP();

			     if (strcmp(token, "read"  ) == 0)  P_READ_USAGE()
			else if (strcmp(token, "write" ) == 0)  P_WRITE_USAGE()
			else if (strcmp(token, "auth"  ) == 0)  P_AUTH_USAGE()
			else if (strcmp(token, "detect") == 0)  P_DETECT_USAGE()
			else if (strcmp(token, "reset" ) == 0)  P_RESET_USAGE()

			P_HELP();
		}
		/* read */
		else if (strcmp(token, "read") == 0)
		{
			/* Get the next token. */
			if (!(token = strtok(NULL, " ")))
				P_READ_USAGE();

			/* Check that the token is valid. */
			     if (strcmp(token, "uid"  ) == 0)  FREE_RET(cmd)
			else if (strcmp(token, "atqa" ) == 0)  FREE_RET(cmd)
			else if (strcmp(token, "sak"  ) == 0)  FREE_RET(cmd)
			else if (strcmp(token, "block") == 0)
			{
				/* Get the next token. */
				if (!(token = strtok(NULL, " ")))
					P_READ_BLOCK_USAGE();

				/* Validate that the token is unsigned decimal < 64. */
				if (IS_DEC_M64(token))  FREE_RET(cmd)
				else  P_READ_BLOCK_USAGE();
			}

			P_READ_USAGE();
		}
		/* write */
		else if (strcmp(token, "write") == 0)
		{
			/* Get the next token. */
			if (!(token = strtok(NULL, " ")))
				P_WRITE_USAGE();

			/* Validate that the token is unsigned decimal < 64. */
			if (!IS_DEC_M64(token))
				P_WRITE_USAGE();

			/* Get the next token. */
			if (!(token = strtok(NULL, " ")))
				P_WRITE_USAGE();

			/* Validate that the token is hex length 32. */
			if (IS_HEX_L32(token))
				FREE_RET(cmd)

			P_WRITE_USAGE();
		}
		/* auth */
		else if (strcmp(token, "auth") == 0)
		{
			/* Get the next token. */
			if (!(token = strtok(NULL, " ")))
				P_AUTH_USAGE();

			/* Validate dual-key auth. */
			if (IS_DEC_M64(token))
			{
				/* Get the next token. */
				if (!(token = strtok(NULL, " ")))
					P_AUTH_USAGE();

				/* Validate that key A is hex length 12. */
				if (!IS_HEX_L12(token))
					P_AUTH_USAGE();

				/* Get the next token. */
				if (!(token = strtok(NULL, " ")))
					P_AUTH_USAGE();

				/* Validate that key B is hex length 12. */
				if (IS_HEX_L12(token))
					FREE_RET(cmd)
			}
			/* Validate single-key auth. */
			else if (IS_AB(token))
			{
				/* Get the next token. */
				if (!(token = strtok(NULL, " ")))
					P_AUTH_USAGE();

				/* Validate that the token is unsigned decimal < 64. */
				if (!IS_DEC_M64(token))
					P_AUTH_USAGE();

				/* Get the next token. */
				if (!(token = strtok(NULL, " ")))
					P_AUTH_USAGE();

				/* Validate that key is in hex and is 12 digits long. */
				if (IS_HEX_L12(token))
					FREE_RET(cmd)
			}

			P_AUTH_USAGE();
		}
		/* detect */
		else if (strcmp(token, "detect") == 0)
		{
			if (!(token = strtok(NULL, " ")))
				P_DETECT_USAGE();

			if (strcmp(token, "card") == 0)
				FREE_RET(cmd)

			P_DETECT_USAGE();
		}
		/* reset */
		else if (strcmp(token, "reset") == 0)
		{
			FREE_RET(cmd)
		}
		/* Unknown command. */
		else
		{
			puts("Unknown command.");
		}
	}
}

/* Send a command to specified fd. Input will be written to the command buffer
   up to the specified maximum length. */
void send_command(int8_t fd, char *command, uint8_t len, uint8_t debug)
{
	char *token;
	char buffer[32];
	uint8_t size;
	uint8_t temp;

	memset(buffer, 0, 32);

	/* Get a validated user command. */
	get_command(command, len);
	token = strtok(command, " ");

	/* exit */
	if (strcmp(token, "exit") == 0)  return;
	/* read */
	else if (strcmp(token, "read") == 0)
	{
		token = strtok(NULL, " ");

		if (strcmp(token, "uid") == 0)
		{
			buffer[0] = COMMAND_READ_UID;
			size = 1;
		}
		else if (strcmp(token, "atqa") == 0)
		{
			buffer[0] = COMMAND_READ_ATQA;
			size = 1;
		}
		else if (strcmp(token, "sak") == 0)
		{
			buffer[0] = COMMAND_READ_SAK;
			size = 1;
		}
		else if (strcmp(token, "block") == 0)
		{
			buffer[0] = COMMAND_READ_BLOCK;

			token = strtok(NULL, " ");
			buffer[1] = atoi(token);

			size = 2;
		}
	}
	/* write */
	else if (strcmp(token, "write") == 0)
	{
		buffer[0] = COMMAND_WRITE;

		token = strtok(NULL, " ");
		buffer[1] = atoi(token);

		token = strtok(NULL, " ");
		hexstr_to_bytes(buffer+2, token, strlen(token));

		size = 18;
	}
	/* auth */
	else if (strcmp(token, "auth") == 0)
	{
		token = strtok(NULL, " ");

		/* Dual-key. */
		if (IS_DEC_M64(token))
		{
			buffer[0] = COMMAND_AUTHENTICATE;

			temp = atoi(token);
			buffer[1] = temp + (3-temp % 4);

			token = strtok(NULL, " ");
			hexstr_to_bytes(buffer+2, token, strlen(token));
			token = strtok(NULL, " ");
			hexstr_to_bytes(buffer+8, token, strlen(token));

			size = 14;
		}
		/* Single key. */
		else
		{
			if (strcmp(token, "A") == 0)  buffer[0] = COMMAND_AUTHENTICATE_A;
			else                          buffer[0] = COMMAND_AUTHENTICATE_B;

			token = strtok(NULL, " ");
			temp = atoi(token);
			buffer[1] = temp + (3-temp % 4);

			token = strtok(NULL, " ");
			hexstr_to_bytes(buffer+2, token, strlen(token));

			size = 8;
		}
	}
	/* detect */
	else if (strcmp(token, "detect") == 0)
	{
		buffer[0] = COMMAND_DETECT_CARD;
		size = 1;
	}
	/* reset */
	else if (strcmp(token, "reset") == 0)
	{
		buffer[0] = COMMAND_RESET;
		size = 1;
	}

	if (debug)
	{
		printf("DBG: Sending %d byte(s) of data through serial.\n", size);
		printf("DBG: -> ");
		for (uint8_t i = 0; i < size; i++) printf("%02X ", (unsigned char)buffer[i]);
		printf("\n");
	}

	/* Printing output. */
	switch (buffer[0])
	{
		case COMMAND_DETECT_CARD:
			printf("Detecting card...");
			fflush(stdout);
			break;

		case COMMAND_AUTHENTICATE:
		case COMMAND_AUTHENTICATE_A:
		case COMMAND_AUTHENTICATE_B:
			printf("Authenticating to block %d (trailer block %d)...", temp, buffer[1]);
			fflush(stdout);
			break;

		case COMMAND_WRITE:
			printf("Writing data...");
			fflush(stdout);
			break;

		case COMMAND_RESET:
			printf("Resetting crypto...");
			fflush(stdout);
			break;
	}

	write(fd, buffer, size);
}

#endif
