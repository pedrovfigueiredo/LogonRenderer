EXECUTABLE = swpathtracer

CXX = g++

CXXFLAGS = -std=c++11 -Wall -Wextra -Wpedantic

ifeq ($(DEBUG), 1)
CXXFLAGS += -DDEBUG -O0 -g3
else
CXXFLAGS += -DNDEBUG -O3 -g0
endif

CPPFLAGS += -MMD \
			-I/usr/local/Cellar/glm/ \
            -I/usr/local/Cellar/imagemagick/7.0.8-3/include/ImageMagick-7 \
            -I/usr/local/Cellar/assimp/ \
			-I/usr/local/include \
			-I.

LDFLAGS = -L/usr/lib/x86_64-linux-gnu \
		  -L/usr/lib \
		  -L/usr/local/lib \
		  -L.

LDLIBS = -lassimp \
         -DMAGICKCORE_HDRI_ENABLE=1 \
         -DMAGICKCORE_QUANTUM_DEPTH=16 \
         -I/usr/local/Cellar/imagemagick/7.0.8-3/include/ImageMagick-7 \
         -L/usr/local/Cellar/imagemagick/7.0.8-3/lib \
         -lMagick++-7.Q16HDRI \
         -lMagickWand-7.Q16HDRI \
         -lMagickCore-7.Q16HDRI \
         -L/usr/local/Cellar/imagemagick/7.0.8-3/lib

SOURCES = $(wildcard *.cpp)

OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDLIBS)

.cpp.o:

-include $(OBJECTS:.o=.d)

.PHONY: clean distclean

clean:
	rm *.o *.d

distclean: clean
	rm $(EXECUTABLE)

