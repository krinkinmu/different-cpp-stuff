CXX := clang++-3.5
CXXFLAGS := -Wall -Wextra -std=c++03 -pedantic -MMD
LIBS = -lboost_system -lboost_date_time -lboost_thread

TIMER1_OBJS := timer1.o
TIMER2_OBJS := timer2.o
TIMER3_OBJS := timer3.o
TIMER4_OBJS := timer4.o
TIMER5_OBJS := timer5.o
TARGETS := timer1 timer2 timer3 timer4 timer5

all: $(TARGETS)

timer1: $(TIMER1_OBJS)
	$(CXX) $(LIBS) $^ -o $@

timer2: $(TIMER2_OBJS)
	$(CXX) $(LIBS) $^ -o $@

timer3: $(TIMER3_OBJS)
	$(CXX) $(LIBS) $^ -o $@

timer4: $(TIMER4_OBJS)
	$(CXX) $(LIBS) $^ -o $@

timer5: $(TIMER5_OBJS)
	$(CXX) $(LIBS) $^ -o $@

-include: *.d

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGETS)

.PHONY: clean
