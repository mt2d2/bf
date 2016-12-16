#ifndef PROGRAM_H
#define PROGRAM_H

#include <cstddef>
#include <map>
#include <vector>

#include "ir.h"

class Program {
public:
  explicit Program(std::vector<IR> in);
  const std::vector<IR> &getInstrs() const;
  const std::map<size_t, size_t> &getLoops() const;

  void debug() const;

private:
  void findLoops();

  std::vector<IR> instrs;
  std::map<size_t, size_t> loops;
};

#endif // PROGRAM_H
