%language "C++"
%require "3.2"
%defines "headers/Parser.hpp"
%output "Parser.cpp"

%define api.parser.class {Parser}
%define api.namespace {C1}
%parse-param {Lexer* lexer}

%code requires {
    #include <string>
    #include <iostream>
    #include <vector>
    #include <stack>
    #include <cstring>
    #include "Attributes.hpp"
    #include "CodeGen.hpp"
    #include "TypeTab.hpp"
    #include "SymTab.hpp"
    #include "PilaTs.hpp"

    namespace C1 {
        class Lexer;
    }
}

%code {
    #include "headers/Lexer.hpp"

    // Definir yylex para que Bison llame a nuestro Lexer
    #define yylex(yylval) lexer->lex(yylval)

    // ========================================================
    // Variables globales del esquema de traducción
    // ========================================================
    TypeTab tablaTipos;                     // Tabla de tipos global            
    PilaTs pilaTs;                          // Pila de tablas de símbolos
    std::stack<int> pilaOffset;             // Pila de direcciones   
    std::stack<std::string> pilaBreak;
    std::stack<std::string> pilaReturn;       
    std::stack<std::string> pilaTrue;
    std::stack<std::string> pilaFalse;        
    std::stack<std::string> pilaDir;          
    int dir = 0;        // Dirección actual de memoria

    // Variable auxiliar para propagar L.tipo (atributo heredado)
    int currentType = 0;

    // Variable para el tipo de retorno de funciones
    int tipoReturnFunc = 0;

    // Lista de tipos de parámetros (para F.lista)
    std::vector<int> listaParams;
    std::vector<int> argsList; 

    // ========================================================
    // Funciones semánticas auxiliares
    // ========================================================
    bool compatibles(int t1, int t2) {
        if (t1 == t2) return true;
        std::string n1 = tablaTipos.getName(t1);
        std::string n2 = tablaTipos.getName(t2);
        if ((n1 == "int" || n1 == "float") && (n2 == "int" || n2 == "float")) return true;
        return false;
    }

    int maxTipo(int t1, int t2) {
        if (t1 == t2) return t1;
        std::string n1 = tablaTipos.getName(t1);
        std::string n2 = tablaTipos.getName(t2);
        if (n1 == "float" || n2 == "float") return tablaTipos.getId("float");
        return t1;
    }

    std::string ampliar(std::string dirStr, int t1, int t2) {
        if (t1 == t2) return dirStr;
        std::string tDest = tablaTipos.getName(t2);
        if (tDest == "float" && tablaTipos.getName(t1) == "int") {
            std::string temp = CodeGen::newTemp();
            CodeGen::emit("inttofloat", dirStr, "", temp);
            return temp;
        }
        return dirStr;
    }

    std::string reducir(std::string dirStr, int t1, int t2) {
        if (t1 == t2) return dirStr;
        std::string tDest = tablaTipos.getName(t2);
        if (tDest == "int" && tablaTipos.getName(t1) == "float") {
            std::string temp = CodeGen::newTemp();
            CodeGen::emit("floattoint", dirStr, "", temp);
            return temp;
        }
        return dirStr;
    }
}

// ========================================================
// Unión de valores semánticos
// ========================================================
%union {
    int ival;
    char* sval;
    struct {
        int tipo;           // Tipo resultante
        int base;           // Tipo base 
        char dir[32];     
        int tam;          
        char baseStr[32]; 
    } attr;
}

// ========================================================
// Tokens (terminales)
// ========================================================
%token <sval> ID FLOATLIT CHARLIT
%token <ival> INTLIT
%token INT FLOAT BOOL CHAR STRUCT
%token DEF BREAK RETURN IF ELSE WHILE FOR
%token TRUE FALSE
%token SEMICOLON COMMA DOT
%token LBRACKET RBRACKET LBRACE RBRACE LPAREN RPAREN
%token ASIGNACION MAS MENOS MULT DIV MOD
%token OR AND MAYOR_QUE MENOR_QUE MAYOR_IGUAL MENOR_IGUAL NO_IGUAL IGUAL

