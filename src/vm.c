#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "vm.h"
#include "debug.h"
#include "memory.h"

VM vm;

static void resetStack() {
    vm.stackCount = 0;
}

void initVM() {
    vm.stack = NULL;
    vm.stackCapacity = 0;
    resetStack();
}

void freeVM() {}

void push(Value value) {
    if (vm.stackCapacity < vm.stackCount + 1) {
        int oldCapacity = vm.stackCapacity;
        vm.stackCapacity = GROW_CAPACITY(oldCapacity);
        vm.stack = GROW_ARRAY(Value, vm.stack, oldCapacity, vm.stackCapacity);
    }
    vm.stack[vm.stackCount] = value;
    vm.stackCount++;
}

Value pop() {
    vm.stackCount--;
    return vm.stack[vm.stackCount];
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_CONSTANT_LONG() (vm.chunk->constants.values[READ_BYTE() | (READ_BYTE() << 8) | (READ_BYTE() << 16)])
/*
Using a do-while loop below allows us to execute a multi
statement macro with all inner statements in the same scope
without it expanding in a way that includes a trailing ';' value.
*/
#define BINARY_OP(op) \
    do { \
        double b = pop(); \
        double a = pop(); \
        push(a op b); \
    } while (false)

    for (;;) {

#ifdef DEBUG_TRACE_EXECUTION
    printf("        ");
    for(Value* slot = vm.stack; slot < vm.stack + vm.stackCount; slot++) {
        printf("[ ");
        printValue(*slot);
        printf(" ]");
    }
    printf("\n");
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif

        uint8_t instruction;
        switch(instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_CONSTANT_LONG: {
                Value constantLong = READ_CONSTANT_LONG();
                push(constantLong);
                break;
            }
            case OP_ADD: BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE: BINARY_OP(/); break;
            case OP_NEGATE: {
                push(-pop());
                break;
            }
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_CONSTANT_LONG
#undef BINARY_OP
}

InterpretResult interpret(const char* source) {
    compile(source);
    return INTERPRET_OK;
}