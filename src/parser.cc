#include "parser.h"

#include <algorithm>
#include <fstream>
#include <ios>
#include <vector>

#include "ir.h"

static const char bfs[] = {'>', '<', '+', '-', '.', ',', '[', ']'};
static const Op ops[] = {Op::IncPtr,  Op::DecPtr,  Op::IncByte, Op::DecByte,
                         Op::PutChar, Op::GetChar, Op::Label,   Op::Jmp};

static bool isBfChar(char ch) {
  return std::any_of(std::begin(bfs), std::end(bfs),
                     [=](char x) { return ch == x; });
}

Program parse(const std::string &file) {
  std::vector<IR> instrs;

  char ch;
  std::fstream fin(file, std::fstream::in);
  while (fin >> std::noskipws >> ch) {
    if (isBfChar(ch)) {
      const auto it = std::find(std::begin(bfs), std::end(bfs), ch);
      if (it != std::end(bfs)) {
        instrs.push_back(IR(ops[it - std::begin(bfs)]));
      }
    }
  }

  instrs.push_back(IR(Hlt));

  return Program(instrs);
}
