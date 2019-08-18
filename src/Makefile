TARGET  = a.out
GEN_SOURCES = parser.cpp tokens.cpp
GEN_HEADERS = parser.hpp
OUTPUT = llvm.out

LLVMCONFIG = llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++11
LDFLAGS = `$(LLVMCONFIG) --ldflags` -rdynamic
LIBS = `$(LLVMCONFIG) --libs`

OBJS = 	main.o    \
		parser.o  \
		tokens.o  \
       	codegen.o \
       	print.o  \

FILES = $(TARGET) $(GEN_HEADERS) $(GEN_SOURCES) $(OUTPUT) $(OBJS)

all: clean $(TARGET)

$(TARGET): $(GEN_SOURCES) $(GEN_HEADERS) $(OBJS)
	g++ -g -o $@ $(OBJS) $(LIBS) $(LDFLAGS)

%.o: %.cpp
	g++ -g -O3 -c $(CPPFLAGS) -o $@ $<
	
parser.hpp: parser.cpp

parser.cpp: parser.y
	bison -d -o $@ $^

tokens.cpp: scanner.l parser.hpp
	flex -o $@ $^

clean:
	rm -rf $(FILES) output