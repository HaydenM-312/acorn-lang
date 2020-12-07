#include "main.h"

int main(int argc, char* argv[]) {
	char* 	file = load_txt(argv[1]); // Load the file named in the first argument into an array
	char**	tokens = tokenize(file); // Turn the array into a list of tokens


	free(tokens);
	free(file); // Free the malloc'ed file
	return 0;
}
