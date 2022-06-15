CC= g++
CFLAGS= -std=c++11 -g $(shell llvm-config --cxxflags)
LDFLAGS= -g $(shell llvm-config --ldflags --system-libs --libs core native mcjit)

YACC= bison
YFLAGS= -d

LEX= flex
LFLAGS=

YGEN= kaleidoscope.y
YSRC= kal.parser.gen.cc
YHEADER= kal.parser.gen.hh

LGEN= kaleidoscope.l
LSRC= kal.lexer.gen.cc

SRCS= $(YSRC) $(LSRC) \
	ast_node.cc \
	visitor.cc \
	utility.cc \
	main.cc \
	print_json_visitor.cc \
	codegen_visitor.cc

HEADERS= $(YHEADER) \
	ast_node.hh \
	visitor.hh \
	utility.cc \
	main.hh \
	print_json_visitor.hh \
	codegen_visitor.hh

OBJECTS= $(SRCS:.cc=.o)

TARGET= kcc

all: $(TARGET)

$(YSRC): $(YGEN)
	$(YACC) $(YFLAGS) -o $@ $<

$(LSRC): $(LGEN) $(YHEADER)
	$(LEX) $(LFLAGS) -o $@ $<

%.o: %.cc %.hh
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(TARGET) $(OBJECTS) $(YSRC) $(YHEADER) $(LSRC)
