#include "program.h"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <stack>

Program::Program(std::vector<IR> in)
    : instrs(std::move(in)), loops(std::map<size_t, size_t>()) {
  findLoops();
}

const std::vector<IR> &Program::getInstrs() const { return instrs; }

const std::map<size_t, size_t> &Program::getLoops() const { return loops; }

void Program::findLoops() {
  // this algorithm was adopted from https://github.com/gensym-vla/PyBrainFuck
  // please see LICENSE for the appropriate LICENSE
  std::stack<size_t> open;
  size_t pc = 0;
  for (const auto i : instrs) {
    switch (i.getOp()) {
    case Op::Label:
      open.push(pc);
      break;
    case Op::Jmp: {
      assert(open.size() > 0);

      const auto top = open.top();
      open.pop();
      loops[top] = pc;
    } break;
    default:
      break;
    }
    pc += 1;
  }
  assert(open.size() == 0);
}

void Program::debug() const {
  for (const auto i : instrs) {
    std::cout << Op2Str.at(i.getOp()) << ": " << i.getArg() << std::endl;
  }
}
