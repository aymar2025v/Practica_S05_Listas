// ============================================================
// main_lista.cpp
// Práctica N.º 05 — SIS210
// Demostración manual de ListaEnlazada<T> (Actividad 3).
// ============================================================

#include "../include/ListaEnlazada.hpp"

#include <cassert>
#include <iostream>
#include <string>

// ------------------------------------------------------------
// Utilidad para imprimir títulos de sección
// ------------------------------------------------------------
static void titulo(const std::string& texto) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << texto << "\n";
    std::cout << std::string(60, '=') << "\n";
}

int main() {
    // --------------------------------------------------------
    // 1) Inserción al final
    // --------------------------------------------------------
    titulo("1) insertarFinal");
    ListaEnlazada<int> l;
    for (int v : {10, 20, 30, 40, 50}) {
        l.insertarFinal(v);
    }
    l.imprimir();
    assert(l.size() == 5);
    assert(l.cabeza()->dato == 10);
    assert(l.cola()->dato   == 50);

    // --------------------------------------------------------
    // 2) Inserción al inicio
    // --------------------------------------------------------
    titulo("2) insertarInicio(5)");
    l.insertarInicio(5);
    l.imprimir();
    assert(l.size() == 6);
    assert(l.cabeza()->dato == 5);

    // --------------------------------------------------------
    // 3) Inserción en posición media
    // --------------------------------------------------------
    titulo("3) insertarEnPosicion(3, 25)");
    l.insertarEnPosicion(3, 25);
    l.imprimir();
    assert(l.size() == 7);

    // --------------------------------------------------------
    // 4) Posición fuera de rango → std::out_of_range
    // --------------------------------------------------------
    titulo("4) insertarEnPosicion(100, -1) → out_of_range");
    try {
        l.insertarEnPosicion(100, -1);
        std::cerr << "ERROR: no lanzó excepción\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Excepción capturada: " << e.what() << "\n";
    }

    // --------------------------------------------------------
    // 5) Búsqueda
    // --------------------------------------------------------
    titulo("5) buscar()");
    Nodo<int>* n30 = l.buscar(30);
    std::cout << "buscar(30) -> " << (n30 ? std::to_string(n30->dato) : "nullptr") << "\n";
    std::cout << "buscar(99) -> " << (l.buscar(99) ? "encontrado" : "nullptr") << "\n";
    assert(n30 != nullptr && n30->dato == 30);
    assert(l.buscar(99) == nullptr);

    // --------------------------------------------------------
    // 6) Eliminar cabeza
    // --------------------------------------------------------
    titulo("6) eliminar(5) [cabeza]");
    assert(l.eliminar(5));
    l.imprimir();
    assert(l.cabeza()->dato == 10);

    // --------------------------------------------------------
    // 7) Eliminar cola
    // --------------------------------------------------------
    titulo("7) eliminar(50) [cola]");
    assert(l.eliminar(50));
    l.imprimir();
    assert(l.cola()->dato == 40);

    // --------------------------------------------------------
    // 8) Eliminar intermedio
    // --------------------------------------------------------
    titulo("8) eliminar(25) [intermedio]");
    assert(l.eliminar(25));
    l.imprimir();

    // --------------------------------------------------------
    // 9) Eliminar inexistente
    // --------------------------------------------------------
    titulo("9) eliminar(999) → false");
    bool eliminado = l.eliminar(999);
    std::cout << "resultado: " << (eliminado ? "true" : "false") << "\n";
    assert(!eliminado);

    // --------------------------------------------------------
    // 10) Invertir
    // --------------------------------------------------------
    titulo("10) invertir()");
    l.invertir();
    l.imprimir();
    assert(l.cabeza()->dato == 40);
    assert(l.cola()->dato   == 10);

    // --------------------------------------------------------
    // 11) Caso borde: un solo nodo
    // --------------------------------------------------------
    titulo("11) Un solo nodo: eliminar deja lista vacía");
    ListaEnlazada<int> lu;
    lu.insertarFinal(42);
    lu.imprimir();
    assert(lu.eliminar(42));
    assert(lu.cabeza() == nullptr);
    assert(lu.cola()   == nullptr);
    assert(lu.size()   == 0);
    assert(lu.vacia());
    lu.imprimir();

    // --------------------------------------------------------
    // 12) Invertir lista vacía (no debe romper)
    // --------------------------------------------------------
    titulo("12) invertir() en lista vacía");
    ListaEnlazada<int> lv;
    lv.invertir();
    lv.imprimir();
    assert(lv.vacia());

    // --------------------------------------------------------
    // 13) Copia profunda
    // --------------------------------------------------------
    titulo("13) Constructor de copia (copia profunda)");
    ListaEnlazada<int> original;
    for (int v : {1, 2, 3}) original.insertarFinal(v);
    ListaEnlazada<int> copia = original;
    original.insertarFinal(99);   // modificar el original
    std::cout << "original: "; original.imprimir();
    std::cout << "copia:    "; copia.imprimir();
    assert(copia.size() == 3);    // la copia no debe verse afectada

    // --------------------------------------------------------
    // 14) Movimiento
    // --------------------------------------------------------
    titulo("14) Constructor de movimiento");
    ListaEnlazada<int> movida = std::move(original);
    std::cout << "movida:   "; movida.imprimir();
    std::cout << "original (tras mover): "; original.imprimir();
    assert(original.vacia());
    assert(movida.size() == 4);

    // --------------------------------------------------------
    // 15) Lista de strings (template)
    // --------------------------------------------------------
    titulo("15) ListaEnlazada<std::string>");
    ListaEnlazada<std::string> ls;
    ls.insertarFinal("UNAP");
    ls.insertarFinal("Puno");
    ls.insertarFinal("2026");
    ls.insertarInicio("SIS210");
    ls.imprimir();

    // --------------------------------------------------------
    // Fin
    // --------------------------------------------------------
    titulo("Todas las aserciones pasaron correctamente ✔");
    return 0;
}

