#include "headers/CodeGen.hpp"

#include <iomanip>
#include <iostream>

std::vector<Tac> CodeGen::code;
int CodeGen::tempCount = 0;
int CodeGen::labelCount = 0;

void CodeGen::reset() {
	code.clear();
	tempCount = 0;
	labelCount = 0;
}

std::string CodeGen::newTemp() {
	return "t" + std::to_string(++tempCount);
}

std::string CodeGen::newLabel() {
	return "L" + std::to_string(++labelCount);
}

void CodeGen::emit(const std::string& op,
				const std::string& arg1,
				const std::string& arg2,
				const std::string& res) {
	code.emplace_back(op, arg1, arg2, res);
}

void CodeGen::emit(const Tac& tac) {
	code.push_back(tac);
}

const std::vector<Tac>& CodeGen::getCode() {
	return code;
}

void CodeGen::print(std::ostream& out) {
	for (const Tac& tac : code) {
		if (tac.getOp() == "label") {
			out << tac.getRes() << ':' << std::endl;
			continue;
		}

		if (tac.getOp() == "goto") {
			out << "goto " << tac.getRes() << std::endl;
			continue;
		}

		if (tac.getOp() == "if") {
			out << "if " << tac.getArg1() << " goto " << tac.getRes() << std::endl;
			continue;
		}

		if (tac.getOp() == "param") {
			out << "param " << tac.getRes() << std::endl;
			continue;
		}

		if (tac.getOp() == "call") {
			out << tac.getRes() << " = call " << tac.getArg1();
			if (!tac.getArg2().empty()) {
				out << ", " << tac.getArg2();
			}
			out << std::endl;
			continue;
		}

		if (tac.getOp() == "return") {
			out << "return " << tac.getRes() << std::endl;
			continue;
		}

		if (tac.getRes().empty()) {
			out << tac.getOp();
			if (!tac.getArg1().empty()) {
				out << ' ' << tac.getArg1();
			}
			if (!tac.getArg2().empty()) {
				out << ' ' << tac.getArg2();
			}
			out << std::endl;
			continue;
		}

		if (tac.getArg2().empty()) {
			out << tac.getRes() << " = " << tac.getArg1() << ' ' << tac.getOp() << std::endl;
		} else {
			out << tac.getRes() << " = " << tac.getArg1() << ' ' << tac.getOp() << ' ' << tac.getArg2() << std::endl;
		}
	}
}
