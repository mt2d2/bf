#include "vm.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>

VM::VM(Program prog) : program(std::move(prog)) {}

#define DISPATCH                                                               \
  instr = instrs[pc++];                                                        \
  goto *lbls[instr.getOp()];

void VM::run() {
  const auto &instrs = program.getInstrs();
  uint8_t *ptr = static_cast<uint8_t *>(calloc(30000, 1));

  static const void *lbls[] = {&&IncPtr,  &&DecPtr,  &&IncByte,
                               &&DecByte, &&PutChar, &&GetChar,
                               &&Label,   &&Jmp,     &&Hlt};

  size_t pc = 0;
  auto instr = instrs[pc];
  DISPATCH;

IncPtr:
  ptr += instr.getArg();
  DISPATCH;
DecPtr:
  ptr -= instr.getArg();
  DISPATCH;
IncByte:
  *ptr += instr.getArg();
  DISPATCH;
DecByte:
  *ptr -= instr.getArg();
  DISPATCH;
PutChar:
  putchar(*ptr);
  DISPATCH;
GetChar:
  *ptr = getchar();
  DISPATCH;
Label:
  if (!*ptr) {
    pc = instr.getArg();
  }
  DISPATCH;
Jmp:
  if (*ptr) {
    pc = instr.getArg();
  }
  DISPATCH;
Hlt:
  return;
}
