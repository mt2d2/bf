#ifndef IR_H
#define IR_H

#include <cstdint>
#include <map>
#include <string>

enum Op : uint16_t {
  IncPtr,
  DecPtr,
  IncByte,
  DecByte,
  PutChar,
  GetChar,
  Label,
  Jmp,
  Hlt
};
static_assert(Op::Hlt < 16, "16-bits opcode overflow");

static const std::map<Op, std::string> Op2Str = {
    {Op::IncPtr, "IncPtr"},   {Op::DecPtr, "DecPtr"},
    {Op::IncByte, "IncByte"}, {Op::DecByte, "DecByte"},
    {Op::PutChar, "PutChar"}, {Op::GetChar, "GetChar"},
    {Op::Label, "Label"},     {Op::Jmp, "Jmp"},
    {Op::Hlt, "Hlt"}};

class IR {
public:
  uint32_t v;

  explicit IR(Op op);
  IR(Op op, uint16_t arg);

  Op getOp() const { return (Op)(uint16_t)(v >> 16); }
  uint16_t getArg() const { return v; }

private:
  void set(Op op, uint16_t arg);
};

static_assert(sizeof(IR) == sizeof(uint32_t),
              "IR must be the size of uint32_t");

#endif // IR_H
