#ifndef VM_H
#define VM_H

#include <cstdint>
#ifdef JIT
#include <vector>
#endif

#include "program.h"
#include "trace.h"

class IR;

class VM {
public:
  explicit VM(Program prog);
  ~VM();
  void run();

private:
  Program program;
  uint8_t *tape;

#ifdef JIT
  Trace trace;
  std::vector<Trace> traces;
#endif
};

#endif // VM_H
