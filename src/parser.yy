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
    TypeTab tablaTipos;               // Tabla de tipos global
    PilaTs pilaTs;                    // Pila de tablas de símbolos
    std::stack<int> pilaOffset;       // Pila de direcciones
    std::stack<std::string> pilaBreak;        // Pila break
    std::stack<std::string> pilaReturn;       // Pila return para etiquetas o lugares para retornar
    std::stack<std::string> pilaTrue;         // Pila true. Etiquetas para rama true
    std::stack<std::string> pilaFalse;        // Pila false. Etiquetas para rama false
    std::stack<std::string> pilaNext;          // Pila next. Etiquetas para siguiente iteracion
    int dir = 0;                      // Dirección actual de memoria

    // Variable auxiliar para propagar L.tipo (atributo heredado)
    int currentType = 0;

    // Variable para el tipo de retorno de funciones
    int tipoReturnFunc = 0;

    // Lista de tipos de parámetros (para F.lista)
    std::vector<int> listaParams;

    std::vector<int> argsList; 

    // Funciones Semánticas Auxiliares
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
    int ival;              // Para números enteros (NUM)
    char* sval;            // Para identificadores (ID)
    struct {
        int tipo;          // Tipo resultante
        int base;          // Tipo base (heredado en A)
        char dir[32];
        int tam;
        char baseStr[32];
    } attr;                // Para T, B, A
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
%type <attr> P, H, D, T, B, A, L, F, G, R, S, E, N, M, C, Z, S_ASIG

%left OR
%left AND
%left IGUAL NO_IGUAL
%left MAYOR_QUE MENOR_QUE MAYOR_IGUAL MENOR_IGUAL
%left MAS MENOS
%left MULT DIV MOD 

// ========================================================
// Símbolo inicial
// ========================================================
%start P

%%

// ============================================================
// Acciones semánticas
// ============================================================

// P → H
P :
    {
        dir = 0;
        pilaTs.push(new SymTab());
        CodeGen::reset();
    }
    H
  ; 

// H → D H 
//    | ε 
H : D
    {
        std::cout << std::endl;
        tablaTipos.print();
        std::cout << std::endl;
        std::cout << "Codigo intermedio TAC:" << std::endl;
        CodeGen::print(std::cout);
        std::cout << std::endl;
        std::cout << "Tabla de simbolos global:" << std::endl;
        pilaTs.top()->print();
    }
    H
  | /* ε */
    {
        // Producción vacía, no hacer nada
    }
  ;

// D → T L ;
//    | struct id { D }; 
//    | def T id ( F ) { D R } 
D : T L 
    {
        $2.tipo = $1.tipo;
        currentType = $2.tipo;
    }
    SEMICOLON
    | STRUCT ID 
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
    LBRACE D RBRACE SEMICOLON
    {
        std::string id = $2;
        SymTab* ts = pilaTs.pop();
        int tipoStruct = tablaTipos.addStructType(dir, ts);
        pilaTs.bottom()->addSym(id, -1, tipoStruct, "struct");
        dir = pilaOffset.top();
        pilaOffset.pop();
        if ($2) free($2);
    }
  ;

// T → B A
T : B A
    {
        $$.tipo = $2.tipo;
        currentType = $$.tipo;
    }
  ;

// B → int
//    | float
//    | bool 
//    | char 
//    | id
B : INT
    {
        $$.tipo = tablaTipos.getId("int");
        $$.base = $$.tipo;
        currentType = $$.tipo;
    }

  | FLOAT
    {
        $$.tipo = tablaTipos.getId("float");
        $$.base = $$.tipo;
        currentType = $$.tipo;
    }

  | BOOL
    {
        $$.tipo = tablaTipos.getId("bool");
        $$.base = $$.tipo;
        currentType = $$.tipo;
    }

  | CHAR
    {
        $$.tipo = tablaTipos.getId("char");
        $$.base = $$.tipo;
        currentType = $$.tipo;
    }
  | ID
    {
        std::string id = $1;
        int idStruct = tablaTipos.getId(id); 
        if (idStruct != -1) {
            $$.tipo = idStruct;
            $$.base = idStruct;
            currentType = idStruct;
        } else {
            std::cerr << "Error: Struct no declarado" << std::endl;
        }
        if ($1) free($1);
    }
  ;

// A → [ INTLIT ] A₁
A : LBRACKET INTLIT RBRACKET A
    {
        if ($2 <= 0) {
          cerr << "Error: El índice debe ser mayor a cero" << endl;
          $$.tipo = currentType; 
        } else {
          $$.tipo = tablaTipos.addArrayType($2, $4.tipo);
        }
        $$.base = currentType;
    }
  // A → ε
  | /* ε */
    {
        $$.tipo = currentType;
        $$.base = currentType;
    }
  ;

// L → L , id
L : L COMMA ID
    {
      string id = $3;
      if (!pilaTs.top()->existe(id)) {
        pilaTs.top()->addSym(id, dir, currentType, "var");
        dir += tablaTipos.getTam(currentType);
      } else {
        cerr << "Error: La variable " << id << " ya fue declarada" << endl;
      }
      if ($3) free($3);
    }
  // L → id
  | ID
    {
        string id = $1;
        if (!pilaTs.top()->existe(id)) {
          pilaTs.top()->addSym(id, dir, currentType, "var");
          dir += tablaTipos.getTam(currentType);
        } else {
          cerr << "Error: La variable " << id << " ya fue declarada" << endl;
        }
        if ($1) free($1);
    }
  ;

// F → G 
//    | ε

// G → G , T id
// G → T id
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

// R → S R 
//    | ε
R : S R 
    | {

    }
    ;

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
            std::string res = std::string($1.baseStr) + "[" + std::to_string($1.tam) + "]"; // Usamos tam temporalmente como offset de memoria
            CodeGen::emit("=", a1, "", res);
        } else {
            std::cerr << "Error: Tipos incompatibles en struct" << std::endl;
        }
    }
  ;

// S → break ; 
//    | return E ; 
//    | id = E ; 
//    | C = E ; 
//    | Z = E ; 
//    | if ( E ) S  
//    | if ( E ) S1 else S2
//    | while ( E ) S
//    | for (S1 ; E; S2 ) S3
S : S_ASIG SEMICOLON


// E → E1 + E2
//    | E1 - E2
//    | E1 * E2
//    | E1 / E2
//    | E1 % E2
//    | E1 || E2
//    | E1 && E2
//    | E1 > E2
//    | E1 < E2
//    | E1 >= E2
//    | E1 <= E2
//    | E1 != E2
//    | E1 == E2
//    | C
//    | id
//    | id ( N )
//    | Z 
//    | intlit
//    | floatlit  
//    | true
//    | false
//    | charlit
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
    ;

// N → M
//    | ε
N :

// M → M , E
//    | E
M :

// C → id [ E ]
//    | C [ E ]
C : 

// Z → Z . id
//    | id 
Z : 



%%

// Manejo de errores de Bison
void C1::Parser::error(const std::string& msg) {
    std::cerr << "Error de sintaxis: " << msg << std::endl;
}
