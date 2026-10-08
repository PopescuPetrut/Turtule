CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDLIBS = -lm

TARGET = lsystem
SRCS = runic.c memory_functions.c undo_redo.c system_process.c LSYSTEM.c turtule.c
HEADERS = runic.h

.PHONY: build run memcheck clean

build: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) $(LDLIBS) -o $(TARGET)

# Render the example fractals (plant.ppm, snowflake.ppm, dragon.ppm)
run: $(TARGET)
	./$(TARGET) < examples/demo_commands.txt

# Run the demo under Valgrind and fail on any leak or memory error
memcheck: $(TARGET)
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET) < examples/demo_commands.txt

clean:
	rm -f $(TARGET) *.ppm