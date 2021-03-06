#ifndef TRACE_H
#define TRACE_H

#include <cstdint>
#include <vector>

class IR;

typedef uint8_t *(*nativeTrace)(uint8_t *);

class Trace {
public:
  static const int16_t LoopThreshold = 1000;

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
  void compile(const std::vector<Trace> &traces);
  nativeTrace getMcode() const { return reinterpret_cast<nativeTrace>(mcode); }
#ifndef NDEBUG
  void debug() const;
#endif

private:
  bool isLoopHead(const IR *ir) const;
  void invalidate();
  const IR *lastIR() const;

  std::vector<const IR *> instrs;
  State lastState;

  void *mcode;
  std::size_t mcodeSz;
};

#endif // TRACE_H
