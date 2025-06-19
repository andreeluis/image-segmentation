CXX = g++
CXXFLAGS = -std=c++17 -I/usr/include/opencv4
LDFLAGS = -lopencv_core -lopencv_imgcodecs -lopencv_highgui
SRC = src/main.cpp src/ift.cpp src/mst.cpp src/utils.cpp
TARGET = main

.PHONY: all clean $(TARGET)

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRC) -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)