// ========================================================
// Tipos de los no-terminales
// ========================================================
%type <attr> P H D T B A L F G R S E N M C Z S_ASIG M_IF

// Precedencia para expresiones
%left OR
%left AND
%left IGUAL NO_IGUAL
%left MAYOR_QUE MENOR_QUE MAYOR_IGUAL MENOR_IGUAL
%left MAS MENOS
%left MULT DIV MOD 

// Un confilcto esperado: Dangling-else
%expect 1

// ========================================================
// Símbolo inicial
// ========================================================
%start P

%%

// ============================================================
// Acciones semánticas
// ============================================================

P : 
    {
        dir = 0;
        pilaTs.push(new SymTab());
        CodeGen::reset();
    }
    H
    {
        std::cout << "\n========== RESULTADOS DEL COMPILADOR ==========\n";
        tablaTipos.print();
        std::cout << "\n";
        pilaTs.bottom()->print();
        std::cout << "\n========== CODIGO INTERMEDIO (TAC) ==========\n";
        CodeGen::print(std::cout);
        std::cout << "===============================================\n";
    }
  ; 

H : D H | /* ε */ { } ;

D : T L SEMICOLON
  | STRUCT ID LBRACE 
    {
        std::string id = $2;
        if (!pilaTs.bottom()->existe(id)) {
            pilaTs.push(new SymTab());
            pilaOffset.push(dir);
            dir = 0;
        } else {
            std::cerr << "Error: Estructura duplicada" << std::endl;
        }
    }
    H RBRACE SEMICOLON
    {
        std::string id = $2;
        SymTab* ts = pilaTs.pop();
        int tipoStruct = tablaTipos.addStructType(dir, ts);
        pilaTs.bottom()->addSym(id, -1, tipoStruct, "struct");
        dir = pilaOffset.top();
        pilaOffset.pop();
        if ($2) free($2);
    }
  | DEF T ID LPAREN 
    {
        std::string id = $3;
        if (!pilaTs.bottom()->existe(id)) {
            pilaTs.push(new SymTab());
            pilaOffset.push(dir);
            dir = 0;
            tipoReturnFunc = $2.tipo;
            CodeGen::emit("label", "", "", id);
            listaParams.clear();
        } else {
            std::cerr << "Error: Función duplicada" << std::endl;
        }
    }
    F RPAREN LBRACE H R RBRACE
    {
        std::string id = $3;
        pilaTs.pop();
        pilaTs.bottom()->addSym(id, -1, $2.tipo, "func", listaParams);
        dir = pilaOffset.top();
        pilaOffset.pop();
        if ($3) free($3);
    }
  ;

T : B A
    {
        $$.tipo = $2.tipo;
        currentType = $$.tipo;
    }
  ;

B : INT   { $$.tipo = tablaTipos.getId("int");   $$.base = $$.tipo; currentType = $$.tipo; }
  | FLOAT { $$.tipo = tablaTipos.getId("float"); $$.base = $$.tipo; currentType = $$.tipo; }
  | BOOL  { $$.tipo = tablaTipos.getId("bool");  $$.base = $$.tipo; currentType = $$.tipo; }
  | CHAR  { $$.tipo = tablaTipos.getId("char");  $$.base = $$.tipo; currentType = $$.tipo; }
  | STRUCT ID 
    {
        std::string id = $2;
        int idStruct = -1;
        
        if (pilaTs.bottom()->existe(id)) {
            idStruct = pilaTs.bottom()->getType(id);
        }

        if (idStruct != -1) {
            $$.tipo = idStruct;
            $$.base = idStruct;
            currentType = idStruct;
        } else {
            std::cerr << "Error: Struct no declarado '" << id << "'" << std::endl;
            $$.tipo = 0; // 0 = int. Se pone un tipo seguro para evitar colapsar la memoria basura
            currentType = 0;
        }
        if ($2) free($2);
    }
  ;

