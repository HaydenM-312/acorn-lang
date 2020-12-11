#include "amain.h"

typedef signed char byte; 

enum op_codes {
	OP_HALT,
	OP_LOADI,
	OP_LOADA,
	OP_STORE,
	OP_ADD,
	OP_MULT,
	OP_SUB,
	OP_DIV

};

typedef struct {
	byte* memory;
	byte* call;
	byte* registers;
	long pc;
	long sp;
} VM;

void init_vm(VM* vm) {
	vm->pc = 0;
	vm->sp = 0xfffff;
	vm->memory = (byte*) calloc(0x100000, sizeof(byte)); // Reserve one megabyte of RAM
	vm->registers = (byte*) calloc(0x8, sizeof(byte)); // Create 8 registers
}

void clear_vm(VM* vm) {
	free(vm->memory);
	free(vm->registers);
}

byte* read_bin(char path[]) {
	FILE *fptr = fopen(path, "rb");
	byte* file;
	size_t size; 

	// If the file doesn't exist then return an error and exit the program
	if (!fptr) {
		fprintf(stderr, "No such file or directory found '%s'.", path);
		exit(-1);
	}

	fseek(fptr, 0L, SEEK_END); // Seek to end of file
	size = ftell(fptr);	// Set the size to the offset
	fseek(fptr, 0L, SEEK_SET); // Seek to beginning of file for loading into array
	file = (byte*) malloc(sizeof(byte)*size); // Allocate memory based on the size of the file

	if (!file) {
		fprintf(stderr, "Not enough memory");
		exit(-1);
	}
	
	fread(file, sizeof(byte), size, fptr); // Read file and write it into the array

	if(file == NULL) {
		fprintf(stderr, "Unable to read file");
		exit(-1);
	} 
	fclose(fptr); // Close the file
	return file; // This returns a malloc'ed value, which NEEDS TO BE FREED
	
}

void print_vm(VM vm) {
	printf("[");
	for(int i = 0; i < 8; i++) {
		printf(" %d", vm.registers[i]);
	}
	printf(" ] ");
	printf("[");
	for(int i = 0; i < 8; i++) {
		printf(" %d", vm.memory[i]);
	}
	printf(" ]\n");
}

int main(int argc, char* argv[]) {
	byte* code = read_bin(argv[1]);
	VM vm;
	init_vm(&vm);
	while (code[vm.pc] != OP_HALT) {
		switch(code[vm.pc]) {
			// 1 Byte instructions
			case OP_LOADI:
				vm.registers[code[++vm.pc]] = code[++vm.pc];
				break;
			case OP_LOADA:
				vm.registers[code[++vm.pc]] = vm.memory[code[++vm.pc]];
				break;
			case OP_STORE:
				vm.memory[code[2+vm.pc]] = vm.registers[code[1+vm.pc]];
				vm.pc += 2;
				break;
			case OP_ADD:
				vm.registers[code[3+vm.pc]] = vm.registers[code[1+vm.pc]] + vm.registers[code[2+vm.pc]];
				vm.pc += 3;
				break;
			case OP_MULT:
				vm.registers[code[3+vm.pc]] = vm.registers[code[1+vm.pc]] * vm.registers[code[2+vm.pc]];
				vm.pc += 3;
				break;
			case OP_SUB:
				vm.registers[code[3+vm.pc]] = vm.registers[code[1+vm.pc]] - vm.registers[code[2+vm.pc]];
				vm.pc += 3;
				break;
			case OP_DIV:
				vm.registers[code[3+vm.pc]] = vm.registers[code[1+vm.pc]] / vm.registers[code[2+vm.pc]];
				vm.pc += 3;
				break;
			default: break;
		}
		vm.pc++;
		printf("%d",vm.pc);
		print_vm(vm);
	}
	clear_vm(&vm);
	free(code);
	return 0;
}
