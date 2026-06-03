#include "headers/TypeTab.hpp"

// Constructor: pre-carga los tipos primitivos
TypeTab::TypeTab() {
    // Tipo 0: int (4 bytes)
    types[0] = Type("int", 4);
    // Tipo 1: float (4 bytes)
    types[1] = Type("float", 4);
    // Tipo 2: bool (4 bytes)
    types[2] = Type("bool", 4);
    // Tipo 3: char (4 bytes)
    types[3] = Type("char", 4);
    // El siguiente ID disponible es 2
    nextId = 4;
}

// Obtiene el ID de un tipo por nombre
int TypeTab::getId(string name) {
    for (auto& par : types) {
        if (par.second.name == name) {
            return par.first;
        }
    }
    return -1;  // No encontrado
}

// Agrega un tipo arreglo a la tabla de tipos
// Parámetros:
//   numElems: número de elementos del arreglo
//   tipoBase: ID del tipo base (ej: si es int[5], tipoBase = 0)
// Retorna: el ID del nuevo tipo creado
int TypeTab::addArrayType(int numElems, int tipoBase) {
    int tam = numElems * getTam(tipoBase);
    types[nextId] = Type("array", tam, numElems, tipoBase);
    int idAsignado = nextId;
    nextId++; 
    return idAsignado; 
}

// Agrega un tipo struct a la tabla de tipos
// Parámetros:
//   tam: tamaño total del struct (suma de tamaños de sus campos)
// Retorna: el ID del nuevo tipo creado
int TypeTab::addStructType(int tam) {
    types[nextId] = Type("struct", tam);
    int idAsignado = nextId;
    nextId++;
    return idAsignado; 
}

// Obtiene el tamaño de un tipo dado su ID
int TypeTab::getTam(int id) {
    if (types.find(id) != types.end()) {
        return types[id].tam;
    }
    cerr << "Error: tipo con ID " << id << " no encontrado en tabla de tipos." << endl;
    return 0;
}

// Obtiene el nombre de un tipo dado su ID
string TypeTab::getName(int id) {
    if (types.find(id) != types.end()) {
        return types[id].name;
    }
    return "desconocido";
}

// Imprime la tabla de tipos
void TypeTab::print() {
    cout << "========== TABLA DE TIPOS ==========" << endl;
    cout << "ID\tNombre\t\tTam\tElems\tBase" << endl;
    cout << "----\t------\t\t---\t-----\t----" << endl;
    for (auto& par : types) {
        cout << par.first << "\t"
            << par.second.name << "\t\t"
            << par.second.tam << "\t"
            << par.second.numElems << "\t"
            << par.second.tipoBase << endl;
    }
    cout << "====================================" << endl;
}
