#include "headers/SymTab.hpp"
#include <iomanip>

SymTab::SymTab() {}

// Agrega un símbolo a la tabla de símbolos
// Parámetros:
//   id:   nombre del identificador
//   dir:  dirección de memoria asignada
//   tipo: ID del tipo (referencia a TypeTab)
//   cat:  categoría ("var", "func", "param")
bool SymTab::addSym(string id, int dir, int tipo, string cat) {
    if(existe(id)) {
        return false;
    } 
    syms[id] = Sym(dir, tipo, cat);
    return true; 
}

// Versión para funciones (incluye lista de parámetros)
bool SymTab::addSym(string id, int dir, int tipo, string cat, vector<int> params) {
    if(existe(id)) {
        return false;
    } 
    
    syms[id] = Sym(dir, tipo, cat, params);
    return true; // Placeholder
}

// Verifica si un identificador ya existe en esta tabla de símbolos
// Parámetros:
//   id: nombre del identificador a buscar
bool SymTab::existe(string id) {
    return syms.find(id) != syms.end();
}

// Obtiene la dirección de un símbolo
int SymTab::getDir(string id) {
    if (existe(id)) {
        return syms[id].dir;
    }
    cerr << "Error: símbolo '" << id << "' no encontrado." << endl;
    return -1;
}

// Obtiene el tipo de un símbolo
int SymTab::getType(string id) {
    if (existe(id)) {
        return syms[id].tipo;
    }
    cerr << "Error: símbolo '" << id << "' no encontrado." << endl;
    return -1;
}

// Obtiene la categoría de un símbolo
string SymTab::getCat(string id) {
    if (existe(id)) {
        return syms[id].cat;
    }
    return "";
}

// Retorna el mapa completo
map<string, Sym> SymTab::getSyms() {
    return syms;
}

// Obtiene la lista de parámetros de un símbolo
vector<int> SymTab::getArgs(string id) {
    if (existe(id)) {
        return syms[id].params;
    }
    cerr << "Error: símbolo '" << id << "' no encontrado." << endl;
    return vector<int>(); // Retorna un vector vacío en caso de error
}

// Imprime la tabla de símbolos
void SymTab::print() {
    cout << "========== TABLA DE SIMBOLOS ==========" << endl;
    
    cout << left 
         << setw(18) << "Nombre" 
         << setw(8)  << "Dir" 
         << setw(8)  << "Tipo" 
         << setw(10) << "Cat" 
         << "Params" << endl;
         
    cout << "------------------------------------------------" << endl;
    
    for (auto& par : syms) {
        cout << left 
             << setw(18) << par.first 
             << setw(8)  << par.second.dir 
             << setw(8)  << par.second.tipo 
             << setw(10) << par.second.cat;
             
        // Lógica para imprimir el vector de parámetros
        if (!par.second.params.empty()) {
            cout << "[";
            for (size_t i = 0; i < par.second.params.size(); ++i) {
                cout << par.second.params[i];
                if (i < par.second.params.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "]";
        } else if (par.second.cat == "func") {
            cout << "[]";
        } else {
            cout << "-";
        }
        
        cout << endl;
    }
    cout << "================================================" << endl;
}
