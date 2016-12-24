#include "vm.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#define likely(x) __builtin_expect((long)(x), 1)
#define unlikely(x) __builtin_expect((long)(x), 0)

VM::VM(Program prog) : program(std::move(prog)) {}

void VM::run() {
  const auto &instrs = program.getInstrs();
  uint8_t *ptr = static_cast<uint8_t *>(calloc(30000, 1));

  static const void *lbls[] = {&&IncPtr,  &&DecPtr,  &&IncByte,
                               &&DecByte, &&PutChar, &&GetChar,
                               &&Label,   &&Jmp,     &&Hlt};

  size_t pc = 0;
  auto instr = instrs[pc];

#define DISPATCH                                                               \
  instr = instrs[pc++];                                                        \
  goto *lbls[instr.getOp()];

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
  if (unlikely(!*ptr)) {
    pc = instr.getArg();
  }
  DISPATCH;
Jmp:
  if (likely(*ptr)) {
    pc = instr.getArg();
  }
  DISPATCH;
Hlt:
  return;
}
