objects = ball.o paddle.o
test_objects = all_test.o ball_test.o paddle_test.o
CPPFLAGS = -I src
SDL_LINKER = `sdl-config --cflags --libs`

vpath %_test.cpp test
vpath %.h src
vpath %.cpp src

all: run_tests pong

pong: $(objects) main.o
	g++ $(CPPFLAGS) -o pong $(objects) main.o $(SDL_LINKER)

all_tests: $(test_objects) $(objects)
	g++ $(CPPFLAGS) -o all_tests $(test_objects) $(objects) /usr/lib/libUnitTest++.a $(SDL_LINKER)

ball_test.o: ball.h drawable.h
ball.o: drawable.h
paddle_test.o: paddle.h drawable.h
paddle.o: drawable.h

.PHONY: run_tests clean

run_tests: all_tests
	./$<

clean:
	-rm all_tests pong main.o $(test_objects) $(objects)