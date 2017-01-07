#include <cstdlib>
#include <iostream>

#include "parser.h"
#include "vm.h"

void fail(const std::string &msg) {
  std::cerr << msg << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fail("bf program.b");
  }

  const auto program = parse(argv[1]);
  program.debug();
  VM(program).run();
}
