EXE=bf
PRE=compile.cc
OBJS=parser.o ir.o program.o trace.o compile.o vm.o main.o
DEPS=$(OBJS:.o=.d)

CXX=clang++
CXXFLAGS=-std=c++11 -Wall -Werror -flto -Ofast -march=native -g
LDFLAGS=-flto
LUA=luajit

$(EXE): $(PRE) $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

%.cc: %.dasc
		$(LUA) dynasm/dynasm.lua -o $@ -D X64 $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

-include $(DEPS)

.PHONY=clean
clean:
	-rm -rf $(DEPS) $(PRE) $(OBJS) $(EXE)
