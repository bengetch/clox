#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);
    int constantTwo = addConstant(&chunk, 5);
    writeChunk(&chunk, OP_CONSTANT, 10);
    writeChunk(&chunk, constant, 10);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constantTwo, 123);
    writeChunk(&chunk, OP_RETURN, 123);

    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);

    return 0;
}