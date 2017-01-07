#ifndef IR_H
#define IR_H

#include <cstdint>
#include <map>
#include <string>

enum Op {
  IncPtr,
  DecPtr,
  IncByte,
  DecByte,
  PutChar,
  GetChar,
  Label,
  Jmp,
  Assign,
  MulAdd,
  MulSub,
  Hlt
};
static_assert(Op::Hlt <= (1uLL << 32), "32 bits opcode overflow");

static const std::map<Op, std::string> Op2Str = {
    {Op::IncPtr, "IncPtr"},   {Op::DecPtr, "DecPtr"},
    {Op::IncByte, "IncByte"}, {Op::DecByte, "DecByte"},
    {Op::PutChar, "PutChar"}, {Op::GetChar, "GetChar"},
    {Op::Label, "Label"},     {Op::Jmp, "Jmp"},
    {Op::Assign, "Assign"},   {Op::MulAdd, "MulAdd"},
    {Op::MulSub, "MulSub"},   {Op::Hlt, "Hlt"}};

class IR {
public:
  // 32-28, 28-14, 14-0
  // Op     A      B
  // 32     16     16
  uint64_t v;

  explicit IR(Op op);
  IR(Op op, uint16_t a);
  IR(Op op, uint16_t a, int16_t b);

  Op getOp() const { return (Op)(uint32_t)(v >> 32); }
  uint16_t getA() const {
    uint32_t arg = v;
    return arg >> 16;
  }
  int16_t getB() const {
    uint32_t arg = v;
    return (int16_t)arg;
  }

private:
  void set(Op op, uint16_t a, int16_t b);
};
static_assert(sizeof(IR) == sizeof(uint64_t), "IR must be 64 bits");

#endif // IR_H
