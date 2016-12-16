#ifndef VM_H
#define VM_H

#include "program.h"

class VM {
public:
  explicit VM(Program prog);
  void run();

private:
  Program program;
};

#endif // VM_H
