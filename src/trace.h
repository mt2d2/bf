#ifndef TRACE_H
#define TRACE_H

#include <cstdint>
#include <vector>

class IR;

typedef uint8_t *(*nativeTrace)(uint8_t *);

class Trace {
public:
  enum State {
    Abort,
    Tracing,
    Complete,
  };

  Trace();
  ~Trace();
  Trace(Trace &&o) noexcept;
  Trace &operator=(Trace &&o);

  State record(const IR *ir);
  bool isComplete() const { return lastState == Trace::State::Complete; }
  void debug() const;
  void compile(const std::vector<Trace> &traces);
  nativeTrace getMcode() const { return reinterpret_cast<nativeTrace>(mcode); }

private:
  bool isLoopHead(const IR *ir) const;
  void invalidate();
  const IR *lastIR() const;

  std::vector<const IR *> instrs;
  State lastState;

  void *mcode;
  size_t mcodeSz;
};

#endif // TRACE_H
