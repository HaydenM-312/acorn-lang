#include "amain.h"

int main(int argc, char* argv[]) {
		if (argc > 1) {
		Token* tokens = tokenize(argv[1],0); // Load the file named in the first argument into an array
		Tree syntax;
		make_ast(tokens, &syntax);

		print_token(*(syntax.node));
		for(int i = 0; tokens[i-1].type != TOKEN_EOF; i++) {
			printf("%d: ", i);
			print_token(tokens[i]);
			free_token(&tokens[i]);
		}

		free(tokens);
		free_ast(&syntax);
		if (err) return(-1);

	} else {
		while (1) {
			char in_text[1024];
			printf("$ ");
			fgets(in_text,1024,stdin);
			Token* tokens = tokenize(in_text,1); // Turn the array into a list of tokens

			for(int i = 0; tokens[i-1].type != TOKEN_EOF; i++) {
				printf("%d: ", i);
				print_token(tokens[i]);
				free_token(&tokens[i]);
			}
		}
	}
	return 0;
}
