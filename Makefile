objects = ball.o
test_objects = all_test.o ball_test.o
CPPFLAGS = -I src

vpath %_test.cpp test
vpath %.h src
vpath %.cpp src

all: run_tests

all_tests: $(test_objects) $(objects)
	g++ -o all_tests $(test_objects) $(objects) /usr/lib/libUnitTest++.a

.PHONY: run_tests clean

run_tests: all_tests
	./$<

clean:
	-rm all_tests $(test_objects) $(objects)