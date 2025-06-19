CXX = g++
CXXFLAGS = -std=c++17 `pkg-config --cflags opencv4`
SRC = src/main.cpp src/ift.cpp src/mst.cpp src/utils.cpp
TARGET = main

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	LDFLAGS = $(shell pkg-config --libs opencv4)
else
	LDFLAGS = -lopencv_core -lopencv_imgcodecs -lopencv_highgui
endif

.PHONY: all clean $(TARGET)

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRC) -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