// cd cpp
// g++ -std=c++17 -Wall -Wextra -o build/main_lista src/main_lista.cpp && ./build/main_lista

/*
============================================================
1) insertarFinal
============================================================
10 -> 20 -> 30 -> 40 -> 50 -> NULL

============================================================
2) insertarInicio(5)
============================================================
5 -> 10 -> 20 -> 30 -> 40 -> 50 -> NULL

============================================================
3) insertarEnPosicion(3, 25)
============================================================
5 -> 10 -> 20 -> 25 -> 30 -> 40 -> 50 -> NULL

============================================================
4) insertarEnPosicion(100, -1) → out_of_range
============================================================
Excepción capturada: Posición fuera de rango

============================================================
5) buscar()
============================================================
buscar(30) -> 30
buscar(99) -> nullptr

============================================================
6) eliminar(5) [cabeza]
============================================================
10 -> 20 -> 25 -> 30 -> 40 -> 50 -> NULL

============================================================
7) eliminar(50) [cola]
============================================================
10 -> 20 -> 25 -> 30 -> 40 -> NULL

============================================================
8) eliminar(25) [intermedio]
============================================================
10 -> 20 -> 30 -> 40 -> NULL

============================================================
9) eliminar(999) → false
============================================================
resultado: false

============================================================
10) invertir()
============================================================
40 -> 30 -> 20 -> 10 -> NULL

============================================================
11) Un solo nodo: eliminar deja lista vacía
============================================================
42 -> NULL
NULL

============================================================
12) invertir() en lista vacía
============================================================
NULL

============================================================
13) Constructor de copia (copia profunda)
============================================================
original: 1 -> 2 -> 3 -> 99 -> NULL
copia:    1 -> 2 -> 3 -> NULL

============================================================
14) Constructor de movimiento
============================================================
movida:   1 -> 2 -> 3 -> 99 -> NULL
original (tras mover): NULL

============================================================
15) ListaEnlazada<std::string>
============================================================
SIS210 -> UNAP -> Puno -> 2026 -> NULL

============================================================
Todas las aserciones pasaron correctamente ✔
============================================================
*/