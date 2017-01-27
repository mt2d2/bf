#include "ir.h"

#include <cassert>

IR::IR(Op op) : v(0) { set(op, 8, 1, 0); }

IR::IR(Op op, uint16_t a) : v(0) { set(op, 8, a, 0); }

IR::IR(Op op, uint16_t a, int16_t b) : v(0) { set(op, 8, a, b); }

void IR::set(Op op, uint16_t thresh, uint16_t a, int16_t b) {
  uint32_t oper = op << 16 | thresh;
  uint32_t arg = a << 16 | (uint16_t)b;
  v = ((uint64_t)oper << 32) | arg;
}

std::ostream &operator<<(std::ostream &os, const IR &i) {
  os << Op2Str.at(i.getOp()) << " [" << i.getA() << ", " << i.getB() << "] ";
  return os;
}
