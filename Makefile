CC= g++
CFLAGS= -std=c++11
LDFLAGS=

YACC= bison
YFLAGS= -d

LEX= flex
LFLAGS=

YGEN= parser.yy
YSRC= parser.cc
YHEADER= parser.hh

LGEN= lexer.ll
LSRC= lexer.cc

SRCS= $(YSRC) $(LSRC) codegen.cc
HEADERS= $(YHEADER) codegen.hh
OBJECTS= $(SRCS:.cc:.o)

TARGET= kaleidoscope

all: $(TARGET)

$(YSRC): $(YGEN)
	$(YACC) $(YFLAGS) -o $@ $<

$(LSRC): $(LGEN) $(YHEADER)
	$(LEX) $(LFLAGS) -o $@ $<

%.o: %.cc
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(TARGET) $(OBJECTS) $(YSRC) $(YHEADER) $(LSRC)