A : LBRACKET INTLIT RBRACKET A
    {
        if ($2 > 0) {
            $$.tipo = tablaTipos.addArrayType($2, $4.tipo);
        } else {
            std::cerr << "Error: El índice debe ser mayor a cero" << std::endl;
            $$.tipo = currentType;
        }
        $$.base = currentType;
    }
  | /* ε */
    {
        $$.tipo = currentType;
        $$.base = currentType;
    }
  ;

L : L COMMA ID
    {
        std::string id = $3;
        if (!pilaTs.top()->existe(id)) {
            pilaTs.top()->addSym(id, dir, currentType, "var");
            dir += tablaTipos.getTam(currentType);
        } else {
            std::cerr << "Error: Variable duplicada" << std::endl;
        }
        if ($3) free($3);
    }
  | ID
    {
        std::string id = $1;
        if (!pilaTs.top()->existe(id)) {
            pilaTs.top()->addSym(id, dir, currentType, "var");
            dir += tablaTipos.getTam(currentType);
        } else {
            std::cerr << "Error: Variable duplicada" << std::endl;
        }
        if ($1) free($1);
    }
  ;

F : G | /* ε */ { } ;

G : G COMMA T ID
    {
        std::string id = $4;
        if (!pilaTs.top()->existe(id)) {
            pilaTs.top()->addSym(id, dir, $3.tipo, "arg");
            dir += tablaTipos.getTam($3.tipo);
            listaParams.push_back($3.tipo);
        } else {
            std::cerr << "Error: Parámetro duplicado" << std::endl;
        }
        if ($4) free($4);
    }
  | T ID
    {
        std::string id = $2;
        if (!pilaTs.top()->existe(id)) {
            pilaTs.top()->addSym(id, dir, $1.tipo, "arg");
            dir += tablaTipos.getTam($1.tipo);
            listaParams.push_back($1.tipo);
        } else {
            std::cerr << "Error: Parámetro duplicado" << std::endl;
        }
        if ($2) free($2);
    }
  ;

R : S R | /* ε */ { } ;

S_ASIG : ID ASIGNACION E 
    {
        std::string id = $1;
        int tipoDest = -1;
        if (pilaTs.top()->existe(id)) tipoDest = pilaTs.top()->getType(id);
        else if (pilaTs.bottom()->existe(id)) tipoDest = pilaTs.bottom()->getType(id);
        else std::cerr << "Error: El id no fue declarado: " << id << std::endl;

        if (tipoDest != -1 && compatibles(tipoDest, $3.tipo)) {
            std::string a1 = reducir($3.dir, $3.tipo, tipoDest);
            CodeGen::emit("=", a1, "", id);
        } else {
            std::cerr << "Error: Tipos incompatibles" << std::endl;
        }
        if ($1) free($1);
    }
  | C ASIGNACION E 
    {
        if (compatibles($1.tipo, $3.tipo)) {
            std::string a1 = reducir($3.dir, $3.tipo, $1.tipo);
            std::string res = std::string($1.baseStr) + "[" + $1.dir + "]";
            CodeGen::emit("=", a1, "", res);
        } else {
            std::cerr << "Error: Tipos incompatibles en arreglo" << std::endl;
        }
    }
  | Z ASIGNACION E 
    {
        if (compatibles($1.tipo, $3.tipo)) {
            std::string a1 = reducir($3.dir, $3.tipo, $1.tipo);
            std::string res = std::string($1.baseStr) + "[" + std::to_string($1.tam) + "]"; 
            CodeGen::emit("=", a1, "", res);
        } else {
            std::cerr << "Error: Tipos incompatibles" << std::endl;
        }
    }
  ;

