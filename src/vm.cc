#include "vm.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <stack>

VM::VM(Program prog) : program(std::move(prog)) {}

void VM::run() {
  std::stack<size_t> pcs;
  char *mem = static_cast<char *>(calloc(30000, 1));
  char *ptr = mem;

  const size_t size = program.getInstrs().size();
  for (size_t pc = 0; pc < size; ++pc) {
    switch (program.getInstrs()[pc].getOp()) {
    case IncPtr:
      ++ptr;
      break;
    case DecPtr:
      --ptr;
      break;
    case IncByte:
      ++*ptr;
      break;
    case DecByte:
      --*ptr;
      break;
    case PutChar:
      putchar(*ptr);
      break;
    case GetChar:
      *ptr = getchar();
      break;
    case Label:
      if (*ptr > 0) {
        pcs.push(pc);
      } else {
        pc = program.getLoops().at(pc);
      }
      break;
    case Jmp:
      assert(pcs.size() > 0);
      pc = pcs.top() - 1;
      pcs.pop();
      break;
    }
  }
}
