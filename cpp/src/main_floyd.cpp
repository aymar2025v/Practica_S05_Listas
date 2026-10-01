// ============================================================
// main_floyd.cpp
// Práctica N.º 05 — SIS210
// Demostración del algoritmo de Floyd en C++ (Actividad 5).
// ============================================================

#include "../include/ListaEnlazada.hpp"
#include "../include/Floyd.hpp"

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
    // 1) Lista SIN ciclo usando ListaEnlazada<int>
    // --------------------------------------------------------
    titulo("1) Lista sin ciclo (ListaEnlazada<int>)");
    ListaEnlazada<int> l;
    for (int v : {1, 2, 3, 4, 5}) l.insertarFinal(v);
    l.imprimir();
    std::cout << "tieneCiclo()  -> " << (l.tieneCiclo()  ? "true" : "false") << "\n";
    std::cout << "inicioCiclo() -> " << (l.inicioCiclo() ? "nodo" : "nullptr") << "\n";
    assert(!l.tieneCiclo());
    assert(l.inicioCiclo() == nullptr);

    // --------------------------------------------------------
    // 2) Lista VACÍA
    // --------------------------------------------------------
    titulo("2) Lista vacía");
    ListaEnlazada<int> vacia;
    assert(!vacia.tieneCiclo());
    assert(vacia.inicioCiclo() == nullptr);
    std::cout << "tieneCiclo() -> false, inicioCiclo() -> nullptr\n";

    // --------------------------------------------------------
    // 3) Un solo nodo sin ciclo
    // --------------------------------------------------------
    titulo("3) Un solo nodo sin ciclo");
    ListaEnlazada<int> uno;
    uno.insertarFinal(42);
    assert(!uno.tieneCiclo());
    assert(uno.inicioCiclo() == nullptr);
    std::cout << "tieneCiclo() -> false\n";

    // --------------------------------------------------------
    // 4) Ciclo manual con nodos sueltos: 1 → 2 → 3 → 4 → 5 → (3)
    //    Aquí usamos las funciones LIBRES de Floyd.hpp.
    // --------------------------------------------------------
    titulo("4) Ciclo manual 1→2→3→4→5→3");
    Nodo<int>* n1 = new Nodo<int>(1);
    Nodo<int>* n2 = new Nodo<int>(2);
    Nodo<int>* n3 = new Nodo<int>(3);
    Nodo<int>* n4 = new Nodo<int>(4);
    Nodo<int>* n5 = new Nodo<int>(5);

    n1->siguiente = n2;
    n2->siguiente = n3;
    n3->siguiente = n4;
    n4->siguiente = n5;
    n5->siguiente = n3;   // ciclo hacia n3

    std::cout << "tieneCicloLibre()   -> " << (tieneCicloLibre(n1)  ? "true" : "false") << "\n";

    Nodo<int>* inicio = inicioCicloLibre(n1);
    if (inicio != nullptr) {
        std::cout << "inicioCicloLibre()  -> nodo con dato = " << inicio->dato << "\n";
        assert(inicio->dato == 3);
    } else {
        std::cerr << "ERROR: no se detectó el ciclo\n";
    }

    // Liberar manualmente para no dejar memory leaks
    // (rompemos el ciclo antes de borrar)
    n5->siguiente = nullptr;
    delete n1;
    delete n2;
    delete n3;
    delete n4;
    delete n5;

    // --------------------------------------------------------
    // 5) Ciclo que empieza en la cabeza: 1 → 2 → 3 → 1
    // --------------------------------------------------------
    titulo("5) Ciclo desde la cabeza 1→2→3→1");
    Nodo<int>* m1 = new Nodo<int>(1);
    Nodo<int>* m2 = new Nodo<int>(2);
    Nodo<int>* m3 = new Nodo<int>(3);
    m1->siguiente = m2;
    m2->siguiente = m3;
    m3->siguiente = m1;

    Nodo<int>* ini_m = inicioCicloLibre(m1);
    assert(ini_m == m1);
    std::cout << "inicioCicloLibre() -> nodo con dato = " << ini_m->dato << " (cabeza)\n";

    m3->siguiente = nullptr;
    delete m1; delete m2; delete m3;

    // --------------------------------------------------------
    // 6) Un solo nodo con auto-ciclo
    // --------------------------------------------------------
    titulo("6) Auto-ciclo en un solo nodo");
    Nodo<int>* s = new Nodo<int>(7);
    s->siguiente = s;
    assert(tieneCicloLibre(s));
    assert(inicioCicloLibre(s) == s);
    std::cout << "tieneCicloLibre()  -> true\n";
    std::cout << "inicioCicloLibre() -> mismo nodo (dato=" << s->dato << ")\n";
    s->siguiente = nullptr;
    delete s;

    // --------------------------------------------------------
    // 7) Dos nodos con ciclo mutuo: 1 ↔ 2
    // --------------------------------------------------------
    titulo("7) Ciclo mutuo 1 ↔ 2");
    Nodo<int>* a = new Nodo<int>(1);
    Nodo<int>* b = new Nodo<int>(2);
    a->siguiente = b;
    b->siguiente = a;
    assert(tieneCicloLibre(a));
    assert(inicioCicloLibre(a) == a);
    std::cout << "inicioCicloLibre() -> nodo con dato = 1\n";
    b->siguiente = nullptr;
    delete a; delete b;

    // --------------------------------------------------------
    // Fin
    // --------------------------------------------------------
    titulo("Todas las aserciones de Floyd pasaron correctamente ✔");
    return 0;
}

// cd cpp
// g++ -std=c++17 -Wall -Wextra -o build/main_floyd src/main_floyd.cpp && ./build/main_floyd

/*
============================================================
1) Lista sin ciclo (ListaEnlazada<int>)
============================================================
1 -> 2 -> 3 -> 4 -> 5 -> NULL
tieneCiclo()  -> false
inicioCiclo() -> nullptr

============================================================
2) Lista vacía
============================================================
tieneCiclo() -> false, inicioCiclo() -> nullptr

============================================================
3) Un solo nodo sin ciclo
============================================================
tieneCiclo() -> false

============================================================
4) Ciclo manual 1→2→3→4→5→3
============================================================
tieneCicloLibre()   -> true
inicioCicloLibre()  -> nodo con dato = 3

============================================================
5) Ciclo desde la cabeza 1→2→3→1
============================================================
inicioCicloLibre() -> nodo con dato = 1 (cabeza)

============================================================
6) Auto-ciclo en un solo nodo
============================================================
tieneCicloLibre()  -> true
inicioCicloLibre() -> mismo nodo (dato=7)

============================================================
7) Ciclo mutuo 1 ↔ 2
============================================================
inicioCicloLibre() -> nodo con dato = 1

============================================================
Todas las aserciones de Floyd pasaron correctamente ✔
============================================================
*/