#include "headers/SymTab.hpp"

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
    // TODO: Implementar (similar al anterior pero usando el constructor con params)
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

// Imprime la tabla de símbolos
void SymTab::print() {
    cout << "========== TABLA DE SIMBOLOS ==========" << endl;
    cout << "Nombre\t\tDir\tTipo\tCat" << endl;
    cout << "------\t\t---\t----\t---" << endl;
    for (auto& par : syms) {
        cout << par.first << "\t\t"
            << par.second.dir << "\t"
            << par.second.tipo << "\t"
            << par.second.cat << endl;
    }
    cout << "========================================" << endl;
}
