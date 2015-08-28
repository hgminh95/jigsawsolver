CC = g++
DEBUG = -O3
CFLAGS = -c -std=c++11 $(DEBUG)
LFLAGS = -std=c++11 $(DEBUG)

OBJS = bin/image/image.o

.PHONY: compile clean test

all: compile test clean

compile:
	@printf "[COMPILE] src/image/*\n"
	@mkdir -p bin/image
	@$(CC) $(CFLAGS) src/image/*.cpp
	@mv ./*.o bin/image
	@printf "[COMPILE] src/utility/*\n"
	@mkdir -p bin/utility
	@$(CC) $(CFLAGS) src/utility/*.cpp
	@mv ./*.o bin/utility
	@printf "[COMPILE] src/*\n"
	@$(CC) $(CFLAGS) src/*.cpp
	@mv ./*.o bin/
	@printf "[COMPILE] Linking...\n"
	@$(CC) -o bin/jigsawsolver $(LFLAGS) bin/*.o bin/image/*.o bin/utility/*.o
	@printf "[COMPILE] Done.\n"

clean:
	@printf "[CLEAN] "
	@rm -f */*.o */*/*.o
	@printf "[DONE]\n"

test:
	@printf "[TEST] Not implement yet\n"
