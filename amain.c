#include "main.h"

int main(int argc, char* argv[]) {
	char* 	file = load_txt(argv[1]); // Load the file named in the first argument into an array
	char**	tokens = tokenize(file); // Turn the array into a list of tokens

	for(int i = 0; i < sizeof(tokens)/sizeof(tokens[0]); i++) {
		printf(tokens[i]);
		free(tokens[i]);
	}
	
	free(tokens);
	free(file); // Free the malloc'ed file
	return 0;
} 
