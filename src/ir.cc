#include "ir.h"

IR::IR(Op op) : v(0) { set(op, 0); }

IR::IR(Op op, uint16_t arg) : v(0) { set(op, arg); }

Op IR::getOp() const { return (Op)(uint16_t)(v >> 16); }

uint16_t IR::getArg() const { return v; }

void IR::set(Op op, uint16_t arg) { v = op << 16 | arg; }
