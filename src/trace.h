#ifndef TRACE_H
#define TRACE_H

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

  State record(const IR *ir);
  bool isComplete() const;
  void debug() const;

private:
  bool isLoopHead(const IR *ir) const;
  void invalidate();

  std::vector<const IR *> instrs;
  State lastState;
};

#endif // TRACE_H
