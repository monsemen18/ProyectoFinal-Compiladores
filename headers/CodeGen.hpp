#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <iosfwd>
#include <string>
#include <vector>
#include "Tac.hpp"

// Clase encargada de gestionar y almacenar la generación de Código de Tres Direcciones 
// TAC (Three Address Code) durante en el análisis semántico.
class CodeGen {
private:
	static std::vector<Tac> code;	// Vector que almacena todas las instrucciones generadas
	static int tempCount; 			// genera t1, t2, t3...
	static int labelCount;			// genera L1, L2, L3...

public:
	// Reinicia la lista de código
	static void reset();

	// Incrementa el contador y devuelve el nombre de una nueva temporal
	static std::string newTemp();

	// Incrementa el contador y devuelve el nombre de una nueva etiqueta
	static std::string newLabel();

	// Construye y agrega una nueva instrucción TAC a la lista usando sus componentes
	// op: Operador, arg1: Primer operando, arg2: Segundo operando, res: Resultado
	static void emit(const std::string& op,
					const std::string& arg1 = "",
					const std::string& arg2 = "",
					const std::string& res = "");

	// Sobrecarga de emit para agregar una instrucción TAC ya construida
	static void emit(const Tac& tac);

	// Devuelve la lista completa de instrucciones generadas
	static const std::vector<Tac>& getCode();

	// Imprime todo el código intermedio generado
	static void print(std::ostream& out);
};

#endif
