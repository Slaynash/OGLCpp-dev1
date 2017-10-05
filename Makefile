CC      = g++
CFLAGS  = -W -Wall -ansi -Wno-deprecated
OS      = $(shell uname)

TARGET  = program

ifeq ($(OS), Darwin)
    LDFLAGS = -framework OpenGL -framework GLUT -framework SDL2
else
    LDFLAGS = -lglut -lGLU -lGL -lm -lSDL2 -lGLEW -lSDL2_image
endif

SRC = $(shell find src/ -type f \( -iname \*.cpp -o -iname \*.c \))
OBJ = $(patsubst src/%,bin/%,$(addsuffix .o, $(basename $(SRC))))


all: directories $(TARGET)

directories:
	@echo "=== SRC ==="
	@echo $(SRC)
	@echo "=== OBJ ==="
	@echo $(OBJ)
	@mkdir -p bin
	@mkdir -p build

compileAndRun: all
	build/$(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o build/$@ $^ $(LDFLAGS)

bin/%.o: src/%.c
	@echo ===== Creating file $@ ===
	mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

bin/%.o: src/%.cpp
	@echo ===== Creating file $@ ===
	mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -rf bin/

cleaner: clean
	@rm -rf build/
