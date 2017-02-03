#include "trace.h"

#include <cassert>
#include <iostream>

#include <sys/mman.h>

#include "ir.h"

Trace::Trace()
    : instrs(std::vector<const IR *>()), lastState(Trace::State::Abort),
      mcode(nullptr), mcodeSz(0) {
  instrs.reserve(5);
}

Trace::~Trace() {
  if (mcode) {
    assert(munmap(mcode, mcodeSz) == 0);
  }
}

Trace::Trace(Trace &&o) noexcept : mcode(std::move(o.mcode)),
                                   mcodeSz(std::move(o.mcodeSz)) {
  o.mcode = nullptr;
}

Trace &Trace::operator=(Trace &&o) {
  instrs = o.instrs;
  lastState = o.lastState;
  mcode = o.mcode;
  mcodeSz = o.mcodeSz;
  return *this;
}

Trace::State Trace::record(const IR *ir) {
  if (ir->getOp() == Op::Jit) {
    lastState = Trace::State::Abort;
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

const IR *Trace::lastIR() const {
  assert(instrs.size() > 0);
  return instrs.at(instrs.size() - 1);
}
