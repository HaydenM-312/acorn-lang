#include "amain.h"

typedef signed char byte; 
typedef unsigned char ubyte; 

enum op_codes {
	OP_HALT,
	OP_GOTO,
	OP_CALL,
	OP_RETURN,
	OP_JUMP,
	OP_BCONST,
	OP_BPOP,
	OP_BADD,
	OP_BSUB,
	OP_BMULT,
	OP_BDIV,
	OP_BNEGATE,
	OP_BEQU,
	OP_BNEQ,
	OP_BGT,
	OP_BLT,
	OP_BLOAD,
	OP_BSTORE,
	OP_ICONST,
	OP_IPOP,
	OP_IADD,
	OP_ISUB,
	OP_IMULT,
	OP_IDIV,
	OP_INEGATE,
	OP_IEQU,
	OP_INEQ,
	OP_IGT,
	OP_ILT,
	OP_ILOAD,
	OP_ISTORE,
	OP_DCONST,
	OP_DPOP,
	OP_DADD,
	OP_DSUB,
	OP_DMULT,
	OP_DDIV,
	OP_DNEGATE,
	OP_DEQU,
	OP_DNEQ,
	OP_DGT,
	OP_DLT,
	OP_DLOAD,
	OP_DSTORE
};

typedef struct {
	byte* memory;
	byte* call;
	long pc;
	long sp;
	long fp;
	long cp;
} VM;

void init_vm(VM* vm) {
	vm->pc = 0;
	vm->sp = 0;
	vm->fp = 0xfffff;
	vm->memory = (byte*) malloc(0x100000 * sizeof(byte));
	vm->call = (byte*) malloc(0x10000 * sizeof(byte));
}

void clear_vm(VM* vm) {
	free(vm->memory);
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

byte push(VM *vm, byte data) {
	vm->memory[vm->sp++] = data;
	return data;
}

byte local_push(VM *vm, byte data) {
	vm->memory[vm->fp--] = data;
	return data;
}

byte pop(VM *vm) {
	return vm->memory[--vm->sp];
}

void print_vm(VM vm) {
	printf("[");
	for(long i = 0; i < vm.sp; i++) {
		printf(" %x", vm.memory[i]);
	}
	printf(" ]\t");
	printf("[");
	for(long i = 0xfffff; i > vm.fp; i--) {
		printf(" %x", vm.memory[i]);
	}
	printf(" ]\n");
}

int main(int argc, char* argv[]) {
	byte* code = read_bin(argv[1]);
	VM vm;
	init_vm(&vm);
	while (code[vm.pc] != OP_HALT) {
		if (vm.pc >= 0x100000) {
			fprintf(stderr, "Stack overflow.");
			exit(-1);
		}
		switch(code[vm.pc]) {
			// 1 Byte instructions
			case OP_BCONST:
				push(&vm, code[++vm.pc]);
				break;
			case OP_BSTORE:
				local_push(&vm, code[++vm.pc]);
				break;
			case OP_BPOP:
				pop(&vm);
				break;
			case OP_BNEGATE:
				push(&vm, -1 * pop(&vm));
				break;
			case OP_BADD:
				push(&vm, pop(&vm) + pop(&vm));
				break;
			case OP_BSUB:{
			 	byte a = pop(&vm);
			 	byte b = pop(&vm);
				push(&vm, b - a);
				break;}
			case OP_BMULT:
				push(&vm, pop(&vm) * pop(&vm));
				break;
			case OP_BDIV:{
			 	byte a = pop(&vm);
			 	byte b = pop(&vm);
				push(&vm, b /  a);
				break;}
			case OP_BGT:{
			 	byte a = pop(&vm);
			 	byte b = pop(&vm);
				if (b > a) {
					vm.pc = code[vm.pc+1]--;
				} else {
					vm.pc++;
				}
				break;
			}
			case OP_BLT:{
			 	byte a = pop(&vm);
			 	byte b = pop(&vm);
				if (b < a) {
					vm.pc = code[vm.pc+1]--;
				} else {
					vm.pc++;
				}
				break;
			}
			case OP_JUMP: 
				vm.pc += code[vm.pc+1]--;
				break;
			// 4 Byte instructions
			case OP_ICONST:
				push(&vm, code[++vm.pc]);
				push(&vm, code[++vm.pc]);
				break;
			case OP_IADD:{
				//push(&vm, pop(&vm) + pop(&vm));
				ubyte a = pop(&vm);
				ubyte b = pop(&vm);
				ubyte c = pop(&vm);
				ubyte d = pop(&vm);
				printf("0x%d\n", (short) (a | b << 8) + (short) (a | b << 8));
				break;}
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