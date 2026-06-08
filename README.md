# Front-end de Compilador

## Equipo 
1. Mendez Camacho Monserrat
2. jjd
3. dk
4. kd

## Descripción

Un front-end de compilador para un subconjunto de un lenguaje tipo C 

## Caracteristicas

- análisis léxico
- análisis sintáctico
- manejo de tablas de símbolos
- manejo de tabla de tipos
- verificación de tipos
- manejo de ámbitos
- generación de código intermedio de tres direcciones

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
