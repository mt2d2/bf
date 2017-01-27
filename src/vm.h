#ifndef VM_H
#define VM_H

#include <cstdint>
#include <unordered_map>

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
  unsigned traceCount;
  std::unordered_map<unsigned, Trace> traces;
};

#endif // VM_H
