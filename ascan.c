#pragma once
#include "main.h"

char* load_txt(char path[]) {
	FILE *fptr = fopen(path, "r");
	char* file_text;
	size_t size;
	size_t n = 0;
	int c;

	// If the file doesn't exist then return an error and exit the program
	if (!fptr) {
		fprintf(stderr, "No such file or directory found '%s'.", path);
		exit(-1);
	}

	fseek(fptr, 0L, SEEK_END); // Seek to end of file
	size = ftell(fptr);	// Set the size to the offset
	fseek(fptr, 0L, SEEK_SET); // Seek to beginning of file for loading into array
	file_text = (char*)malloc(size); // Allocate memory based on the size of the file
	
	if (!file_text) {
		fprintf(stderr, "Not enough memory");
		exit(-1);
	}

	while ((c = fgetc(fptr)) != EOF) 
		file_text[n++] = (char)c; // Read file and write it into the array character by character
	file_text[n] = '\0'; // Null-terminate the array

	if(!file_text) {
		fprintf(stderr, "Unable to read file");
		exit(-1);
	}

	fclose(fptr); // Close the file

	return file_text; // This returns a malloc'ed value, which NEEDS TO BE FREED
}