// para evitar conflictos del if else
M_IF : IF LPAREN E RPAREN 
    {
        std::string lTrue = CodeGen::newLabel();
        std::string lFalse = CodeGen::newLabel();
        CodeGen::emit("if", $3.dir, "", lTrue);
        CodeGen::emit("goto", "", "", lFalse);
        CodeGen::emit("label", "", "", lTrue);
        pilaTrue.push(lTrue);
        pilaFalse.push(lFalse);
        memset(&$$, 0, sizeof($$)); // Limpiamos para evitar warnings
    }
  ;

S : BREAK SEMICOLON
    {
        CodeGen::emit("goto", "", "", pilaBreak.top());
    }
  | RETURN E SEMICOLON
    {
        if (compatibles(tipoReturnFunc, $2.tipo)) {
            CodeGen::emit("return", "", "", $2.dir);
        } else {
            std::cerr << "Error: Tipo de retorno incorrecto" << std::endl;
        }
    }
  | S_ASIG SEMICOLON
  | M_IF S 
    {
        std::string lFalse = pilaFalse.top();
        CodeGen::emit("label", "", "", lFalse);
        pilaTrue.pop();
        pilaFalse.pop();
    }
  | M_IF S ELSE 
    {
        std::string lNext = CodeGen::newLabel();
        std::string lFalse = pilaFalse.top();
        CodeGen::emit("goto", "", "", lNext);
        CodeGen::emit("label", "", "", lFalse);
        pilaDir.push(lNext); 
    } 
    S 
    {
        std::string lNext = pilaDir.top();
        CodeGen::emit("label", "", "", lNext);
        pilaTrue.pop();
        pilaFalse.pop();
        pilaDir.pop();
    }
  | WHILE 
    {
        std::string lNext = CodeGen::newLabel(); 
        CodeGen::emit("label", "", "", lNext);
        pilaDir.push(lNext); 
    }
    LPAREN E RPAREN
    {
        std::string lTrue = CodeGen::newLabel();
        std::string lFalse = CodeGen::newLabel();
        CodeGen::emit("if", $4.dir, "", lTrue);
        CodeGen::emit("goto", "", "", lFalse);
        CodeGen::emit("label", "", "", lTrue);
        pilaBreak.push(lFalse); 
        pilaTrue.push(lTrue);
        pilaFalse.push(lFalse);
    }
    S
    {
        std::string lNext = pilaDir.top();
        std::string lFalse = pilaFalse.top();
        CodeGen::emit("goto", "", "", lNext);
        CodeGen::emit("label", "", "", lFalse);
        pilaTrue.pop();
        pilaFalse.pop();
        pilaDir.pop();
        pilaBreak.pop();
    }
  | FOR LPAREN S_ASIG SEMICOLON 
    {
        std::string lNext = CodeGen::newLabel(); 
        CodeGen::emit("label", "", "", lNext);
        pilaDir.push(lNext);
    }
    E SEMICOLON
    {
        std::string lTrue = CodeGen::newLabel();
        std::string lFalse = CodeGen::newLabel();
        std::string lInc = CodeGen::newLabel(); 
        CodeGen::emit("if", $6.dir, "", lTrue);
        CodeGen::emit("goto", "", "", lFalse);
        CodeGen::emit("label", "", "", lInc);
        
        pilaTrue.push(lTrue);
        pilaFalse.push(lFalse);
        pilaReturn.push(lInc); 
        pilaBreak.push(lFalse); 
    }
    S_ASIG 
    {
        std::string lNext = pilaDir.top();
        std::string lTrue = pilaTrue.top();
        CodeGen::emit("goto", "", "", lNext);
        CodeGen::emit("label", "", "", lTrue);
    }
    RPAREN S
    {
        std::string lInc = pilaReturn.top();
        std::string lFalse = pilaFalse.top();
        CodeGen::emit("goto", "", "", lInc);
        CodeGen::emit("label", "", "", lFalse);
        
        pilaTrue.pop();
        pilaFalse.pop();
        pilaDir.pop();
        pilaBreak.pop();
        pilaReturn.pop();
    }
    | LBRACE R RBRACE 
    { memset(&$$, 0, sizeof($$)); }
  ;

