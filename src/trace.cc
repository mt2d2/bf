#include "trace.h"

#include <iostream>

Trace::Trace() : instrs(std::vector<const IR *>()) {}

Trace::State Trace::record(const IR *ir) {
  if (isComplete()) {
    goto done;
  } else if (isLoopHead(ir)) {
    lastState = Trace::State::Complete;
    goto done;
  } else if (ir->getOp() == Op::Label && instrs.size() > 0) {
    // inner loop detected, abort
    lastState = Trace::State::Abort;
    invalidate();
    goto done;
  }

  instrs.push_back(ir);
  lastState = Trace::State::Tracing;

done:
  return lastState;
}

bool Trace::isComplete() const { return lastState == Trace::State::Complete; }

void Trace::debug() const {
  for (const auto *ir : instrs) {
    std::cout << *ir << std::endl;
  }
}

bool Trace::isLoopHead(const IR *ir) const {
  return instrs.size() > 0 && ir == instrs[0];
}

void Trace::invalidate() {
  lastState = Trace::State::Abort;
  instrs.clear();
}
