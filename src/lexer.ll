%{
    #include "Parser.hpp"
    #include "headers/Lexer.hpp"

    #define YY_DECL int C1::Lexer::lex(C1::Parser::semantic_type *yylval)
    
    extern int contador_errores;
%}

%option c++ noyywrap outfile="Lexer.cpp" yyclass="Lexer"

DIG     [0-9]
LETTER  [a-zA-Z_]
ID      {LETTER}({LETTER}|{DIG})*
intlit  {DIG}+
floatlit {DIG}+\.{DIG}+
charlit  \'([^\\\n]|\\.)\'

WSPC    [ \t\r]+

%%

"def"       { return Parser::token::DEF; }
"int"       { return Parser::token::INT; }
"float"     { return Parser::token::FLOAT; }
"bool"      { return Parser::token::BOOL; }
"char"      { return Parser::token::CHAR; }
"struct"    { return Parser::token::STRUCT; }
"break"     { return Parser::token::BREAK; }
"return"    { return Parser::token::RETURN; }
"if"        { return Parser::token::IF; }
"else"      { return Parser::token::ELSE; }
"while"     { return Parser::token::WHILE; }
"for"       { return Parser::token::FOR; }

";"         { return Parser::token::SEMICOLON; }
","         { return Parser::token::COMMA; }
"["         { return Parser::token::LBRACKET; }
"]"         { return Parser::token::RBRACKET; }
"{"         { return Parser::token::LBRACE; }
"}"         { return Parser::token::RBRACE; }
"("         { return Parser::token::LPAREN; }
")"         { return Parser::token::RPAREN; }
"."         { return Parser::token::DOT; }

"="         { return Parser::token::ASIGNACION; }
"+"         { return Parser::token::MAS; }
"-"         { return Parser::token::MENOS; }
"*"         { return Parser::token::MULT; }
"/"         { return Parser::token::DIV; }
"%"         { return Parser::token::MOD; }

"||"        { return Parser::token::OR; }
"&&"        { return Parser::token::AND; }
">"         { return Parser::token::MAYOR_QUE; }
"<"         { return Parser::token::MENOR_QUE; }
">="        { return Parser::token::MAYOR_IGUAL; }
"<="        { return Parser::token::MENOR_IGUAL; }
"!="        { return Parser::token::NO_IGUAL; }
"=="        { return Parser::token::IGUAL; }

"true"         { return Parser::token::TRUE; }
"false"         { return Parser::token::FALSE; }

{intlit}       {
                yylval->ival = atoi(yytext);
                return Parser::token::INTLIT;
            }

{floatlit}     {
                yylval->sval = strdup(yytext);
                return Parser::token::FLOATLIT;
            }

{charlit}      {
                yylval->sval = strdup(yytext);
                return Parser::token::CHARLIT;
            }

{ID}        {
                yylval->sval = strdup(yytext);
                return Parser::token::ID;
            }

"//".* { /* Ignora los comentarios de una sola linea */ }
{WSPC}      { /* Ignorar espacios en blanco */ }
"\n"        { line++; }
.           { std::cerr << "Error léxico: carácter inesperado " << yytext << " en línea " << line << std::endl; 
            contador_errores++;
            }

<<EOF>>     { yyterminate(); }

%%


