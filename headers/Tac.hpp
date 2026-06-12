#ifndef CUADRUPLA_H
#define CUADRUPLA_H

#include <string>
using namespace std;

// Clase que representa una Cuádrupla (intruccion de Código de Tres Direcciones - TAC)
class Tac
{
private:
    string op;      // Operador 
    string arg1;    // Primer operando
    string arg2;    // Segundo operando
    string res;     // Variable o temporal donde se guardará el resultado

public:
    // Constructor por defecto
    Tac();

    // Contructor para inicializar la cuádrupla con todos sus elementos
    Tac(string op, string arg1, string arg2, string res);

    // Métodos getters para acceder a los atributos privados
    string getOp() const;
    string getArg1() const;
    string getArg2() const;
    string getRes() const;
};

#endif