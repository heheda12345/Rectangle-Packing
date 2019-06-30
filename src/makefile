SOURCES=$(wildcard *.cpp)
HEADERS=$(wildcard *.h)
OBJECTS=$(SOURCES:%.cpp=%.o)
TARGET=main.exe

all: $(TARGET)
$(TARGET): $(OBJECTS) $(HEADERS)
	g++ $(OBJECTS) -o $(TARGET) -std=c++0x -I D:\library\boost_1_67_0
%.o: %.cpp $(HEADERS)
	g++ -c $< -o $@ -std=c++0x -I D:\library\boost_1_67_0
.PHONY: clean
clean:
	del *.o *.exe
