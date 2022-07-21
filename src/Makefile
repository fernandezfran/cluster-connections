# compiler
CPP = g++

# flags
CPPFLAGS = -g -O3

# binary file
TARGET = cluster-connections

# files
SOURCES = input.cpp xyz_reader.cpp distance.cpp connections.cpp main.cpp
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

# rules
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CPP) $(CPPFLAGS) -o $@ $^

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< 

clean:
	rm -f $(TARGET) *.o

.PHONY: clean all
