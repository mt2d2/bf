#include "program.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stack>

Program::Program(std::vector<IR> in) : instrs(std::move(in)) {
  foldRepeated();
  foldMovement();
  findLoops();
}

const std::vector<IR> &Program::getInstrs() const { return instrs; }

void Program::debug() const {
  unsigned indent = 0;
  for (const auto i : instrs) {
    for (unsigned i = 0; i < indent; ++i)
      std::cout << "  ";

    std::cout << Op2Str.at(i.getOp()) << " [" << i.getA() << ", " << i.getB()
              << "] " << std::endl;

    if (i.getOp() == Op::Label)
      ++indent;
    else if (i.getOp() == Op::Jmp)
      --indent;
  }
}

void Program::findLoops() {
  // this algorithm was adopted from https://github.com/gensym-vla/PyBrainFuck
  // please see LICENSE for the appropriate copyright
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
      instrs[top] = IR(Op::Label, pc);
      instrs[pc] = IR(Op::Jmp, top);
    } break;
    default:
      break;
    }
    pc += 1;
  }
  assert(open.size() == 0);
}

void Program::foldRepeated() {
  static const Op tgts[] = {Op::DecPtr, Op::IncPtr, Op::IncByte, Op::DecByte};

  for (const auto tgtOp : tgts) {
    auto it = instrs.begin();
    while (it != instrs.end()) {
      if (it->getOp() == tgtOp) {
        auto jt = it;

        size_t sz = 0;
        while (it->getOp() == tgtOp && it != instrs.end()) {
          ++it, ++sz;
        }
        *jt = IR(tgtOp, sz);
        it = instrs.erase(jt + 1, it);
      } else {
        ++it;
      }
    }
  }
}

void Program::foldMovement() {
  int move = 0;

  const auto isMov = [](const std::vector<IR>::const_iterator &it) {
    return it->getOp() == Op::IncPtr || it->getOp() == Op::DecPtr;
  };
  const auto isMod = [](const std::vector<IR>::const_iterator &it) {
    static const auto allwd = {Op::IncByte, Op::DecByte, Op::PutChar,
                               Op::GetChar};
    return std::any_of(std::begin(allwd), std::end(allwd),
                       [&](const Op op) { return it->getOp() == op; });
  };
  const auto isLoop = [](const std::vector<IR>::const_iterator &it) {
    return it->getOp() == Op::Label || it->getOp() == Op::Jmp;
  };

  auto it = instrs.begin();
  while (it != instrs.end()) {
    if (isLoop(it) && move != 0) {
      it = instrs.insert(
          it, IR(move > 0 ? Op::IncPtr : Op::DecPtr, std::abs(move)));
      it += 2;
      move = 0;
    } else if (isMov(it)) {
      move += (it->getOp() == Op::IncPtr) ? it->getA() : -it->getA();
      it = instrs.erase(it, it + 1);
    } else if (isMod(it)) {
      *it = IR(it->getOp(), it->getA(), move);
      ++it;
    } else {
      ++it;
    }
  }
}
