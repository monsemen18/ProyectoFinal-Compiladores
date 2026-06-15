# Front-end de Compilador

## Equipo 
1. Méndez Camacho Monserrat - 424490034
2. Rodríguez Nevarez Edwin - 312227441
3. Carrillo Gonzalez Luis Israel - 321050324
4. Reyna Méndez Cristian Ignacio - 320140579

## Descripción

Un front-end de compilador para un subconjunto de un lenguaje tipo C. 

## Caracteristicas

- Análisis léxico
- Análisis sintáctico
- Manejo de tablas de símbolos
- Manejo de tabla de tipos
- Verificación de tipos
- Manejo de ámbitos
- Generación de código intermedio de tres direcciones

## Estructura del proyecto

```
├── headers
│   ├── Attributes.hpp
│   ├── CodeGen.hpp
│   ├── Lexer.hpp
│   ├── PilaTs.hpp
│   ├── SymTab.hpp
│   ├── Tac.hpp
│   └── TypeTab.
├── pruebas
├── Makefile
├── README.md
└── src
    ├── CodeGen.cpp
    ├── lexer.ll
    ├── main.cpp
    ├── parser.yy
    ├── PilaTs.cpp
    ├── SymTab.cpp
    ├── Tac.cpp
    └── TypeTab.cpp
```

## Uso

### Compilar

```bash
make
```

```bash
./comp/pruebas/[archivo de prueba]
```
