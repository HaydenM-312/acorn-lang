#include "main.h"

int main(int argc, char* argv[]) {
		if (argc > 1) {
		char* 	file = load_txt(argv[1]); // Load the file named in the first argument into an array
		Token* 	tokens = tokenize(file); // Turn the array into a list of tokens

		for(int i = 0; tokens[i-1].type != TOKEN_EOF; i++) {
			printf("%d: ", i);
			print_token(tokens[i]);
			free_token(&tokens[i]);
		}

		free(tokens);
		free(file); // Free the malloc'ed file
		if (err) return(-1);

	} else {
		while (1) {
			char in_text[1024];
			printf("> ");
			fgets(in_text,1024,stdin);
			Token* tokens = tokenize(in_text); // Turn the array into a list of tokens

			for(int i = 0; tokens[i-1].type != TOKEN_EOF; i++) {
				printf("%d: ", i);
				print_token(tokens[i]);
				free_token(&tokens[i]);
			}
		}
	}
	return 0;
}
