#ifndef PROGRAM_H
#define PROGRAM_H

#include <cstddef>
#include <vector>

#include "ir.h"

class Program {
public:
  explicit Program(std::vector<IR> in);
  const std::vector<IR> &getInstrs() const;
  void debug() const;

private:
  void findLoops();
  void foldRepeated();
  void foldMovement();
  void foldClear();

  std::vector<IR> instrs;
};

#endif // PROGRAM_H
