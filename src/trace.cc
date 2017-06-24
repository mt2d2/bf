#include "trace.h"

#include <cassert>
#include <iostream>

#include <sys/mman.h>

#include "ir.h"

Trace::Trace()
    : instrs(), lastState(Trace::State::Abort), mcode(nullptr), mcodeSz(0) {
  instrs.reserve(5);
}

Trace::~Trace() {
  if (mcode) {
    assert(munmap(mcode, mcodeSz) == 0);
  }
}

Trace::Trace(Trace &&o) noexcept
    : mcode(std::move(o.mcode)), mcodeSz(std::move(o.mcodeSz)) {
  o.mcode = nullptr;
}

Trace &Trace::operator=(Trace &&o) {
  instrs = std::move(o.instrs);
  lastState = std::move(o.lastState);
  mcode = std::move(o.mcode);
  mcodeSz = std::move(o.mcodeSz);
  return *this;
}

Trace::State Trace::record(const IR *ir) {
  if (isLoopHead(ir)) {
    lastState = Trace::State::Complete;
    goto done;
  } else if (ir->getOp() == Op::Label && instrs.size() > 0) {
    // inner loop detected, abort
    invalidate();
    goto done;
  }

  instrs.push_back(ir);
  lastState = Trace::State::Tracing;

done:
  return lastState;
}

#ifndef NDEBUG
void Trace::debug() const {
  for (const auto *ir : instrs) {
    std::cout << *ir << std::endl;
  }
}
#endif

bool Trace::isLoopHead(const IR *ir) const {
  return instrs.size() > 0 && ir == instrs[0];
}

void Trace::invalidate() {
  lastState = Trace::State::Abort;
  instrs.clear();
}

const IR *Trace::lastIR() const {
  assert(instrs.size() > 0);
  return instrs.at(instrs.size() - 1);
}
