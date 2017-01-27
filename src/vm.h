#ifndef VM_H
#define VM_H

#include <cstdint>

#include "program.h"
#include "trace.h"

class VM {
public:
  explicit VM(Program prog);
  ~VM();
  void run();

private:
  Program program;
  Trace trace;
  uint8_t *tape;
};

#endif // VM_H
