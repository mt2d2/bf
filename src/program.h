#ifndef PROGRAM_H
#define PROGRAM_H

#include <cstddef>
#include <vector>

#include "ir.h"

class Program {
public:
  explicit Program(std::vector<IR> in);
  std::vector<IR> &getInstrs();
#ifndef NDEBUG
  void debug() const;
#endif

private:
  void findLoops();
  void foldRepeated();
  void foldMovement();
  void foldMultiply();
  void foldAssign();

  std::vector<IR> instrs;
};

#endif // PROGRAM_H
