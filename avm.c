#include "main.h"

typedef char byte;

enum opcodes {
  OP_BPUSH,
  OP_BPOP,
  OP_BADD,
  OP_BSUB,
  OP_BMULT,
  OP_BDIV,
  OP_BGT,
  OP_BLT,
  OP_BGTE,
  OP_BLTE,
  OP_BEQU,
  OP_BNEQ,
  OP_IPUSH,
  OP_IPOP,
  OP_IADD,
  OP_ISUB,
  OP_IMULT,
  OP_IDIV,
  OP_IGT,
  OP_ILT,
  OP_IGTE,
  OP_ILTE,
  OP_IEQU,
  OP_INEQ,
  OP_GOTO,
  OP_EOF
};


typedef struct virtual_machine {
  byte stack[0x10000];
  int sp;
  int pc;
} VM;

void init_vm(VM* vm) {
  vm->sp = 0;
  vm->pc = 0;
}

byte instructions[] = {
  OP_BPUSH, 1,
  OP_BPUSH, 3,
  OP_BSUB,
  OP_EOF
};

byte pop(VM* vm) {
  return vm->stack[--(vm->sp)];
}

void push(VM* vm, byte data) {
  vm->stack[(vm->sp)++] = data;
}

void print_stack(VM vm) {
  printf("[");
  for(int i = 0; i < vm.sp; i++) {
    printf(" %d ", vm.stack[i]);
  }
  printf("]\n");
}

void run_vm(byte instruct[], VM* vm) {
  while (instruct[vm->pc] != OP_EOF) {
    switch(instruct[vm->pc]) {
      // Byte Instructions
      case OP_BPUSH:
        push(vm, instruct[++vm->pc]);
        break;
      case OP_BADD:
        push(vm, pop(vm) + pop(vm));
        break;
      case OP_BSUB: {
        byte a = pop(vm);
        byte b = pop(vm);
        push(vm, b - a );
        break;
      }
      case OP_BMULT:
        push(vm, pop(vm) * pop(vm));
        break;
      case OP_BDIV: {
        byte a = pop(vm);
        byte b = pop(vm);
        push(vm, b / a );
        break;
      }
      case OP_BPOP:
        pop(vm);
        break;
      case OP_BEQU:
        if (pop(vm) == pop(vm)) {
          vm->pc = instruct[vm->pc+1]-1;
        }
        break;
      case OP_BNEQ:
        if (pop(vm) != pop(vm)) {
          vm->pc = instruct[vm->pc+1]-1;
        }
        break;
      case OP_BGT:
        if (pop(vm) > pop(vm)) {
          vm->pc = instruct[vm->pc+1]-1;
        }
        break;
      case OP_BLT:
        if (pop(vm) < pop(vm)) {
          vm->pc = instruct[vm->pc+1]-1;
        }
        break;
      case OP_BGTE:
        if (pop(vm) >= pop(vm)) {
          vm->pc = instruct[vm->pc+1]-1;
        }
        break;
      case OP_BLTE:
        if (pop(vm) <= pop(vm)) {
          vm->pc = instruct[vm->pc+1]-1;
        }
        break;
      default:break;
    }
    vm->pc++;
    print_stack(*vm);

  }
}


int main() {
  VM machine;
  init_vm(&machine);
  run_vm(instructions, &machine);
  printf("pc:%d, sp:%d\n", machine.pc, machine.sp);
}
