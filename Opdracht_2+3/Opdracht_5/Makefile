# A simple Makefile for compiling small SDL projects

# set the compiler
CC := gcc

# set the compiler flags
CFLAGS := `sdl2-config --libs --cflags` -Wpedantic -Wconversion -Wno-missing-braces -Wall -Wextra -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lm

# add header files here
HDRS :=

# add source files here
SRCS := sdl2b.c Mod/Blit/Blit.c Mod/Input/Input.c Mod/Textures/Textures.c Mod/Update/Update.c 

# generate names of object files
OBJS := $(SRCS:.c=.o)

# name of executable
EXEC := game

# default recipe
all: $(EXEC)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#	$(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) *.o

.PHONY: all clean
