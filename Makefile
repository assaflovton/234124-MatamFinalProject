CC =/usr/local/bin/gcc
CXX=/usr/local/bin/g++
OBJS = Execute.o Calc.o gcalc.o Graph.o ParseUtility.o 
EXEC = gcalc
DEBUG_FLAGS = -g
COMP_FLAGS = -std=c++11 -fPIC -Wall -Werror

$(EXEC) : $(OBJS)
	$(CXX) $(DEBUG_FLAGS) $(OBJS) -o $@
Calc.o: Calc.cpp Calc.h Graph.h Exceptions.h ParseUtility.h 
	$(CXX) -c  $(DEBUG_FLAGS) $(COMP_FLAGS) $*.cpp 
Execute.o: Execute.cpp Execute.h Graph.h Exceptions.h ParseUtility.h Calc.h
	$(CXX) -c  $(DEBUG_FLAGS) $(COMP_FLAGS) $*.cpp 
gcalc.o: gcalc.cpp Graph.h Exceptions.h ParseUtility.h Calc.h Execute.h
	$(CXX) -c  $(DEBUG_FLAGS) $(COMP_FLAGS) $*.cpp 
Graph.o: Graph.cpp Graph.h Exceptions.h ParseUtility.h
	$(CXX) -c  $(DEBUG_FLAGS) $(COMP_FLAGS) $*.cpp 
ParseUtility.o: ParseUtility.cpp ParseUtility.h
	$(CXX) -c  $(DEBUG_FLAGS) $(COMP_FLAGS) $*.cpp 
libgraph.a: $(OBJS)
	ar -rs $@ $^
clean:
	rm -f $(OBJS) $(EXEC)
make tar:
	zip gcalc.zip design.pdf Makefile test_in.txt test_out.txt Calc.cpp Calc.h ParseUtility.h ParseUtility.cpp gcalc.cpp Graph.h Graph.cpp Execute.h Exceptions.h Execute.cpp graph.i
 
