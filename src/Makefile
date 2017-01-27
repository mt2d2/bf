EXE=bf
OBJS=parser.o ir.o program.o trace.o vm.o main.o
DEPS=$(OBJS:.o=.d)

CXX=clang++
CXXFLAGS=-std=c++11 -Wall -Werror -Ofast -march=native -g
LDFLAGS=

$(EXE): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

-include $(DEPS)

.PHONY=clean
clean:
	-rm -rf $(DEPS) $(OBJS) $(EXE)
