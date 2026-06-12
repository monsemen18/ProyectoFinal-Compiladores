// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "Parser.hpp"


// Unqualified %code blocks.
#line 27 "src/parser.yy"

    #include "headers/Lexer.hpp"

    // Definir yylex para que Bison llame a nuestro Lexer
    #define yylex(yylval) lexer->lex(yylval)

    extern int contador_errores; 

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

#line 119 "Parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 7 "src/parser.yy"
namespace C1 {
#line 193 "Parser.cpp"

  /// Build a parser object.
  Parser::Parser (Lexer* lexer_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (value_type) v)
    : Base (t)
    , value (YY_MOVE (v))
  {}


  template <typename Base>
  Parser::symbol_kind_type
  Parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
  }

  // by_kind.
  Parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  Parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  Parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  Parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  Parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  Parser::symbol_kind_type
  Parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  Parser::symbol_kind_type
  Parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_kind_type
  Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value))
  {
    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YY_USE (yysym.kind ());
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;


      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // $@1: %empty
#line 156 "src/parser.yy"
    {
        dir = 0;
        pilaTs.push(new SymTab());
        CodeGen::reset();
    }
#line 654 "Parser.cpp"
    break;

  case 3: // P: $@1 H
#line 162 "src/parser.yy"
    {
        std::cout << "\n========== RESULTADOS DEL COMPILADOR ==========\n";
        tablaTipos.print();
        std::cout << "\n";
        pilaTs.bottom()->print();
        std::cout << "\n========== CODIGO INTERMEDIO (TAC) ==========\n";
        CodeGen::print(std::cout);
        std::cout << "===============================================\n";
    }
#line 668 "Parser.cpp"
    break;

  case 4: // H: D H
#line 173 "src/parser.yy"
    { (yylhs.value.attr) = (yystack_[1].value.attr); }
#line 674 "Parser.cpp"
    break;

  case 5: // H: %empty
#line 173 "src/parser.yy"
                   { }
#line 680 "Parser.cpp"
    break;

  case 6: // D: T L SEMICOLON
#line 175 "src/parser.yy"
    { (yylhs.value.attr) = (yystack_[2].value.attr); }
#line 686 "Parser.cpp"
    break;

  case 7: // $@2: %empty
#line 177 "src/parser.yy"
    {
        std::string id = (yystack_[1].value.sval);
        if (!pilaTs.bottom()->existe(id)) {
            pilaTs.push(new SymTab());
            pilaOffset.push(dir);
            dir = 0;
        } else {
            std::cerr << "Error Semantico: Estructura duplicada" << std::endl;
            contador_errores++;
        }
    }
#line 702 "Parser.cpp"
    break;

  case 8: // D: STRUCT ID LBRACE $@2 H RBRACE SEMICOLON
#line 189 "src/parser.yy"
    {
        std::string id = (yystack_[5].value.sval);
        SymTab* ts = pilaTs.pop();
        int tipoStruct = tablaTipos.addStructType(dir, ts);
        pilaTs.bottom()->addSym(id, -1, tipoStruct, "struct");
        dir = pilaOffset.top();
        pilaOffset.pop();
        if ((yystack_[5].value.sval)) free((yystack_[5].value.sval));
    }
#line 716 "Parser.cpp"
    break;

  case 9: // $@3: %empty
#line 199 "src/parser.yy"
    {
        std::string id = (yystack_[1].value.sval);
        if (!pilaTs.bottom()->existe(id)) {
            pilaTs.push(new SymTab());
            pilaOffset.push(dir);
            dir = 0;
            tipoReturnFunc = (yystack_[2].value.attr).tipo;
            CodeGen::emit("label", "", "", id);
            listaParams.clear();
        } else {
            std::cerr << "Error Semantico: Función duplicada" << std::endl;
            contador_errores++;
        }
    }
#line 735 "Parser.cpp"
    break;

  case 10: // D: DEF T ID LPAREN $@3 F RPAREN LBRACE H R RBRACE
#line 214 "src/parser.yy"
    {
        std::string id = (yystack_[8].value.sval);
        pilaTs.pop();
        pilaTs.bottom()->addSym(id, -1, (yystack_[9].value.attr).tipo, "func", listaParams);
        dir = pilaOffset.top();
        pilaOffset.pop();
        if ((yystack_[8].value.sval)) free((yystack_[8].value.sval));
    }
#line 748 "Parser.cpp"
    break;

  case 11: // T: B A
#line 225 "src/parser.yy"
    {
        (yylhs.value.attr).tipo = (yystack_[0].value.attr).tipo;
        currentType = (yylhs.value.attr).tipo;
    }
#line 757 "Parser.cpp"
    break;

  case 12: // B: INT
#line 231 "src/parser.yy"
          { (yylhs.value.attr).tipo = tablaTipos.getId("int");   (yylhs.value.attr).base = (yylhs.value.attr).tipo; currentType = (yylhs.value.attr).tipo; }
#line 763 "Parser.cpp"
    break;

  case 13: // B: FLOAT
#line 232 "src/parser.yy"
          { (yylhs.value.attr).tipo = tablaTipos.getId("float"); (yylhs.value.attr).base = (yylhs.value.attr).tipo; currentType = (yylhs.value.attr).tipo; }
#line 769 "Parser.cpp"
    break;

  case 14: // B: BOOL
#line 233 "src/parser.yy"
          { (yylhs.value.attr).tipo = tablaTipos.getId("bool");  (yylhs.value.attr).base = (yylhs.value.attr).tipo; currentType = (yylhs.value.attr).tipo; }
#line 775 "Parser.cpp"
    break;

  case 15: // B: CHAR
#line 234 "src/parser.yy"
          { (yylhs.value.attr).tipo = tablaTipos.getId("char");  (yylhs.value.attr).base = (yylhs.value.attr).tipo; currentType = (yylhs.value.attr).tipo; }
#line 781 "Parser.cpp"
    break;

  case 16: // B: STRUCT ID
#line 236 "src/parser.yy"
    {
        std::string id = (yystack_[0].value.sval);
        int idStruct = -1;
        
        if (pilaTs.bottom()->existe(id)) {
            idStruct = pilaTs.bottom()->getType(id);
        }

        if (idStruct != -1) {
            (yylhs.value.attr).tipo = idStruct;
            (yylhs.value.attr).base = idStruct;
            currentType = idStruct;
        } else {
            std::cerr << "Error: Struct no declarado '" << id << "'" << std::endl;
            contador_errores++;
            (yylhs.value.attr).tipo = 0; // 0 = int. Se pone un tipo seguro para evitar colapsar la memoria basura
            currentType = 0;
        }
        if ((yystack_[0].value.sval)) free((yystack_[0].value.sval));
    }
#line 806 "Parser.cpp"
    break;

  case 17: // A: LBRACKET INTLIT RBRACKET A
#line 259 "src/parser.yy"
    {
        if ((yystack_[2].value.ival) > 0) {
            (yylhs.value.attr).tipo = tablaTipos.addArrayType((yystack_[2].value.ival), (yystack_[0].value.attr).tipo);
        } else {
            std::cerr << "Error Semantico: El índice debe ser mayor a cero" << std::endl;
            contador_errores++;
            (yylhs.value.attr).tipo = currentType;
        }
        (yylhs.value.attr).base = currentType;
    }
#line 821 "Parser.cpp"
    break;

  case 18: // A: %empty
#line 270 "src/parser.yy"
    {
        (yylhs.value.attr).tipo = currentType;
        (yylhs.value.attr).base = currentType;
    }
#line 830 "Parser.cpp"
    break;

  case 19: // L: L COMMA ID
#line 277 "src/parser.yy"
    {
        std::string id = (yystack_[0].value.sval);
        if (!pilaTs.top()->existe(id)) {
            pilaTs.top()->addSym(id, dir, currentType, "var");
            dir += tablaTipos.getTam(currentType);
        } else {
            std::cerr << "Error Semantico: Variable duplicada" << std::endl;
            contador_errores++;
        }
        if ((yystack_[0].value.sval)) free((yystack_[0].value.sval));
    }
#line 846 "Parser.cpp"
    break;

  case 20: // L: ID
#line 289 "src/parser.yy"
    {
        std::string id = (yystack_[0].value.sval);
        if (!pilaTs.top()->existe(id)) {
            pilaTs.top()->addSym(id, dir, currentType, "var");
            dir += tablaTipos.getTam(currentType);
        } else {
            std::cerr << "Error Semantico: Variable duplicada" << std::endl;
            contador_errores++;
        }
        if ((yystack_[0].value.sval)) free((yystack_[0].value.sval));
    }
#line 862 "Parser.cpp"
    break;

  case 21: // F: G
#line 302 "src/parser.yy"
    { (yylhs.value.attr) = (yystack_[0].value.attr); }
#line 868 "Parser.cpp"
    break;

  case 22: // F: %empty
#line 302 "src/parser.yy"
                 { }
#line 874 "Parser.cpp"
    break;

  case 23: // G: G COMMA T ID
#line 305 "src/parser.yy"
    {
        std::string id = (yystack_[0].value.sval);
        if (!pilaTs.top()->existe(id)) {
            pilaTs.top()->addSym(id, dir, (yystack_[1].value.attr).tipo, "arg");
            dir += tablaTipos.getTam((yystack_[1].value.attr).tipo);
            listaParams.push_back((yystack_[1].value.attr).tipo);
        } else {
            std::cerr << "Error Semantico: Parámetro duplicado" << std::endl;
            contador_errores++;
        }
        if ((yystack_[0].value.sval)) free((yystack_[0].value.sval));
    }
#line 891 "Parser.cpp"
    break;

  case 24: // G: T ID
#line 318 "src/parser.yy"
    {
        std::string id = (yystack_[0].value.sval);
        if (!pilaTs.top()->existe(id)) {
            pilaTs.top()->addSym(id, dir, (yystack_[1].value.attr).tipo, "arg");
            dir += tablaTipos.getTam((yystack_[1].value.attr).tipo);
            listaParams.push_back((yystack_[1].value.attr).tipo);
        } else {
            std::cerr << "Error Semantico: Parámetro duplicado" << std::endl;
            contador_errores++;
        }
        if ((yystack_[0].value.sval)) free((yystack_[0].value.sval));
    }
#line 908 "Parser.cpp"
    break;

  case 25: // R: S R
#line 332 "src/parser.yy"
    { (yylhs.value.attr) = (yystack_[1].value.attr); }
#line 914 "Parser.cpp"
    break;

  case 26: // R: %empty
#line 332 "src/parser.yy"
                   { }
#line 920 "Parser.cpp"
    break;

  case 27: // S_ASIG: ID ASIGNACION E
#line 335 "src/parser.yy"
    {
        std::string id = (yystack_[2].value.sval);
        int tipoDest = -1;
        if (pilaTs.top()->existe(id)) tipoDest = pilaTs.top()->getType(id);
        else if (pilaTs.bottom()->existe(id)) tipoDest = pilaTs.bottom()->getType(id);
        else std::cerr << "Error Semantico: El id no fue declarado: " << id << std::endl;
        contador_errores++;

        if (tipoDest != -1 && compatibles(tipoDest, (yystack_[0].value.attr).tipo)) {
            std::string a1 = reducir((yystack_[0].value.attr).dir, (yystack_[0].value.attr).tipo, tipoDest);
            CodeGen::emit("=", a1, "", id);
        } else {
            std::cerr << "Error Semantico: Tipos incompatibles" << std::endl;
            contador_errores++;
        }
        if ((yystack_[2].value.sval)) free((yystack_[2].value.sval));
    }
#line 942 "Parser.cpp"
    break;

  case 28: // S_ASIG: C ASIGNACION E
#line 353 "src/parser.yy"
    {
        if (compatibles((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo)) {
            std::string a1 = reducir((yystack_[0].value.attr).dir, (yystack_[0].value.attr).tipo, (yystack_[2].value.attr).tipo);
            std::string res = std::string((yystack_[2].value.attr).baseStr) + "[" + (yystack_[2].value.attr).dir + "]";
            CodeGen::emit("=", a1, "", res);
        } else {
            std::cerr << "Error Semantico: Tipos incompatibles en arreglo" << std::endl;
            contador_errores++;
        }
    }
#line 957 "Parser.cpp"
    break;

  case 29: // S_ASIG: Z ASIGNACION E
#line 364 "src/parser.yy"
    {
        if (compatibles((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo)) {
            std::string a1 = reducir((yystack_[0].value.attr).dir, (yystack_[0].value.attr).tipo, (yystack_[2].value.attr).tipo);
            std::string res = std::string((yystack_[2].value.attr).baseStr) + "[" + std::to_string((yystack_[2].value.attr).tam) + "]"; 
            CodeGen::emit("=", a1, "", res);
        } else {
            std::cerr << "Error Semantico: Tipos incompatibles" << std::endl;
            contador_errores++;
        }
    }
#line 972 "Parser.cpp"
    break;

  case 30: // M_IF: IF LPAREN E RPAREN
#line 378 "src/parser.yy"
    {
        std::string lTrue = CodeGen::newLabel();
        std::string lFalse = CodeGen::newLabel();
        CodeGen::emit("if", (yystack_[1].value.attr).dir, "", lTrue);
        CodeGen::emit("goto", "", "", lFalse);
        CodeGen::emit("label", "", "", lTrue);
        pilaTrue.push(lTrue);
        pilaFalse.push(lFalse);
        memset(&(yylhs.value.attr), 0, sizeof((yylhs.value.attr))); // Limpiamos para evitar warnings
    }
#line 987 "Parser.cpp"
    break;

  case 31: // S: BREAK SEMICOLON
#line 391 "src/parser.yy"
    {
        CodeGen::emit("goto", "", "", pilaBreak.top());
    }
#line 995 "Parser.cpp"
    break;

  case 32: // S: RETURN E SEMICOLON
#line 395 "src/parser.yy"
    {
        if (compatibles(tipoReturnFunc, (yystack_[1].value.attr).tipo)) {
            CodeGen::emit("return", "", "", (yystack_[1].value.attr).dir);
        } else {
            std::cerr << "Error Semantico: Tipo de retorno incorrecto" << std::endl;
            contador_errores++;
        }
    }
#line 1008 "Parser.cpp"
    break;

  case 33: // S: S_ASIG SEMICOLON
#line 403 "src/parser.yy"
    { (yylhs.value.attr) = (yystack_[1].value.attr); }
#line 1014 "Parser.cpp"
    break;

  case 34: // S: M_IF S
#line 405 "src/parser.yy"
    {
        std::string lFalse = pilaFalse.top();
        CodeGen::emit("label", "", "", lFalse);
        pilaTrue.pop();
        pilaFalse.pop();
    }
#line 1025 "Parser.cpp"
    break;

  case 35: // $@4: %empty
#line 412 "src/parser.yy"
    {
        std::string lNext = CodeGen::newLabel();
        std::string lFalse = pilaFalse.top();
        CodeGen::emit("goto", "", "", lNext);
        CodeGen::emit("label", "", "", lFalse);
        pilaDir.push(lNext); 
    }
#line 1037 "Parser.cpp"
    break;

  case 36: // S: M_IF S ELSE $@4 S
#line 420 "src/parser.yy"
    {
        std::string lNext = pilaDir.top();
        CodeGen::emit("label", "", "", lNext);
        pilaTrue.pop();
        pilaFalse.pop();
        pilaDir.pop();
    }
#line 1049 "Parser.cpp"
    break;

  case 37: // $@5: %empty
#line 428 "src/parser.yy"
    {
        std::string lNext = CodeGen::newLabel(); 
        CodeGen::emit("label", "", "", lNext);
        pilaDir.push(lNext); 
    }
#line 1059 "Parser.cpp"
    break;

  case 38: // $@6: %empty
#line 434 "src/parser.yy"
    {
        std::string lTrue = CodeGen::newLabel();
        std::string lFalse = CodeGen::newLabel();
        CodeGen::emit("if", (yystack_[1].value.attr).dir, "", lTrue);
        CodeGen::emit("goto", "", "", lFalse);
        CodeGen::emit("label", "", "", lTrue);
        pilaBreak.push(lFalse); 
        pilaTrue.push(lTrue);
        pilaFalse.push(lFalse);
    }
#line 1074 "Parser.cpp"
    break;

  case 39: // S: WHILE $@5 LPAREN E RPAREN $@6 S
#line 445 "src/parser.yy"
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
#line 1089 "Parser.cpp"
    break;

  case 40: // $@7: %empty
#line 456 "src/parser.yy"
    {
        std::string lNext = CodeGen::newLabel(); 
        CodeGen::emit("label", "", "", lNext);
        pilaDir.push(lNext);
    }
#line 1099 "Parser.cpp"
    break;

  case 41: // $@8: %empty
#line 462 "src/parser.yy"
    {
        std::string lTrue = CodeGen::newLabel();
        std::string lFalse = CodeGen::newLabel();
        std::string lInc = CodeGen::newLabel(); 
        CodeGen::emit("if", (yystack_[1].value.attr).dir, "", lTrue);
        CodeGen::emit("goto", "", "", lFalse);
        CodeGen::emit("label", "", "", lInc);
        
        pilaTrue.push(lTrue);
        pilaFalse.push(lFalse);
        pilaReturn.push(lInc); 
        pilaBreak.push(lFalse); 
    }
#line 1117 "Parser.cpp"
    break;

  case 42: // $@9: %empty
#line 476 "src/parser.yy"
    {
        std::string lNext = pilaDir.top();
        std::string lTrue = pilaTrue.top();
        CodeGen::emit("goto", "", "", lNext);
        CodeGen::emit("label", "", "", lTrue);
    }
#line 1128 "Parser.cpp"
    break;

  case 43: // S: FOR LPAREN S_ASIG SEMICOLON $@7 E SEMICOLON $@8 S_ASIG $@9 RPAREN S
#line 483 "src/parser.yy"
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
#line 1145 "Parser.cpp"
    break;

  case 44: // S: LBRACE R RBRACE
#line 496 "src/parser.yy"
    { memset(&(yylhs.value.attr), 0, sizeof((yylhs.value.attr))); }
#line 1151 "Parser.cpp"
    break;

  case 45: // E: E MAS E
#line 500 "src/parser.yy"
    {
        if (compatibles((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo)) {
            (yylhs.value.attr).tipo = maxTipo((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo);
            std::string t = CodeGen::newTemp();
            strcpy((yylhs.value.attr).dir, t.c_str());
            std::string a1 = ampliar((yystack_[2].value.attr).dir, (yystack_[2].value.attr).tipo, (yylhs.value.attr).tipo);
            std::string a2 = ampliar((yystack_[0].value.attr).dir, (yystack_[0].value.attr).tipo, (yylhs.value.attr).tipo);
            CodeGen::emit("+", a1, a2, (yylhs.value.attr).dir);
        } else {
            std::cerr << "Error Semantico: Tipos incompatibles" << std::endl;
            contador_errores++;
        }
    }
#line 1169 "Parser.cpp"
    break;

  case 46: // E: E MENOS E
#line 514 "src/parser.yy"
    {
        if (compatibles((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo)) {
            (yylhs.value.attr).tipo = maxTipo((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo);
            std::string t = CodeGen::newTemp();
            strcpy((yylhs.value.attr).dir, t.c_str());
            std::string a1 = ampliar((yystack_[2].value.attr).dir, (yystack_[2].value.attr).tipo, (yylhs.value.attr).tipo);
            std::string a2 = ampliar((yystack_[0].value.attr).dir, (yystack_[0].value.attr).tipo, (yylhs.value.attr).tipo);
            CodeGen::emit("-", a1, a2, (yylhs.value.attr).dir);
        } else {
            std::cerr << "Error Semantico: Tipos incompatibles" << std::endl;
            contador_errores++;
        }
    }
#line 1187 "Parser.cpp"
    break;

  case 47: // E: E MULT E
#line 528 "src/parser.yy"
    {
        if (compatibles((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo)) {
            (yylhs.value.attr).tipo = maxTipo((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo);
            std::string t = CodeGen::newTemp();
            strcpy((yylhs.value.attr).dir, t.c_str());
            std::string a1 = ampliar((yystack_[2].value.attr).dir, (yystack_[2].value.attr).tipo, (yylhs.value.attr).tipo);
            std::string a2 = ampliar((yystack_[0].value.attr).dir, (yystack_[0].value.attr).tipo, (yylhs.value.attr).tipo);
            CodeGen::emit("*", a1, a2, (yylhs.value.attr).dir);
        } else {
            std::cerr << "Error Semantico: Tipos incompatibles" << std::endl; 
            contador_errores++;
        }
    }
#line 1205 "Parser.cpp"
    break;

  case 48: // E: E DIV E
#line 542 "src/parser.yy"
    {
        if (compatibles((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo)) {
            (yylhs.value.attr).tipo = maxTipo((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo);
            std::string t = CodeGen::newTemp();
            strcpy((yylhs.value.attr).dir, t.c_str());
            std::string a1 = ampliar((yystack_[2].value.attr).dir, (yystack_[2].value.attr).tipo, (yylhs.value.attr).tipo);
            std::string a2 = ampliar((yystack_[0].value.attr).dir, (yystack_[0].value.attr).tipo, (yylhs.value.attr).tipo);
            CodeGen::emit("/", a1, a2, (yylhs.value.attr).dir);
        } else {
            std::cerr << "Error Semantico: Tipos incompatibles" << std::endl; 
            contador_errores++;
        }
    }
#line 1223 "Parser.cpp"
    break;

  case 49: // E: E MOD E
#line 556 "src/parser.yy"
    {
        if (tablaTipos.getName((yystack_[2].value.attr).tipo) == "int" && tablaTipos.getName((yystack_[0].value.attr).tipo) == "int") {
            (yylhs.value.attr).tipo = tablaTipos.getId("int");
            std::string t = CodeGen::newTemp();
            strcpy((yylhs.value.attr).dir, t.c_str());
            CodeGen::emit("%", (yystack_[2].value.attr).dir, (yystack_[0].value.attr).dir, (yylhs.value.attr).dir);
        } else {
            std::cerr << "Error Semantico: El operador módulo requiere enteros" << std::endl; 
            contador_errores++;
        }
    }
#line 1239 "Parser.cpp"
    break;

  case 50: // E: E OR E
#line 568 "src/parser.yy"
    {
        if (tablaTipos.getName((yystack_[2].value.attr).tipo) == "bool" && tablaTipos.getName((yystack_[0].value.attr).tipo) == "bool") {
            (yylhs.value.attr).tipo = tablaTipos.getId("bool");
            std::string t = CodeGen::newTemp();
            strcpy((yylhs.value.attr).dir, t.c_str());
            CodeGen::emit("||", (yystack_[2].value.attr).dir, (yystack_[0].value.attr).dir, (yylhs.value.attr).dir);
        } else {
            std::cerr << "Error Semantico: Tipos incompatibles" << std::endl; 
            contador_errores++;
        }
    }
#line 1255 "Parser.cpp"
    break;

  case 51: // E: E AND E
#line 580 "src/parser.yy"
    {
        if (tablaTipos.getName((yystack_[2].value.attr).tipo) == "bool" && tablaTipos.getName((yystack_[0].value.attr).tipo) == "bool") {
            (yylhs.value.attr).tipo = tablaTipos.getId("bool");
            std::string t = CodeGen::newTemp();
            strcpy((yylhs.value.attr).dir, t.c_str());
            CodeGen::emit("&&", (yystack_[2].value.attr).dir, (yystack_[0].value.attr).dir, (yylhs.value.attr).dir);
        } else {
            std::cerr << "Error Semantico: Tipos incompatibles" << std::endl; 
            contador_errores++;
        }
    }
#line 1271 "Parser.cpp"
    break;

  case 52: // E: E MAYOR_QUE E
#line 592 "src/parser.yy"
    {
        if (compatibles((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo)) {
            (yylhs.value.attr).tipo = tablaTipos.getId("bool");
            int maxT = maxTipo((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo);
            std::string t = CodeGen::newTemp();
            strcpy((yylhs.value.attr).dir, t.c_str());
            CodeGen::emit(">", ampliar((yystack_[2].value.attr).dir, (yystack_[2].value.attr).tipo, maxT), ampliar((yystack_[0].value.attr).dir, (yystack_[0].value.attr).tipo, maxT), (yylhs.value.attr).dir);
        } else { std::cerr << "Error Semantico: Tipos incompatibles" << std::endl; 
        contador_errores++; }
    }
#line 1286 "Parser.cpp"
    break;

  case 53: // E: E MENOR_QUE E
#line 603 "src/parser.yy"
    {
        if (compatibles((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo)) {
            (yylhs.value.attr).tipo = tablaTipos.getId("bool");
            int maxT = maxTipo((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo);
            std::string t = CodeGen::newTemp();
            strcpy((yylhs.value.attr).dir, t.c_str());
            CodeGen::emit("<", ampliar((yystack_[2].value.attr).dir, (yystack_[2].value.attr).tipo, maxT), ampliar((yystack_[0].value.attr).dir, (yystack_[0].value.attr).tipo, maxT), (yylhs.value.attr).dir);
        } else { std::cerr << "Error Semantico: Tipos incompatibles" << std::endl; 
        contador_errores++; }
    }
#line 1301 "Parser.cpp"
    break;

  case 54: // E: E MAYOR_IGUAL E
#line 614 "src/parser.yy"
    {
        if (compatibles((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo)) {
            (yylhs.value.attr).tipo = tablaTipos.getId("bool");
            int maxT = maxTipo((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo);
            std::string t = CodeGen::newTemp();
            strcpy((yylhs.value.attr).dir, t.c_str());
            CodeGen::emit(">=", ampliar((yystack_[2].value.attr).dir, (yystack_[2].value.attr).tipo, maxT), ampliar((yystack_[0].value.attr).dir, (yystack_[0].value.attr).tipo, maxT), (yylhs.value.attr).dir);
        } else { std::cerr << "Error Semantico: Tipos incompatibles" << std::endl; 
        contador_errores++; }
    }
#line 1316 "Parser.cpp"
    break;

  case 55: // E: E MENOR_IGUAL E
#line 625 "src/parser.yy"
    {
        if (compatibles((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo)) {
            (yylhs.value.attr).tipo = tablaTipos.getId("bool");
            int maxT = maxTipo((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo);
            std::string t = CodeGen::newTemp();
            strcpy((yylhs.value.attr).dir, t.c_str());
            CodeGen::emit("<=", ampliar((yystack_[2].value.attr).dir, (yystack_[2].value.attr).tipo, maxT), ampliar((yystack_[0].value.attr).dir, (yystack_[0].value.attr).tipo, maxT), (yylhs.value.attr).dir);
        } else { std::cerr << "Error Semantico: Tipos incompatibles" << std::endl; contador_errores++; }
    }
#line 1330 "Parser.cpp"
    break;

  case 56: // E: E IGUAL E
#line 635 "src/parser.yy"
    {
        if (compatibles((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo)) {
            (yylhs.value.attr).tipo = tablaTipos.getId("bool");
            int maxT = maxTipo((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo);
            std::string t = CodeGen::newTemp();
            strcpy((yylhs.value.attr).dir, t.c_str());
            CodeGen::emit("==", ampliar((yystack_[2].value.attr).dir, (yystack_[2].value.attr).tipo, maxT), ampliar((yystack_[0].value.attr).dir, (yystack_[0].value.attr).tipo, maxT), (yylhs.value.attr).dir);
        } else { std::cerr << "Error Semantico: Tipos incompatibles" << std::endl; 
                contador_errores++; }
    }
#line 1345 "Parser.cpp"
    break;

  case 57: // E: E NO_IGUAL E
#line 646 "src/parser.yy"
    {
        if (compatibles((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo)) {
            (yylhs.value.attr).tipo = tablaTipos.getId("bool");
            int maxT = maxTipo((yystack_[2].value.attr).tipo, (yystack_[0].value.attr).tipo);
            std::string t = CodeGen::newTemp();
            strcpy((yylhs.value.attr).dir, t.c_str());
            CodeGen::emit("!=", ampliar((yystack_[2].value.attr).dir, (yystack_[2].value.attr).tipo, maxT), ampliar((yystack_[0].value.attr).dir, (yystack_[0].value.attr).tipo, maxT), (yylhs.value.attr).dir);
        } else { std::cerr << "Error Semantico: Tipos incompatibles" << std::endl;
                contador_errores++; }
    }
#line 1360 "Parser.cpp"
    break;

  case 58: // E: LPAREN E RPAREN
#line 657 "src/parser.yy"
    {
        (yylhs.value.attr).tipo = (yystack_[1].value.attr).tipo;
        (yylhs.value.attr).tam = (yystack_[1].value.attr).tam;
        strcpy((yylhs.value.attr).dir, (yystack_[1].value.attr).dir);
        strcpy((yylhs.value.attr).baseStr, (yystack_[1].value.attr).baseStr);
    }
#line 1371 "Parser.cpp"
    break;

  case 59: // E: C
#line 664 "src/parser.yy"
    {
        (yylhs.value.attr).tipo = (yystack_[0].value.attr).tipo;
        std::string t = CodeGen::newTemp();
        strcpy((yylhs.value.attr).dir, t.c_str());
        std::string rhs = std::string((yystack_[0].value.attr).baseStr) + "[" + (yystack_[0].value.attr).dir + "]";
        CodeGen::emit("=", rhs, "", (yylhs.value.attr).dir);
    }
#line 1383 "Parser.cpp"
    break;

  case 60: // E: Z
#line 672 "src/parser.yy"
    {
        (yylhs.value.attr).tipo = (yystack_[0].value.attr).tipo;
        std::string t = CodeGen::newTemp();
        strcpy((yylhs.value.attr).dir, t.c_str());
        std::string rhs = std::string((yystack_[0].value.attr).baseStr) + "[" + std::to_string((yystack_[0].value.attr).tam) + "]";
        CodeGen::emit("=", rhs, "", (yylhs.value.attr).dir);
    }
#line 1395 "Parser.cpp"
    break;

  case 61: // E: ID
#line 680 "src/parser.yy"
    {
        std::string id = (yystack_[0].value.sval);
        if (pilaTs.top()->existe(id)) {
            (yylhs.value.attr).tipo = pilaTs.top()->getType(id);
            strcpy((yylhs.value.attr).dir, id.c_str());
        } else if (pilaTs.bottom()->existe(id)) {
            (yylhs.value.attr).tipo = pilaTs.bottom()->getType(id);
            strcpy((yylhs.value.attr).dir, id.c_str());
        } else {
            std::cerr << "Error Semantico: El id no fue declarado: " << id << std::endl;
            contador_errores++;
        }
        if ((yystack_[0].value.sval)) free((yystack_[0].value.sval));
    }
#line 1414 "Parser.cpp"
    break;

  case 62: // E: ID LPAREN N RPAREN
#line 695 "src/parser.yy"
    {
        std::string id = (yystack_[3].value.sval);
        if (pilaTs.bottom()->existe(id) && pilaTs.bottom()->getCat(id) == "func") {
            std::vector<int> paramsEsperados = pilaTs.bottom()->getArgs(id);
            
            // Comparamos si lo que mandó el usuario (argsList) coincide con lo que pide la función
            if (paramsEsperados == argsList) {
                (yylhs.value.attr).tipo = pilaTs.bottom()->getType(id);
                std::string t = CodeGen::newTemp();
                strcpy((yylhs.value.attr).dir, t.c_str());
                CodeGen::emit("call", id, std::to_string(argsList.size()), (yylhs.value.attr).dir);
            } else {
                std::cerr << "Error Semantico: La lista de parametros no coincide con la lista de argumentos en la funcion '" << id << "'" << std::endl;
                contador_errores++;
                (yylhs.value.attr).tipo = 0;
            }

        } else {
            std::cerr << "Error Semantico: El identificador '" << id << "' no es una funcion declarada" << std::endl;
            contador_errores++;
            (yylhs.value.attr).tipo = 0;
        }
        if ((yystack_[3].value.sval)) free((yystack_[3].value.sval));
        argsList.clear(); 
    }
#line 1444 "Parser.cpp"
    break;

  case 63: // E: INTLIT
#line 721 "src/parser.yy"
    {
        (yylhs.value.attr).tipo = tablaTipos.getId("int");
        strcpy((yylhs.value.attr).dir, std::to_string((yystack_[0].value.ival)).c_str());
    }
#line 1453 "Parser.cpp"
    break;

  case 64: // E: FLOATLIT
#line 726 "src/parser.yy"
    {
        (yylhs.value.attr).tipo = tablaTipos.getId("float");
        strcpy((yylhs.value.attr).dir, (yystack_[0].value.sval));
        if ((yystack_[0].value.sval)) free((yystack_[0].value.sval));
    }
#line 1463 "Parser.cpp"
    break;

  case 65: // E: TRUE
#line 732 "src/parser.yy"
    {
        (yylhs.value.attr).tipo = tablaTipos.getId("bool");
        strcpy((yylhs.value.attr).dir, "1");
    }
#line 1472 "Parser.cpp"
    break;

  case 66: // E: FALSE
#line 737 "src/parser.yy"
    {
        (yylhs.value.attr).tipo = tablaTipos.getId("bool");
        strcpy((yylhs.value.attr).dir, "0");
    }
#line 1481 "Parser.cpp"
    break;

  case 67: // E: CHARLIT
#line 742 "src/parser.yy"
    {
        (yylhs.value.attr).tipo = tablaTipos.getId("char");
        strcpy((yylhs.value.attr).dir, (yystack_[0].value.sval));
        if ((yystack_[0].value.sval)) free((yystack_[0].value.sval));
    }
#line 1491 "Parser.cpp"
    break;

  case 68: // N: M
#line 749 "src/parser.yy"
    { (yylhs.value.attr) = (yystack_[0].value.attr); }
#line 1497 "Parser.cpp"
    break;

  case 69: // N: %empty
#line 749 "src/parser.yy"
                 { }
#line 1503 "Parser.cpp"
    break;

  case 70: // M: M COMMA E
#line 752 "src/parser.yy"
    {
        argsList.push_back((yystack_[0].value.attr).tipo);
        CodeGen::emit("param", (yystack_[0].value.attr).dir);
    }
#line 1512 "Parser.cpp"
    break;

  case 71: // M: E
#line 757 "src/parser.yy"
    {
        argsList.push_back((yystack_[0].value.attr).tipo);
        CodeGen::emit("param", (yystack_[0].value.attr).dir);
    }
#line 1521 "Parser.cpp"
    break;

  case 72: // C: ID LBRACKET E RBRACKET
#line 764 "src/parser.yy"
    {
        std::string id = (yystack_[3].value.sval);
        int tipoArray = -1;
        if (pilaTs.top()->existe(id)) tipoArray = pilaTs.top()->getType(id);
        else if (pilaTs.bottom()->existe(id)) tipoArray = pilaTs.bottom()->getType(id);
        
        if (tablaTipos.getName(tipoArray) == "array" && tablaTipos.getName((yystack_[1].value.attr).tipo) == "int") {
            strcpy((yylhs.value.attr).baseStr, id.c_str());
            (yylhs.value.attr).tipo = tablaTipos.getTipoBase(tipoArray);
            (yylhs.value.attr).tam = tablaTipos.getTam((yylhs.value.attr).tipo);
            std::string t = CodeGen::newTemp();
            strcpy((yylhs.value.attr).dir, t.c_str());
            CodeGen::emit("*", (yystack_[1].value.attr).dir, std::to_string((yylhs.value.attr).tam), t);
        }
        if ((yystack_[3].value.sval)) free((yystack_[3].value.sval));
    }
#line 1542 "Parser.cpp"
    break;

  case 73: // C: C LBRACKET E RBRACKET
#line 781 "src/parser.yy"
    {
        if (tablaTipos.getName((yystack_[3].value.attr).tipo) == "array" && tablaTipos.getName((yystack_[1].value.attr).tipo) == "int") {
            strcpy((yylhs.value.attr).baseStr, (yystack_[3].value.attr).baseStr);
            (yylhs.value.attr).tipo = tablaTipos.getTipoBase((yystack_[3].value.attr).tipo);
            (yylhs.value.attr).tam = tablaTipos.getTam((yylhs.value.attr).tipo);
            std::string t = CodeGen::newTemp();
            std::string dirDest = CodeGen::newTemp();
            CodeGen::emit("*", (yystack_[1].value.attr).dir, std::to_string((yylhs.value.attr).tam), t);
            CodeGen::emit("+", (yystack_[3].value.attr).dir, t, dirDest);
            strcpy((yylhs.value.attr).dir, dirDest.c_str());
        }
    }
#line 1559 "Parser.cpp"
    break;

  case 74: // Z: ID DOT ID
#line 796 "src/parser.yy"
    {
        std::string id = (yystack_[2].value.sval);
        int tipoBase = -1;
        if (pilaTs.top()->existe(id)) tipoBase = pilaTs.top()->getType(id);
        else if (pilaTs.bottom()->existe(id)) tipoBase = pilaTs.bottom()->getType(id);
        
        strcpy((yylhs.value.attr).baseStr, id.c_str());
        (yylhs.value.attr).tam = 0;
        (yylhs.value.attr).tipo = 0; 
        
        if (tipoBase != -1) {
            SymTab* ts = tablaTipos.getTS(tipoBase);
            if (ts && ts->existe((yystack_[0].value.sval))) {
                (yylhs.value.attr).tipo = ts->getType((yystack_[0].value.sval));
                (yylhs.value.attr).tam = ts->getDir((yystack_[0].value.sval));
            } else {
                std::cerr << "Error Semantico: El campo '" << (yystack_[0].value.sval) << "' no existe en el struct" << std::endl;
                contador_errores++;
            }
        } else {
            std::cerr << "Error Semantico: Variable '" << id << "' no declarada" << std::endl;
            contador_errores++;
        }
        if ((yystack_[2].value.sval)) free((yystack_[2].value.sval));
        if ((yystack_[0].value.sval)) free((yystack_[0].value.sval));
    }
#line 1590 "Parser.cpp"
    break;

  case 75: // Z: Z DOT ID
#line 823 "src/parser.yy"
    {
        SymTab* ts = tablaTipos.getTS((yystack_[2].value.attr).tipo);
        strcpy((yylhs.value.attr).baseStr, (yystack_[2].value.attr).baseStr);
        (yylhs.value.attr).tam = (yystack_[2].value.attr).tam; 
        (yylhs.value.attr).tipo = 0;    
        
        if (ts && ts->existe((yystack_[0].value.sval))) {
            (yylhs.value.attr).tipo = ts->getType((yystack_[0].value.sval));
            (yylhs.value.attr).tam = (yystack_[2].value.attr).tam + ts->getDir((yystack_[0].value.sval)); 
        } else {
            std::cerr << "Error Semantico: El campo '" << (yystack_[0].value.sval) << "' no existe" << std::endl;
            contador_errores++;
        }
        if ((yystack_[0].value.sval)) free((yystack_[0].value.sval));
    }
#line 1610 "Parser.cpp"
    break;


#line 1614 "Parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const signed char Parser::yypact_ninf_ = -74;

  const signed char Parser::yytable_ninf_ = -1;

  const short
  Parser::yypact_[] =
  {
     -74,    27,   116,   -74,   -74,   -74,   -74,   -74,    31,   158,
     -74,   116,    37,    19,    53,    80,    82,   -74,   -74,     3,
      75,   -74,   -74,   -74,    59,   -74,    83,    64,   116,   -74,
     -74,    19,    66,   158,   -74,    70,    94,    72,    81,   -74,
     -74,   103,   158,   116,   105,    58,   -74,    40,    85,    16,
     102,   -74,   118,    58,   104,   126,    58,    58,    -1,   -14,
     145,    16,    16,   -74,    54,   -74,   -74,   -74,   -74,   -74,
      16,    78,   125,   127,    16,   156,   196,   187,   -74,   -74,
     230,   -74,    16,    16,   256,    16,   -74,   120,   197,    16,
     154,   -74,    16,    16,    16,    16,    16,    16,    16,    16,
      16,    16,    16,    16,    16,   169,    16,   239,   -74,   -74,
     139,   197,   -74,   197,   -74,   197,   243,   251,   -74,   -28,
     -28,   -74,   -74,   -74,   210,   223,    34,    34,    34,    34,
     236,   236,   -74,   184,   -74,    58,   -74,   -74,    16,   -74,
      16,   -74,   197,    58,   101,   -74,   -74,   196,   -74,   249,
      58,   -74
  };

  const signed char
  Parser::yydefact_[] =
  {
       2,     0,     5,     1,    12,    13,    14,    15,     0,     0,
       3,     5,     0,    18,    16,     0,     0,     4,    20,     0,
       0,    11,     7,    16,     0,     6,     0,     0,     5,     9,
      19,    18,     0,    22,    17,     0,     0,     0,    21,     8,
      24,     0,     0,     5,     0,    26,    23,     0,     0,     0,
       0,    37,     0,    26,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    31,    61,    64,    67,    63,    65,    66,
       0,     0,    59,    60,     0,     0,     0,     0,    10,    33,
      34,    25,     0,     0,     0,     0,    74,     0,    27,    69,
       0,    32,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    44,    35,
       0,    28,    75,    29,    72,    71,     0,    68,    58,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      57,    56,    30,     0,    40,     0,    73,    62,     0,    38,
       0,    36,    70,     0,     0,    39,    41,     0,    42,     0,
       0,    43
  };

  const short
  Parser::yypgoto_[] =
  {
     -74,   -74,   -74,     4,   -74,   -74,   -74,    -5,   -74,   248,
     -74,   -74,   -74,   -11,   -73,   -74,   -55,   -74,   -74,   -74,
     -74,   -74,   -74,   -44,   -74,   -74,   -45,   -43
  };

  const unsigned char
  Parser::yydefgoto_[] =
  {
       0,     1,     2,    10,    11,    28,    33,    12,    13,    21,
      19,    37,    38,    54,    55,    56,    57,   135,    75,   143,
     140,   147,   149,    71,   116,   117,    72,    73
  };

  const unsigned char
  Parser::yytable_[] =
  {
      58,    80,    59,   107,    16,    94,    95,    96,    58,    84,
      59,    58,    58,    59,    59,    17,    85,    87,    88,    64,
      65,    66,    67,    82,    25,    26,    90,     3,    36,    83,
     105,    58,    32,    59,    14,    68,    69,    44,   110,   111,
      18,   113,    77,    20,    70,   115,    81,    45,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,    47,   133,    60,    61,    92,    93,    94,    95,    96,
      62,    48,    49,    50,   148,    51,    52,    60,    61,    22,
     141,    27,    89,    23,    53,    24,    30,    29,   145,    31,
      58,    39,    59,    35,   142,   151,   144,    40,    58,    91,
      59,    41,    58,    42,    59,    58,    63,    59,    46,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   146,     4,     5,     6,     7,     8,     9,    43,
      74,    78,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   114,    76,    79,    86,    82,
      84,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   136,     4,     5,     6,     7,    15,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   118,   106,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   132,    47,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   139,   108,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,    92,    93,    94,    95,    96,   109,    98,    99,   100,
     101,   102,   103,   104,    92,    93,    94,    95,    96,   112,
     134,    99,   100,   101,   102,   103,   104,    92,    93,    94,
      95,    96,   137,   138,    99,   100,   101,   102,   150,    34
  };

  const unsigned char
  Parser::yycheck_[] =
  {
      45,    56,    45,    76,     9,    33,    34,    35,    53,    23,
      53,    56,    57,    56,    57,    11,    30,    61,    62,     3,
       4,     5,     6,    24,    21,    22,    70,     0,    33,    30,
      74,    76,    28,    76,     3,    19,    20,    42,    82,    83,
       3,    85,    53,    24,    28,    89,    57,    43,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,     3,   106,    23,    24,    31,    32,    33,    34,    35,
      30,    13,    14,    15,   147,    17,    18,    23,    24,    26,
     135,     6,    28,     3,    26,     3,     3,    28,   143,    25,
     135,    21,   135,    27,   138,   150,   140,     3,   143,    21,
     143,    29,   147,    22,   147,   150,    21,   150,     3,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    21,     7,     8,     9,    10,    11,    12,    26,
      28,    27,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    25,    28,    21,     3,    24,
      23,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    25,     7,     8,     9,    10,    11,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    29,    28,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    29,     3,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    29,    27,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    31,    32,    33,    34,    35,    16,    37,    38,    39,
      40,    41,    42,    43,    31,    32,    33,    34,    35,     3,
      21,    38,    39,    40,    41,    42,    43,    31,    32,    33,
      34,    35,    29,    22,    38,    39,    40,    41,    29,    31
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    45,    46,     0,     7,     8,     9,    10,    11,    12,
      47,    48,    51,    52,     3,    11,    51,    47,     3,    54,
      24,    53,    26,     3,     3,    21,    22,     6,    49,    28,
       3,    25,    47,    50,    53,    27,    51,    55,    56,    21,
       3,    29,    22,    26,    51,    47,     3,     3,    13,    14,
      15,    17,    18,    26,    57,    58,    59,    60,    70,    71,
      23,    24,    30,    21,     3,     4,     5,     6,    19,    20,
      28,    67,    70,    71,    28,    62,    28,    57,    27,    21,
      60,    57,    24,    30,    23,    30,     3,    67,    67,    28,
      67,    21,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    67,    28,    58,    27,    16,
      67,    67,     3,    67,    25,    67,    68,    69,    29,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    29,    67,    21,    61,    25,    29,    22,    29,
      64,    60,    67,    63,    67,    60,    21,    65,    58,    66,
      29,    60
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    44,    46,    45,    47,    47,    48,    49,    48,    50,
      48,    51,    52,    52,    52,    52,    52,    53,    53,    54,
      54,    55,    55,    56,    56,    57,    57,    58,    58,    58,
      59,    60,    60,    60,    60,    61,    60,    62,    63,    60,
      64,    65,    66,    60,    60,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    68,    68,
      69,    69,    70,    70,    71,    71
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     0,     2,     2,     0,     3,     0,     7,     0,
      11,     2,     1,     1,     1,     1,     2,     4,     0,     3,
       1,     1,     0,     4,     2,     2,     0,     3,     3,     3,
       4,     2,     3,     2,     2,     0,     5,     0,     0,     7,
       0,     0,     0,    12,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     4,     1,     1,     1,     1,     1,     1,     0,
       3,     1,     4,     4,     3,     3
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "ID", "FLOATLIT",
  "CHARLIT", "INTLIT", "INT", "FLOAT", "BOOL", "CHAR", "STRUCT", "DEF",
  "BREAK", "RETURN", "IF", "ELSE", "WHILE", "FOR", "TRUE", "FALSE",
  "SEMICOLON", "COMMA", "DOT", "LBRACKET", "RBRACKET", "LBRACE", "RBRACE",
  "LPAREN", "RPAREN", "ASIGNACION", "MAS", "MENOS", "MULT", "DIV", "MOD",
  "OR", "AND", "MAYOR_QUE", "MENOR_QUE", "MAYOR_IGUAL", "MENOR_IGUAL",
  "NO_IGUAL", "IGUAL", "$accept", "P", "$@1", "H", "D", "$@2", "$@3", "T",
  "B", "A", "L", "F", "G", "R", "S_ASIG", "M_IF", "S", "$@4", "$@5", "$@6",
  "$@7", "$@8", "$@9", "E", "N", "M", "C", "Z", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   156,   156,   156,   173,   173,   175,   177,   176,   199,
     198,   224,   231,   232,   233,   234,   235,   258,   270,   276,
     288,   302,   302,   304,   317,   332,   332,   334,   352,   363,
     377,   390,   394,   403,   404,   412,   411,   428,   434,   427,
     456,   462,   476,   455,   495,   499,   513,   527,   541,   555,
     567,   579,   591,   602,   613,   624,   634,   645,   656,   663,
     671,   679,   694,   720,   725,   731,   736,   741,   749,   749,
     751,   756,   763,   780,   795,   822
  };

  void
  Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Parser::symbol_kind_type
  Parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43
    };
    // Last valid token kind.
    const int code_max = 298;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 7 "src/parser.yy"
} // C1
#line 2092 "Parser.cpp"

#line 840 "src/parser.yy"


void C1::Parser::error(const std::string& msg) {
    std::cerr << "Error de sintaxis: " << msg << std::endl;
    contador_errores++;
}
