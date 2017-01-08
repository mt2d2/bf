#ifndef VM_H
#define VM_H

#include <cstdlib>

#include "program.h"

class VM {
public:
  explicit VM(Program prog);
  ~VM();
  void run();

private:
  Program program;
  uint8_t *tape;
};

#endif // VM_H
