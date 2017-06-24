#include "vm.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#define likely(x) __builtin_expect((long)(x), 1)
#define unlikely(x) __builtin_expect((long)(x), 0)

VM::VM(Program prog)
    : program(std::move(prog)), tape(static_cast<uint8_t *>(calloc(30000, 1)))
#ifdef JIT
      ,
      trace(), traces()
#endif
{
}

VM::~VM() { free(tape); }

void VM::run() {
#ifdef JIT
  static const void *opLbls[] = {
      &&IncPtr, &&DecPtr, &&IncByte, &&DecByte, &&PutChar, &&GetChar, &&Jit,
      &&Label,  &&Jmp,    &&Assign,  &&MulAdd,  &&MulSub,  &&Hlt};
  static const void *traceLbls[] = {
      &&trace_IncPtr,  &&trace_DecPtr,  &&trace_IncByte, &&trace_DecByte,
      &&trace_PutChar, &&trace_GetChar, &&trace_Jit,     &&trace_Label,
      &&trace_Jmp,     &&trace_Assign,  &&trace_MulAdd,  &&trace_MulSub,
      &&trace_Hlt};

#define OP(x)                                                                  \
  trace_##x : {                                                                \
    const auto state = trace.record(instr);                                    \
    if (state != Trace::State::Tracing) {                                      \
      if (false && state == Trace::State::Abort)                               \
        printf("trace aborted\n");                                             \
      disp = opLbls;                                                           \
    }                                                                          \
  }                                                                            \
  x:
#else
  static const void *opLbls[] = {&&IncPtr,  &&DecPtr,  &&IncByte, &&DecByte,
                                 &&PutChar, &&GetChar, &&Label,   &&Jmp,
                                 &&Assign,  &&MulAdd,  &&MulSub,  &&Hlt};
#define OP(x)                                                                  \
  x:
#endif

#define DISPATCH                                                               \
  instr = &instrs[pc++];                                                       \
  goto *disp[instr->getOp()];

  std::vector<IR> &instrs = program.getInstrs();
  void const **disp = opLbls;
  uint8_t *ptr = tape;
  size_t pc = 0;
  IR *instr;

  DISPATCH;

  OP(IncPtr) {
    ptr += instr->getA();
    DISPATCH;
  }
  OP(DecPtr) {
    ptr -= instr->getA();
    DISPATCH;
  }
  OP(IncByte) {
    *(ptr + instr->getB()) += instr->getA();
    DISPATCH;
  }
  OP(DecByte) {
    *(ptr + instr->getB()) -= instr->getA();
    DISPATCH;
  }
  OP(PutChar) {
    putchar(*(ptr + instr->getB()));
    DISPATCH;
  }
  OP(GetChar) {
    *(ptr + instr->getB()) = getchar();
    DISPATCH;
  }
#ifdef JIT
  OP(Jit) {
    nativeTrace mcode = traces[instr->getB()].getMcode();
    uint8_t *newTape = mcode(ptr);
    ptr = newTape;
    pc = instr->getA();
    DISPATCH;
  }
#endif
  OP(Label) {
#ifdef JIT
    if (unlikely(trace.isComplete())) {
      // printf("trace completed\n");
      // trace.debug();
      trace.compile(traces);
      traces.push_back(std::move(trace));
      *instr = IR(Op::Jit, instr->getA(), traces.size() - 1);
      trace = Trace();
      disp = opLbls;
    } else {
      // profiling mode
      if (unlikely(instr->getThresh() == Trace::LoopThreshold)) {
        instr->incThresh();
        trace.record(instr); // record loop header
        disp = traceLbls;
      } else {
        instr->incThresh();
      }
    }
#endif

    if (unlikely(!*ptr)) {
      pc = instr->getA();
    }
    DISPATCH;
  }
  OP(Jmp) {
    pc = instr->getA();
    DISPATCH;
  }
  OP(Assign) {
    *ptr = instr->getA();
    DISPATCH;
  }
  OP(MulAdd) {
    *(ptr + instr->getB()) += *ptr * instr->getA();
    DISPATCH;
  }
  OP(MulSub) {
    *(ptr + instr->getB()) -= *ptr * instr->getA();
    DISPATCH;
  }
  OP(Hlt) { return; }
}
