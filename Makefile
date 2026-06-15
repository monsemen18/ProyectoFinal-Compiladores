CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -I. -Iheaders
FLEX = flex
BISON = bison

# Archivos fuente 
SRCS = src/main.cpp src/TypeTab.cpp src/SymTab.cpp src/PilaTs.cpp src/Tac.cpp src/CodeGen.cpp
OBJS = src/main.o src/TypeTab.o src/SymTab.o src/PilaTs.o src/Tac.o src/CodeGen.o src/Parser.o src/Lexer.o

# Ejecutable
TARGET = comp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Generar Parser con Bison 
src/Parser.cpp headers/Parser.hpp: src/parser.yy
	$(BISON) src/parser.yy
	mv -f Parser.cpp src/Parser.cpp

# Generar Lexer con Flex 
src/Lexer.cpp: src/lexer.ll headers/Parser.hpp
	$(FLEX) src/lexer.ll
	mv -f Lexer.cpp src/Lexer.cpp

# Compilar archivos objeto
src/main.o: src/main.cpp headers/Parser.hpp headers/Lexer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

src/TypeTab.o: src/TypeTab.cpp headers/TypeTab.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

src/SymTab.o: src/SymTab.cpp headers/SymTab.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

src/PilaTs.o: src/PilaTs.cpp headers/PilaTs.hpp headers/SymTab.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

src/Tac.o: src/Tac.cpp headers/Tac.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

src/CodeGen.o: src/CodeGen.cpp headers/CodeGen.hpp headers/Tac.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

src/Parser.o: src/Parser.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

src/Lexer.o: src/Lexer.cpp headers/Lexer.hpp headers/Parser.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS) src/Parser.cpp headers/Parser.hpp src/Lexer.cpp

.PHONY: all clean
