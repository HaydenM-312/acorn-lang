#include "amain.h"

typedef signed char byte; 

enum op_codes {
	OP_HALT,
	OP_ICONST,
	OP_IADD,
	OP_ISUB,
	OP_IDIV,
	OP_IMULT,
	OP_IDUP,
	OP_IPOP,
	OP_ISTORE,
	OP_ILOAD,
	OP_GOTO,
	OP_IGTE,
	OP_ILTE,
	OP_IGTEQ,
	OP_ILTEQ,
	OP_IEQU,
	OP_INEQ,
	OP_RET,
	OP_CALL,
	OP_IAND,
	OP_IOR,
	OP_INOT,

};

typedef struct {
	byte* stack;
	long pc;
	long sp;
	long fp;
} VM;

void init_vm(VM* vm) {
	vm->pc = 0x0;
	vm->sp = 0x0;
	vm->fp = 0xfffff;
	vm->stack = (byte*) calloc(0x100000, sizeof(byte)); // Reserve one megabyte of RAM
}

void clear_vm(VM* vm) {
	free(vm->stack);
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
	for(int i = 0; i < vm.sp; i++) {
		printf(" %d", vm.stack[i]);
	}
	printf(" ]\n");
}

byte pop(VM* vm) {
	return vm->stack[--vm->sp];
}

void push(VM *vm, byte val) {
	vm->stack[vm->sp++] = val;
}

int int_rdiv(int denom, int numer) {
	return denom / numer;
}

int main(int argc, char* argv[]) {
	byte* code = read_bin(argv[1]);
	VM vm;
	init_vm(&vm);
	while (code[vm.pc] != OP_HALT) {
		switch(code[vm.pc]) {
			// 1 Byte instructions
			case OP_ICONST:
				push(&vm, code[++vm.pc]);
				break;
			case OP_IPOP:
				pop(&vm);
				break;
			case OP_IADD:
				push(&vm, pop(&vm) + pop(&vm));
				break;
			case OP_ISUB:
				push(&vm, pop(&vm) - pop(&vm));
				push(&vm, -(pop(&vm)));
				break;
			case OP_IMULT:
				push(&vm, pop(&vm) * pop(&vm));
				break;
			case OP_IDIV:
				push(&vm, int_rdiv(pop(&vm), pop(&vm)));
				break;
			case OP_IEQU:
				push(&vm, pop(&vm) == pop(&vm));
				break;
			case OP_INEQ:
				push(&vm, pop(&vm) != pop(&vm));
				break;
			case OP_IGTE:
				push(&vm, pop(&vm) < pop(&vm));
				break;
			case OP_ILTE:
				push(&vm, pop(&vm) > pop(&vm));
				break;
			case OP_IGTEQ:
				push(&vm, pop(&vm) <= pop(&vm));
				break;
			case OP_ILTEQ:
				push(&vm, pop(&vm) >= pop(&vm));
				break;
			case OP_INOT:
				push(&vm, !pop(&vm));
				break;
			default: break;
		}
		vm.pc++;
		printf("%d",code[vm.pc]);
		print_vm(vm);
	}
	clear_vm(&vm);
	free(code);
	return 0;
}
