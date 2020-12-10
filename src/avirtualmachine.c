#include "amain.h"

enum op_codes {
	OP_EOF,
	OP_GOTO,
	OP_SCONST,
	OP_SPOP,
	OP_SLOAD,
	OP_SADD,
	OP_SSUB,
	OP_SMULT,
	OP_SDIV,
	OP_SNEGATE,
	OP_SEQU,
	OP_SNEQ,
	OP_SGT,
	OP_SLT,
	OP_ICONST,
	OP_IPOP,
	OP_ILOAD,
	OP_IADD,
	OP_ISUB,
	OP_IMULT,
	OP_IDIV,
	OP_INEGATE,
	OP_IEQU,
	OP_INEQ,
	OP_IGT,
	OP_ILT
};

typedef struct {
	signed short* memory;
	long pc;
	long sp;
	long fp;
} VM;

void init_vm(VM* vm) {
	vm->pc = 0;
	vm->sp = 0;
	vm->fp = 0;
	vm->memory = (signed short*) malloc(0x100000 * sizeof(short));
}

void clear_vm(VM* vm) {
	free(vm->memory);
}

short* read_bin(char path[]) {
	FILE *fptr = fopen(path, "rb");
	short* file;
	size_t size; 

	// If the file doesn't exist then return an error and exit the program
	if (!fptr) {
		fprintf(stderr, "No such file or directory found '%s'.", path);
		exit(-1);
	}

	fseek(fptr, 0L, SEEK_END); // Seek to end of file
	size = ftell(fptr);	// Set the size to the offset
	fseek(fptr, 0L, SEEK_SET); // Seek to beginning of file for loading into array
	file = (signed short*) malloc(sizeof(short)*size); // Allocate memory based on the size of the file

	if (!file) {
		fprintf(stderr, "Not enough memory");
		exit(-1);
	}
	
	fread(file, sizeof(signed short), size, fptr); // Read file and write it into the array

	if(file == NULL) {
		fprintf(stderr, "Unable to read file");
		exit(-1);
	}
	fclose(fptr); // Close the file
	return file; // This returns a malloc'ed value, which NEEDS TO BE FREED
}

signed short push(VM *vm, signed short data) {
	vm->memory[vm->sp++] = data;
	return data;
}
signed short pop(VM *vm) {
	return vm->memory[--vm->sp];
}

void print_vm(VM vm) {
	printf("[");
	for(long i = 0; i < vm.sp; i++) {
		printf(" %d", vm.memory[i]);
	}
	printf(" ]\n");
}

int main(int argc, char* argv[]) {
	signed short* code = read_bin(argv[1]);
	VM vm;
	init_vm(&vm);
	while (code[vm.pc] != OP_EOF) {
		switch(code[vm.pc]) {
			case OP_SCONST:
				push(&vm, code[++vm.pc]);
				break;
			case OP_SPOP:
				pop(&vm);
				break;
			case OP_SNEGATE:
				push(&vm, -1 * pop(&vm));
				break;
			case OP_SADD:
				push(&vm, pop(&vm) + pop(&vm));
				break;
			case OP_SSUB:{
			 	signed short a = pop(&vm);
			 	signed short b = pop(&vm);
				push(&vm, b - a);
				break;}
			case OP_SMULT:
				push(&vm, pop(&vm) * pop(&vm));
				break;
			case OP_SDIV:{
			 	signed short a = pop(&vm);
			 	signed short b = pop(&vm);
				push(&vm, b /  a);
				break;}
			case OP_SGT:{
			 	signed short a = pop(&vm);
			 	signed short b = pop(&vm);
				if (b > a) {
					vm.pc = code[vm.pc+1]--;
				} else {
					vm.pc++;
				}
				break;
			}
			default: break;
		}
		printf("%d \t", vm.pc);
		print_vm(vm);
		vm.pc++;
	}
	clear_vm(&vm);
	free(code);
	return 0;
}