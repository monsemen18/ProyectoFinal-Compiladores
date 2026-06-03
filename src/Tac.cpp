#include "headers/Tac.hpp"

Tac::Tac(){

}

Tac::Tac(string operador, string arg1, string arg2, string res)
{
    this->op = operador;
    this->arg1 = arg1;
    this->arg2 = arg2;
    this->res = res;
}

string Tac::getOp() const
{
    return op;
}

string Tac::getArg1() const{
    return arg1;
}

string Tac::getArg2() const{
    return arg2;
}

string Tac::getRes() const
{
    return res;
}