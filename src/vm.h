#ifndef VM_H
#define VM_H

#include <cstdint>
#include <vector>

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

  Trace trace;
  std::vector<Trace> traces;
};

#endif // VM_H
