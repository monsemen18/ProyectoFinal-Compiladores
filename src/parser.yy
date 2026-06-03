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
    std::stack<int> pilaBreak;        // Pila break
    std::stack<int> pilaReturn;       // Pila return para etiquetas o lugares para retornar
    std::stack<int> pilaTrue;         // Pila true. Etiquetas para rama true
    std::stack<int> pilaFalse;        // Pila false. Etiquetas para rama false
    std::stack<int> pilaDir;          // Pila next. Etiquetas para siguiente iteracion
    int dir = 0;                      // Dirección actual de memoria

    // Variable auxiliar para propagar L.tipo (atributo heredado)
    int currentType = 0;

    // Variable para el tipo de retorno de funciones
    int tipoReturnFunc = 0;

    // Lista de tipos de parámetros (para F.lista)
    std::vector<int> listaParams;
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
    } attr;                // Para T, B, A
}

// ========================================================
// Tokens (terminales)
// ========================================================
%token <sval> ID
%token <ival> INTLIT
%token <sval> FLOATLIT CHARLIT
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
%type <attr> P, H, D, T, B, A, L, F, G, R, S, E, N, M, C, Z

// ========================================================
// Símbolo inicial
// ========================================================
%start P

%%

// ============================================================
// Acciones semánticas
// ============================================================

// P → H
P : {
        dir = 0;
        pilaTs.push(new SymTab());
  CodeGen::reset();
    }
    H; 

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
D : 
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
    }
  // L → id
  | ID
    {
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
    }
  ;

// F → G 
//    | ε

// G → G , T id
// G → T id 

// R → S R 
//    | ε

// S → break ; 
//    | return E ; 
//    | id = E ; 
//    | C = E ; 
//    | Z = E ; 
//    | if ( E ) S  
//    | if ( E ) S1 else S2
//    | while ( E ) S
//    | for (S1 ; E; S2 ) S3


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

// N → M
//    | ε

// M → M , E
//    | E

// C → id [ E ]
//    | C [ E ]

// Z → Z . id
//    | id 



%%

// Manejo de errores de Bison
void C1::Parser::error(const std::string& msg) {
    std::cerr << "Error de sintaxis: " << msg << std::endl;
}
