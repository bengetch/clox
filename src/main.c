#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
    initVM();
    Chunk chunk;
    initChunk(&chunk);

    for (int i = 0; i < 300; i++) {
        writeConstant(&chunk, i, i);
        writeChunk(&chunk, OP_NEGATE, i);
    }
//    writeConstant(&chunk, 7, 123);
//    writeChunk(&chunk, OP_NEGATE, 123);
    writeChunk(&chunk, OP_RETURN, 123);

    freeVM();
    interpret(&chunk);
    freeChunk(&chunk);

    return 0;
}