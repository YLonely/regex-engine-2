TARGET := example1

CC		:= g++
C_FLAGS := -std=c++11

BUILD := bin
EXAMPLES := examples
DEBUG := -g
INCLUDE := include

SRCS := $(wildcard src/*.cpp) $(EXAMPLES)/$(TARGET).cpp
INCS := $(wildcard include/*.h)


ifeq ($(OS),Windows_NT)
EXECUTABLE	:= $(TARGET).exe
else
EXECUTABLE	:= $(TARGET)
endif

all: build_check $(BUILD)/$(EXECUTABLE)

build_check:
	@if [ ! -d ./$(BUILD) ];then mkdir $(BUILD);fi

$(BUILD)/$(EXECUTABLE): $(SRCS) $(INCS)
	$(CC) $(DEBUG) $(SRCS) -o $@ -I$(INCLUDE) $(C_FLAGS)

.PHONY: clean
clean:
	@rm -rf $(BUILD)
