#ifndef IR_H
#define IR_H

#include <cstdint>
#include <map>
#include <string>

enum Op { IncPtr, DecPtr, IncByte, DecByte, PutChar, GetChar, Label, Jmp, Hlt };
static_assert(Op::Hlt == 8, "16-bits opcode overflow");

static const std::map<Op, std::string> Op2Str = {
    {Op::IncPtr, "IncPtr"},   {Op::DecPtr, "DecPtr"},
    {Op::IncByte, "IncByte"}, {Op::DecByte, "DecByte"},
    {Op::PutChar, "PutChar"}, {Op::GetChar, "GetChar"},
    {Op::Label, "Label"},     {Op::Jmp, "Jmp"},
    {Op::Hlt, "Hlt"}};

class IR {
public:
  // 32-28, 28-14, 14-0
  // Op     A      B
  uint32_t v;

  explicit IR(Op op);
  IR(Op op, uint16_t a);
  IR(Op op, uint16_t a, uint16_t b);

  Op getOp() const { return (Op)(uint8_t)(v >> 28); }
  uint16_t getA() const {
    uint32_t arg = 0xfffffff & v;
    return arg >> 14;
  }
  uint16_t getB() const {
    uint32_t arg = 0xfffffff & v;
    return arg;
  }

private:
  void set(Op op, uint16_t a, uint16_t b);
};

static_assert(sizeof(IR) == sizeof(uint32_t),
              "IR must be the size of uint32_t");

#endif // IR_H
