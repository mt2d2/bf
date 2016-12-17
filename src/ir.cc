#include "ir.h"

IR::IR(Op op) : v(0) { set(op, 1); }

IR::IR(Op op, uint16_t arg) : v(0) { set(op, arg); }

void IR::set(Op op, uint16_t arg) { v = op << 16 | arg; }
