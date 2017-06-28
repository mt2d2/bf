EXE=bf
OBJS=parser.o ir.o program.o vm.o main.o
DEPS=$(OBJS:.o=.d)

C=clang
CXX=clang++
CXXFLAGS=-std=c++11 -Wall -Werror -flto -Ofast -march=native -g
LDFLAGS=-flto
LUA=luajit

TESTARCH=$(shell $(CC) -E arch.h -dM)
ifneq (,$(findstring BF_JIT_X64,$(TESTARCH)))
  BF_JIT_ARCH=x64
endif
ifneq (,$(findstring BF_JIT_ARM,$(TESTARCH)))
  BF_JIT_ARCH=arm
endif

ifdef BF_JIT_ARCH
	CXXFLAGS+=-DJIT
	PRE=compile_$(BF_JIT_ARCH).cc
	OBJS+=trace.o compile_$(BF_JIT_ARCH).o
endif

$(EXE): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

%.cc: %.dasc
		$(LUA) dynasm/dynasm.lua -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

-include $(DEPS)

.PHONY=clean
clean:
	-rm -rf $(DEPS) $(PRE) $(OBJS) $(EXE)
