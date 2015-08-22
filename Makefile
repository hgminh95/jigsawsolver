CC = g++	
DEBUG = -g -Wall
CFLAGS = -c $(DEBUG)
LFLAGS = $(DEBUG)

OBJS = bin/image/image.o

.PHONY: compile clean test

all: compile clean test

compile:
	@printf "[COMPILE] src/image/*\n"
	@mkdir -p bin/image
	@$(CC) $(CFLAGS) src/image/*.cpp
	@mv ./*.o bin/image
	@printf "[COMPILE] src/ultility/*\n"
	@mkdir -p bin/ultility
	@$(CC) $(CFLAGS) src/ultility/*.cpp
	@mv ./*.o bin/ultility
	@printf "[COMPILE] src/*\n"
	@$(CC) $(CFLAGS) src/*.cpp
	@mv ./*.o bin/ 
	@printf "[COMPILE] Linking...\n"
	@$(CC) -o bin/jigsawsolver $(LFLAGS) bin/*.o bin/image/*.o bin/ultility/*.o
	@printf "[COMPILE] Done."

clean:
	@printf "[CLEAN] "
	@rm -f */*.o */*/*.o
	@printf "[DONE]"

test:
	@printf "[TEST] Not implement yet"
	
	
