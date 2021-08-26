CC=g++
CCFLAGS= -std=c++11

SRCS=2016310493.cpp

TARGET=cache-sim

OBJS := $(patsubst %.cpp,%.o,$(SRCS))

all: $(TARGET)

%.o: %.cpp
			$(CC) $(CCFLAGS) $< -c -o $@

$(TARGET): $(OBJS)
			$(CC) $(CCFLAGS) $^ -o $@

.PHONY=clean
.PHONY=run

run: $(TARGET)
			./$(TARGET) $(ARGS)

clean:
			rm -f $(OBJS) $(TARGET)