E : E MAS E
    {
        if (compatibles($1.tipo, $3.tipo)) {
            $$.tipo = maxTipo($1.tipo, $3.tipo);
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            std::string a1 = ampliar($1.dir, $1.tipo, $$.tipo);
            std::string a2 = ampliar($3.dir, $3.tipo, $$.tipo);
            CodeGen::emit("+", a1, a2, $$.dir);
        } else {
            std::cerr << "Error: Tipos incompatibles" << std::endl;
        }
    }
  | E MENOS E 
    {
        if (compatibles($1.tipo, $3.tipo)) {
            $$.tipo = maxTipo($1.tipo, $3.tipo);
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            std::string a1 = ampliar($1.dir, $1.tipo, $$.tipo);
            std::string a2 = ampliar($3.dir, $3.tipo, $$.tipo);
            CodeGen::emit("-", a1, a2, $$.dir);
        } else {
            std::cerr << "Error: Tipos incompatibles" << std::endl;
        }
    }
  | E MULT E 
    {
        if (compatibles($1.tipo, $3.tipo)) {
            $$.tipo = maxTipo($1.tipo, $3.tipo);
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            std::string a1 = ampliar($1.dir, $1.tipo, $$.tipo);
            std::string a2 = ampliar($3.dir, $3.tipo, $$.tipo);
            CodeGen::emit("*", a1, a2, $$.dir);
        } else {
            std::cerr << "Error: Tipos incompatibles" << std::endl;
        }
    }
  | E DIV E 
    {
        if (compatibles($1.tipo, $3.tipo)) {
            $$.tipo = maxTipo($1.tipo, $3.tipo);
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            std::string a1 = ampliar($1.dir, $1.tipo, $$.tipo);
            std::string a2 = ampliar($3.dir, $3.tipo, $$.tipo);
            CodeGen::emit("/", a1, a2, $$.dir);
        } else {
            std::cerr << "Error: Tipos incompatibles" << std::endl;
        }
    }
  | E MOD E
    {
        if (tablaTipos.getName($1.tipo) == "int" && tablaTipos.getName($3.tipo) == "int") {
            $$.tipo = tablaTipos.getId("int");
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            CodeGen::emit("%", $1.dir, $3.dir, $$.dir);
        } else {
            std::cerr << "Error: El operador módulo requiere enteros" << std::endl;
        }
    }
  | E OR E
    {
        if (tablaTipos.getName($1.tipo) == "bool" && tablaTipos.getName($3.tipo) == "bool") {
            $$.tipo = tablaTipos.getId("bool");
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            CodeGen::emit("||", $1.dir, $3.dir, $$.dir);
        } else {
            std::cerr << "Error: Tipos incompatibles" << std::endl;
        }
    }
  | E AND E 
    {
        if (tablaTipos.getName($1.tipo) == "bool" && tablaTipos.getName($3.tipo) == "bool") {
            $$.tipo = tablaTipos.getId("bool");
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            CodeGen::emit("&&", $1.dir, $3.dir, $$.dir);
        } else {
            std::cerr << "Error: Tipos incompatibles" << std::endl;
        }
    }
  | E MAYOR_QUE E
    {
        if (compatibles($1.tipo, $3.tipo)) {
            $$.tipo = tablaTipos.getId("bool");
            int maxT = maxTipo($1.tipo, $3.tipo);
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            CodeGen::emit(">", ampliar($1.dir, $1.tipo, maxT), ampliar($3.dir, $3.tipo, maxT), $$.dir);
        } else { std::cerr << "Error: Tipos incompatibles" << std::endl; }
    }
  | E MENOR_QUE E 
    {
        if (compatibles($1.tipo, $3.tipo)) {
            $$.tipo = tablaTipos.getId("bool");
            int maxT = maxTipo($1.tipo, $3.tipo);
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            CodeGen::emit("<", ampliar($1.dir, $1.tipo, maxT), ampliar($3.dir, $3.tipo, maxT), $$.dir);
        } else { std::cerr << "Error: Tipos incompatibles" << std::endl; }
    }
  | E MAYOR_IGUAL E 
    {
        if (compatibles($1.tipo, $3.tipo)) {
            $$.tipo = tablaTipos.getId("bool");
            int maxT = maxTipo($1.tipo, $3.tipo);
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            CodeGen::emit(">=", ampliar($1.dir, $1.tipo, maxT), ampliar($3.dir, $3.tipo, maxT), $$.dir);
        } else { std::cerr << "Error: Tipos incompatibles" << std::endl; }
    }
  | E MENOR_IGUAL E 
    {
        if (compatibles($1.tipo, $3.tipo)) {
            $$.tipo = tablaTipos.getId("bool");
            int maxT = maxTipo($1.tipo, $3.tipo);
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            CodeGen::emit("<=", ampliar($1.dir, $1.tipo, maxT), ampliar($3.dir, $3.tipo, maxT), $$.dir);
        } else { std::cerr << "Error: Tipos incompatibles" << std::endl; }
    }
  | E IGUAL E 
    {
        if (compatibles($1.tipo, $3.tipo)) {
            $$.tipo = tablaTipos.getId("bool");
            int maxT = maxTipo($1.tipo, $3.tipo);
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            CodeGen::emit("==", ampliar($1.dir, $1.tipo, maxT), ampliar($3.dir, $3.tipo, maxT), $$.dir);
        } else { std::cerr << "Error: Tipos incompatibles" << std::endl; }
    }
  | E NO_IGUAL E 
    {
        if (compatibles($1.tipo, $3.tipo)) {
            $$.tipo = tablaTipos.getId("bool");
            int maxT = maxTipo($1.tipo, $3.tipo);
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            CodeGen::emit("!=", ampliar($1.dir, $1.tipo, maxT), ampliar($3.dir, $3.tipo, maxT), $$.dir);
        } else { std::cerr << "Error: Tipos incompatibles" << std::endl; }
    }
  | LPAREN E RPAREN
    {
        $$.tipo = $2.tipo;
        $$.tam = $2.tam;
        strcpy($$.dir, $2.dir);
        strcpy($$.baseStr, $2.baseStr);
    }
  | C
    {
        $$.tipo = $1.tipo;
        std::string t = CodeGen::newTemp();
        strcpy($$.dir, t.c_str());
        std::string rhs = std::string($1.baseStr) + "[" + $1.dir + "]";
        CodeGen::emit("=", rhs, "", $$.dir);
    }
  | Z
    {
        $$.tipo = $1.tipo;
        std::string t = CodeGen::newTemp();
        strcpy($$.dir, t.c_str());
        std::string rhs = std::string($1.baseStr) + "[" + std::to_string($1.tam) + "]";
        CodeGen::emit("=", rhs, "", $$.dir);
    }
  | ID
    {
        std::string id = $1;
        if (pilaTs.top()->existe(id)) {
            $$.tipo = pilaTs.top()->getType(id);
            strcpy($$.dir, id.c_str());
        } else if (pilaTs.bottom()->existe(id)) {
            $$.tipo = pilaTs.bottom()->getType(id);
            strcpy($$.dir, id.c_str());
        } else {
            std::cerr << "Error: El id no fue declarado: " << id << std::endl;
        }
        if ($1) free($1);
    }
  | ID LPAREN N RPAREN
    {
        std::string id = $1;
        if (pilaTs.bottom()->existe(id) && pilaTs.bottom()->getCat(id) == "func") {
            $$.tipo = pilaTs.bottom()->getType(id);
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            CodeGen::emit("call", id, std::to_string(argsList.size()), $$.dir);
        } else {
            std::cerr << "Error: El identificador no es una función declarada" << std::endl;
        }
        if ($1) free($1);
        argsList.clear(); 
    }
  | INTLIT
    {
        $$.tipo = tablaTipos.getId("int");
        strcpy($$.dir, std::to_string($1).c_str());
    }
  | FLOATLIT
    {
        $$.tipo = tablaTipos.getId("float");
        strcpy($$.dir, $1);
        if ($1) free($1);
    }
  | TRUE
    {
        $$.tipo = tablaTipos.getId("bool");
        strcpy($$.dir, "1");
    }
  | FALSE
    {
        $$.tipo = tablaTipos.getId("bool");
        strcpy($$.dir, "0");
    }
  | CHARLIT
    {
        $$.tipo = tablaTipos.getId("char");
        strcpy($$.dir, $1);
        if ($1) free($1);
    }
  ;

