#ifndef IR_H
#define IR_H

#include <cstdint>
#include <map>

#ifndef NDEBUG
#include <ostream>
#include <string>
#endif

enum Op {
  IncPtr,
  DecPtr,
  IncByte,
  DecByte,
  PutChar,
  GetChar,
#ifdef JIT
  Jit,
#endif
  Label,
  Jmp,
  Assign,
  MulAdd,
  MulSub,
  Hlt
};
static_assert(Op::Hlt <= (1uLL << 32), "32 bits opcode overflow");

#ifndef NDEBUG
static const std::map<Op, std::string> Op2Str = {
    {Op::IncPtr, "IncPtr"},   {Op::DecPtr, "DecPtr"},
    {Op::IncByte, "IncByte"}, {Op::DecByte, "DecByte"},
    {Op::PutChar, "PutChar"}, {Op::GetChar, "GetChar"},
#ifdef JIT
    {Op::Jit, "Jit"},
#endif
    {Op::Label, "Label"},     {Op::Jmp, "Jmp"},
    {Op::Assign, "Assign"},   {Op::MulAdd, "MulAdd"},
    {Op::MulSub, "MulSub"},   {Op::Hlt, "Hlt"}};
#endif

class IR {
public:
  // 32-28, 28-14, 14-0
  // Op     A      B
  // 32     16     16
  uint64_t v;

  explicit IR(Op op);
  IR(Op op, uint16_t a);
  IR(Op op, uint16_t a, int16_t b);

  Op getOp() const {
    uint32_t oper = v >> 32;
    return (Op)(uint16_t)(oper >> 16);
  }
  uint16_t getThresh() const {
    uint32_t oper = v >> 32;
    return (uint16_t)oper;
  }
  void incThresh() {
    uint16_t thresh = getThresh();
    set(getOp(), thresh + 1, getA(), getB());
  }
  uint16_t getA() const {
    uint32_t arg = v;
    return arg >> 16;
  }
  int16_t getB() const {
    uint32_t arg = v;
    return (int16_t)arg;
  }

private:
  void set(Op op, uint16_t thresh, uint16_t a, int16_t b);
};
static_assert(sizeof(IR) == sizeof(uint64_t), "IR must be 64 bits");

#ifndef NDEBUG
std::ostream &operator<<(std::ostream &os, const IR &i);
#endif

#endif // IR_H
