#include "headers/Tac.hpp"

Tac::Tac(){

}

Tac::Tac(string operador, string arg1, string arg2, string res) {
    this->op = operador;
    this->arg1 = arg1;
    this->arg2 = arg2;
    this->res = res;
}

// Retorna el operador 
string Tac::getOp() const {
    return op;
}

// Retorna el valor del primer argumento
string Tac::getArg1() const {
    return arg1;
}

// Retorna el valor del segundo argumento
string Tac::getArg2() const {
    return arg2;
}

// Retorna el destino de la operación (variable o temporal)
string Tac::getRes() const {
    return res;
}