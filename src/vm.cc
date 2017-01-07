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

  static const void *lbls[] = {&&IncPtr,  &&DecPtr,  &&IncByte, &&DecByte,
                               &&PutChar, &&GetChar, &&Label,   &&Jmp,
                               &&Assign,  &&MulAdd,  &&MulSub,  &&Hlt};

  size_t pc = 0;
  auto instr = instrs[pc];

#define DISPATCH                                                               \
  instr = instrs[pc++];                                                        \
  goto *lbls[instr.getOp()];

  DISPATCH;

IncPtr:
  ptr += instr.getA();
  DISPATCH;
DecPtr:
  ptr -= instr.getA();
  DISPATCH;
IncByte:
  *(ptr + instr.getB()) += instr.getA();
  DISPATCH;
DecByte:
  *(ptr + instr.getB()) -= instr.getA();
  DISPATCH;
PutChar:
  putchar(*(ptr + instr.getB()));
  DISPATCH;
GetChar:
  *(ptr + instr.getB()) = getchar();
  DISPATCH;
Label:
  if (unlikely(!*ptr)) {
    pc = instr.getA();
  }
  DISPATCH;
Jmp:
  if (likely(*ptr)) {
    pc = instr.getA();
  }
  DISPATCH;
Assign:
  *ptr = instr.getA();
  DISPATCH;
MulAdd:
  *(ptr + instr.getB()) += *ptr * instr.getA();
  DISPATCH;
MulSub:
  *(ptr + instr.getB()) -= *ptr * instr.getA();
  DISPATCH;
Hlt:
  return;
}