N : M | /* ε */ { } ;

M : M COMMA E
    {
        argsList.push_back($3.tipo);
        CodeGen::emit("param", $3.dir);
    }
  | E
    {
        argsList.push_back($1.tipo);
        CodeGen::emit("param", $1.dir);
    }
  ;

C : ID LBRACKET E RBRACKET
    {
        std::string id = $1;
        int tipoArray = -1;
        if (pilaTs.top()->existe(id)) tipoArray = pilaTs.top()->getType(id);
        else if (pilaTs.bottom()->existe(id)) tipoArray = pilaTs.bottom()->getType(id);
        
        if (tablaTipos.getName(tipoArray) == "array" && tablaTipos.getName($3.tipo) == "int") {
            strcpy($$.baseStr, id.c_str());
            $$.tipo = tablaTipos.getTipoBase(tipoArray);
            $$.tam = tablaTipos.getTam($$.tipo);
            std::string t = CodeGen::newTemp();
            strcpy($$.dir, t.c_str());
            CodeGen::emit("*", $3.dir, std::to_string($$.tam), t);
        }
        if ($1) free($1);
    }
  | C LBRACKET E RBRACKET
    {
        if (tablaTipos.getName($1.tipo) == "array" && tablaTipos.getName($3.tipo) == "int") {
            strcpy($$.baseStr, $1.baseStr);
            $$.tipo = tablaTipos.getTipoBase($1.tipo);
            $$.tam = tablaTipos.getTam($$.tipo);
            std::string t = CodeGen::newTemp();
            std::string dirDest = CodeGen::newTemp();
            CodeGen::emit("*", $3.dir, std::to_string($$.tam), t);
            CodeGen::emit("+", $1.dir, t, dirDest);
            strcpy($$.dir, dirDest.c_str());
        }
    }
  ;

