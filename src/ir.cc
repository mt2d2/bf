#include "ir.h"

#include <cassert>

IR::IR(Op op) : v(0) { set(op, 1, 0); }

IR::IR(Op op, uint16_t a) : v(0) { set(op, a, 0); }

IR::IR(Op op, uint16_t a, int16_t b) : v(0) { set(op, a, b); }

void IR::set(Op op, uint16_t a, int16_t b) {
  v = ((uint64_t)op << 32) | (((uint32_t)a << 16) | ((uint16_t)b));
}
