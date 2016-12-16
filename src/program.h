#ifndef PROGRAM_H
#define PROGRAM_H

#include <cstddef>
#include <unordered_map>
#include <vector>

#include "ir.h"

class Program {
public:
  explicit Program(std::vector<IR> in);
  const std::vector<IR> &getInstrs() const;
  const std::unordered_map<size_t, size_t> &getLoops() const;

  void debug() const;

private:
  void findLoops();

  std::vector<IR> instrs;
  std::unordered_map<size_t, size_t> loops;
};

#endif // PROGRAM_H
