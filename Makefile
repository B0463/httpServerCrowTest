TARGET = bin/httpServerCrowTest
SRCS = src/main.cpp
OBJS = $(SRCS:src/%.cpp=build/%.o)

CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2 -ffunction-sections -fdata-sections
LDFLAGS = -Wl,--gc-sections -lpthread

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CXX) $(OBJS) $(LDFLAGS) -o $(TARGET)

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build bin

.PHONY: all clean
