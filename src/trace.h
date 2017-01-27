#ifndef TRACE_H
#define TRACE_H

#include <cstdint>
#include <vector>

#include "ir.h"

class Trace {
public:
  enum State {
    Abort,
    Tracing,
    Complete,
  };

  Trace();
  ~Trace();

  State record(const IR *ir);
  bool isComplete() const;
  void debug() const;
  void compile();

private:
  bool isLoopHead(const IR *ir) const;
  void invalidate();

  std::vector<const IR *> instrs;
  State lastState;

  void *mcode;
  size_t mcodeSz;
};

#endif // TRACE_H
