TARGET  = a.out
GEN_SOURCES = parser.cpp tokens.cpp
GEN_HEADERS = parser.hpp

CPPFLAGS = -std=c++11
LDFLAGS = -rdynamic

OBJS = 	main.o    \
		parser.o  \
		tokens.o  \

FILES = $(TARGET) $(GEN_HEADERS) $(GEN_SOURCES) $(OBJS)

all: clean $(TARGET)

$(TARGET): $(GEN_SOURCES) $(GEN_HEADERS) $(OBJS)
	g++ -g -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	g++ -g -O3 -c $(CPPFLAGS) -o $@ $<
	
parser.hpp: parser.cpp

parser.cpp: parser.y
	bison -d -o $@ $^

tokens.cpp: scanner.l parser.hpp
	flex -o $@ $^

clean:
	rm -rf $(FILES) output