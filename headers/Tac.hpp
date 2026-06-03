#ifndef CUADRUPLA_H
#define CUADRUPLA_H

#include <string>
using namespace std;

class Tac
{
private:
    string op;
    string arg1;
    string arg2;
    string res;
public:
    Tac();
    Tac(string op, string arg1, string arg2, string res);
    string getOp() const;
    string getArg1() const;
    string getArg2() const;
    string getRes() const;
};

#endif