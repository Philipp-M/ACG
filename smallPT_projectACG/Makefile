CXX=g++
CXXFLAGS=-c -Wall -O3 -fopenmp
#-L./SDL2/lib -Wall,-rpath,./SDL2/lib -lSDL2 -lpthread -Wall,--no-undefined -lm -ldl -lpthread -lrt
LDFLAGS= -lpthread -fopenmp 
INCLUDES= -I./SDL2/include
SOURCES=main.cpp GScene.cpp GSphere.cpp GTriangle.cpp GSimpleAccelStruct.cpp GPolygonObject.cpp ObjLoader.cpp
DEPENDENCIES=SmallPT.hpp GScene.hpp GObject.hpp GPrimitiveObject.hpp GSphere.hpp DataTypes.hpp GTriangle.hpp GAccelStruct.hpp GSimpleAccelStruct.hpp GPolygonObject.hpp ObjLoader.hpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=aSmallPt
EXECUTABLE_DEBUG=aSmallPt_debug

all: $(SOURCES) $(EXECUTABLE)

debug: CXXFLAGS += -g -O0 -Wall -Wextra
debug: clean $(EXECUTABLE_DEBUG)

$(EXECUTABLE): $(OBJECTS) $(DEPENDENCIES)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@
	
$(EXECUTABLE_DEBUG): $(OBJECTS) $(DEPENDENCIES)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o: $(DEPENDENCIES)
	$(CC) $(INCLUDES) $(CXXFLAGS) $< -o $@
	
.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) $(EXECUTABLE_DEBUG) $(OBJECTS)