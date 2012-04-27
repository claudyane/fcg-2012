CC=g++

SOURCES=*.cpp
EXECUTABLE=openGLGTK
OBJECTS=*.o

all:
	$(CC) -o $(EXECUTABLE) -g $(SOURCES) `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 gtkglext-x11-1.0`
	
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
