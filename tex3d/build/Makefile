CC=g++

SOURCES=../src/*.cpp
EXECUTABLE=tex3d

OUTPUT_DIR = ../bin

ifeq ($(DBG),Yes)
  CFLAGS = -g -fopenmp `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 gtkglext-x11-1.0 glu`
  BUILD_DIR = debug
else
  CFLAGS = -fopenmp `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 gtkglext-x11-1.0 glu`
  BUILD_DIR = release
endif

.SILENT:

all:
	$(CC) -o $(OUTPUT_DIR)/$(BUILD_DIR)/$(EXECUTABLE) $(SOURCES) $(CFLAGS)

clean: 
	rm -f ../bin/$(BUILD_DIR)/$(EXECUTABLE)	



