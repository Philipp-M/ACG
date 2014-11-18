CXX=g++
CXXFLAGS=-c -Wall -O3 -fopenmp -std=c++11
#-L./SDL2/lib -Wall,-rpath,./SDL2/lib -lSDL2 -lpthread -Wall,--no-undefined -lm -ldl -lpthread -lrt
LDFLAGS= -lpthread -lSDL2 -fopenmp
#INCLUDES= -I./SDL2/include
SOURCES=main.cpp GScene.cpp GSphere.cpp GTriangle.cpp GSimpleAccelStruct.cpp GPolygonObject.cpp ObjLoader.cpp tiny_obj_loader.cpp SDLViewer.cpp GBVHAccelStruct.cpp CameraModel.cpp Motion.cpp MotionType.cpp TextureManager.cpp GTexturedTriangle.cpp
DEPENDENCIES=SmallPT.hpp GScene.hpp GObject.hpp GSphere.hpp DataTypes.hpp GTriangle.hpp GAccelStruct.hpp GSimpleAccelStruct.hpp GPolygonObject.hpp ObjLoader.hpp tiny_obj_loader.hpp SDLViewer.hpp GBVHAccelStruct.hpp CameraModel.hpp Motion.hpp MotionType.hpp TextureManager.hpp GTexturedTriangle.hpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=aSmallPt
EXECUTABLE_DEBUG=aSmallPt_debug

all: $(SOURCES) $(EXECUTABLE)

debug: CXXFLAGS += -g -O0 -Wall -Wextra
debug: clean $(EXECUTABLE_DEBUG)

$(EXECUTABLE): $(OBJECTS) $(DEPENDENCIES)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	
$(EXECUTABLE_DEBUG): $(OBJECTS) $(DEPENDENCIES)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o: $(DEPENDENCIES)
	$(CXX) $(INCLUDES) $(CXXFLAGS) $< -o $@
	
.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) $(EXECUTABLE_DEBUG) $(OBJECTS)