Z : ID DOT ID
    {
        std::string id = $1;
        int tipoBase = -1;
        if (pilaTs.top()->existe(id)) tipoBase = pilaTs.top()->getType(id);
        else if (pilaTs.bottom()->existe(id)) tipoBase = pilaTs.bottom()->getType(id);
        
        strcpy($$.baseStr, id.c_str());
        $$.tam = 0;
        $$.tipo = 0; 
        
        if (tipoBase != -1) {
            SymTab* ts = tablaTipos.getTS(tipoBase);
            if (ts && ts->existe($3)) {
                $$.tipo = ts->getType($3);
                $$.tam = ts->getDir($3);
            } else {
                std::cerr << "Error: El campo '" << $3 << "' no existe en el struct" << std::endl;
            }
        } else {
            std::cerr << "Error: Variable '" << id << "' no declarada" << std::endl;
        }
        if ($1) free($1);
        if ($3) free($3);
    }
  | Z DOT ID
    {
        SymTab* ts = tablaTipos.getTS($1.tipo);
        strcpy($$.baseStr, $1.baseStr);
        $$.tam = $1.tam; 
        $$.tipo = 0;    
        
        if (ts && ts->existe($3)) {
            $$.tipo = ts->getType($3);
            $$.tam = $1.tam + ts->getDir($3); 
        } else {
            std::cerr << "Error: El campo '" << $3 << "' no existe" << std::endl;
        }
        if ($3) free($3);
    }
  ;

%%

void C1::Parser::error(const std::string& msg) {
    std::cerr << "Error de sintaxis: " << msg << std::endl;
}