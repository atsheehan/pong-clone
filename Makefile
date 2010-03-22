objects = ball.o
test_objects = all_test.o ball_test.o
CPPFLAGS = -I src

vpath %_test.cpp test
vpath %.h src
vpath %.cpp src

all: run_tests pong

pong: $(objects) main.o
	g++ $(CPPFLAGS) -o pong $(objects) main.o `sdl-config --cflags --libs`

all_tests: $(test_objects) $(objects)
	g++ $(CPPFLAGS) -o all_tests $(test_objects) $(objects) /usr/lib/libUnitTest++.a

ball_test.o: ball.h

.PHONY: run_tests clean

run_tests: all_tests
	./$<

clean:
	-rm all_tests pong main.o $(test_objects) $(objects)