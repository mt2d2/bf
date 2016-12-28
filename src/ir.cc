#include "ir.h"

#include <cassert>

IR::IR(Op op) : v(0) { set(op, 1, 0); }

IR::IR(Op op, uint16_t a) : v(0) { set(op, a, 0); }

IR::IR(Op op, uint16_t a, uint16_t b) : v(0) { set(op, a, b); }

void IR::set(Op op, uint16_t a, uint16_t b) {
  assert(a < (1 << 14));
  assert(b < (1 << 14));

  const uint32_t arg = a << 14 | b;
  v = op << 28 | arg;
}
