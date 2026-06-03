#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <iosfwd>
#include <string>
#include <vector>
#include "Tac.hpp"

class CodeGen {
private:
	static std::vector<Tac> code;
	static int tempCount;
	static int labelCount;

public:
	static void reset();
	static std::string newTemp();
	static std::string newLabel();
	static void emit(const std::string& op,
					const std::string& arg1 = "",
					const std::string& arg2 = "",
					const std::string& res = "");
	static void emit(const Tac& tac);
	static const std::vector<Tac>& getCode();
	static void print(std::ostream& out);
};

#endif
