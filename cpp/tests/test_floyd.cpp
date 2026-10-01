// ============================================================
// test_floyd.cpp
// Práctica N.º 05 — SIS210
// Pruebas unitarias con assert para el algoritmo de Floyd.
// Compilar: g++ -std=c++17 -Wall -Wextra -o build/test_floyd tests/test_floyd.cpp
// ============================================================

#include "../include/ListaEnlazada.hpp"
#include "../include/Floyd.hpp"

#include <cassert>
#include <iostream>
#include <string>

// ------------------------------------------------------------
// Contadores globales
// ------------------------------------------------------------
static int pruebas_ejecutadas = 0;
static int pruebas_ok         = 0;

#define CHECK(cond, nombre)                                    \
    do {                                                       \
        ++pruebas_ejecutadas;                                  \
        if (cond) {                                            \
            ++pruebas_ok;                                      \
            std::cout << "[OK]   " << nombre << "\n";          \
        } else {                                               \
            std::cout << "[FALLO] " << nombre << "\n";         \
            assert(cond);                                      \
        }                                                      \
    } while (0)

// ============================================================
// Helpers para construir listas con nodos sueltos
// ============================================================

// Crea n nodos con valores 1..n y los enlaza. NO crea ciclo.
// Devuelve el puntero al primer nodo.
static Nodo<int>* construirLista(int n) {
    if (n <= 0) return nullptr;
    Nodo<int>* cabeza = new Nodo<int>(1);
    Nodo<int>* actual = cabeza;
    for (int i = 2; i <= n; ++i) {
        actual->siguiente = new Nodo<int>(i);
        actual = actual->siguiente;
    }
    return cabeza;
}

// Libera toda la memoria de una lista SIN ciclo.
static void liberarLista(Nodo<int>* cabeza) {
    while (cabeza != nullptr) {
        Nodo<int>* sig = cabeza->siguiente;
        delete cabeza;
        cabeza = sig;
    }
}

// ============================================================
// 1) Métodos de ListaEnlazada<T>
// ============================================================

static void test_listaVacia_sinCiclo() {
    ListaEnlazada<int> l;
    CHECK(!l.tieneCiclo(),         "vacia: tieneCiclo() = false");
    CHECK(l.inicioCiclo() == nullptr, "vacia: inicioCiclo() = nullptr");
}

static void test_unNodo_sinCiclo() {
    ListaEnlazada<int> l;
    l.insertarFinal(42);
    CHECK(!l.tieneCiclo(),          "un nodo: tieneCiclo() = false");
    CHECK(l.inicioCiclo() == nullptr, "un nodo: inicioCiclo() = nullptr");
}

static void test_listaSimple_sinCiclo() {
    ListaEnlazada<int> l;
    for (int v : {1, 2, 3, 4, 5}) l.insertarFinal(v);
    CHECK(!l.tieneCiclo(),          "lista 1..5: tieneCiclo() = false");
    CHECK(l.inicioCiclo() == nullptr, "lista 1..5: inicioCiclo() = nullptr");
}

// ============================================================
// 2) Funciones libres — casos sin ciclo
// ============================================================

static void test_libre_listaVacia() {
    Nodo<int>* cabeza = nullptr;
    CHECK(!tieneCicloLibre(cabeza),      "libre: vacía → false");
    CHECK(inicioCicloLibre(cabeza) == nullptr, "libre: vacía → nullptr");
}

static void test_libre_unNodo_sinCiclo() {
    Nodo<int>* n = new Nodo<int>(1);
    CHECK(!tieneCicloLibre(n),       "libre: un nodo → false");
    CHECK(inicioCicloLibre(n) == nullptr, "libre: un nodo → nullptr");
    delete n;
}

static void test_libre_listaSinCiclo() {
    Nodo<int>* cabeza = construirLista(5);
    CHECK(!tieneCicloLibre(cabeza),       "libre: 1..5 → false");
    CHECK(inicioCicloLibre(cabeza) == nullptr, "libre: 1..5 → nullptr");
    liberarLista(cabeza);
}

// ============================================================
// 3) Funciones libres — casos con ciclo
// ============================================================

static void test_libre_cicloEnMedio() {
    // 1 → 2 → 3 → 4 → 5 → (vuelve a 3)
    Nodo<int>* cabeza = construirLista(5);
    Nodo<int>* n3 = cabeza->siguiente->siguiente;   // nodo 3
    Nodo<int>* n5 = n3->siguiente->siguiente;       // nodo 5
    n5->siguiente = n3;                             // ciclo a 3

    CHECK(tieneCicloLibre(cabeza), "libre: ciclo en medio → true");
    Nodo<int>* inicio = inicioCicloLibre(cabeza);
    CHECK(inicio != nullptr,        "libre: inicio != nullptr");
    CHECK(inicio->dato == 3,        "libre: inicio tiene dato = 3");

    n5->siguiente = nullptr;   // rompemos el ciclo antes de liberar
    liberarLista(cabeza);
}

static void test_libre_cicloEnCabeza() {
    // 1 → 2 → 3 → (vuelve a 1)
    Nodo<int>* cabeza = construirLista(3);
    Nodo<int>* n3 = cabeza->siguiente->siguiente;
    n3->siguiente = cabeza;

    CHECK(tieneCicloLibre(cabeza),   "libre: ciclo en cabeza → true");
    CHECK(inicioCicloLibre(cabeza) == cabeza,
          "libre: inicio = cabeza");

    n3->siguiente = nullptr;
    liberarLista(cabeza);
}

static void test_libre_cicloEnCola_autociclo() {
    // 1 → 2 → 3 → (vuelve a 3)
    Nodo<int>* cabeza = construirLista(3);
    Nodo<int>* n3 = cabeza->siguiente->siguiente;
    n3->siguiente = n3;   // auto-ciclo

    CHECK(tieneCicloLibre(cabeza),   "libre: auto-ciclo en cola → true");
    CHECK(inicioCicloLibre(cabeza) == n3,
          "libre: inicio = último nodo");

    n3->siguiente = nullptr;
    liberarLista(cabeza);
}

static void test_libre_autoCiclo_unNodo() {
    Nodo<int>* n = new Nodo<int>(7);
    n->siguiente = n;

    CHECK(tieneCicloLibre(n),      "libre: auto-ciclo 1 nodo → true");
    CHECK(inicioCicloLibre(n) == n, "libre: inicio = mismo nodo");

    n->siguiente = nullptr;
    delete n;
}

static void test_libre_cicloMutuo() {
    // 1 ↔ 2
    Nodo<int>* a = new Nodo<int>(1);
    Nodo<int>* b = new Nodo<int>(2);
    a->siguiente = b;
    b->siguiente = a;

    CHECK(tieneCicloLibre(a),      "libre: ciclo mutuo → true");
    CHECK(inicioCicloLibre(a) == a, "libre: inicio = primer nodo");

    b->siguiente = nullptr;
    delete a; delete b;
}

static void test_libre_cicloLargo() {
    // 1 → 2 → ... → 10 → (vuelve a 5)
    Nodo<int>* cabeza = construirLista(10);
    Nodo<int>* n5  = cabeza;
    for (int i = 0; i < 4; ++i) n5 = n5->siguiente;   // llegamos a 5
    Nodo<int>* n10 = cabeza;
    for (int i = 0; i < 9; ++i) n10 = n10->siguiente; // llegamos a 10
    n10->siguiente = n5;

    CHECK(tieneCicloLibre(cabeza), "libre: ciclo largo → true");
    Nodo<int>* inicio = inicioCicloLibre(cabeza);
    CHECK(inicio != nullptr,        "libre: ciclo largo → inicio != nullptr");
    CHECK(inicio->dato == 5,        "libre: ciclo largo → inicio = 5");

    n10->siguiente = nullptr;
    liberarLista(cabeza);
}

// ============================================================
// 4) Verificar que la lista NO se modifica
// ============================================================

static void test_noModificaLista() {
    ListaEnlazada<int> l;
    for (int v : {1, 2, 3, 4, 5}) l.insertarFinal(v);

    // Capturamos los datos y los punteros antes
    Nodo<int>* cab = l.cabeza();
    Nodo<int>* col = l.cola();
    int n = l.size();

    l.tieneCiclo();
    l.inicioCiclo();

    CHECK(l.cabeza() == cab, "no modifica: cabeza intacta");
    CHECK(l.cola()   == col, "no modifica: cola intacta");
    CHECK(l.size()   == n,   "no modifica: tamaño intacto");
}

// ============================================================
// MAIN
// ============================================================
int main() {
    std::cout << "\n===== TEST: Algoritmo de Floyd =====\n\n";

    // Métodos de ListaEnlazada
    test_listaVacia_sinCiclo();
    test_unNodo_sinCiclo();
    test_listaSimple_sinCiclo();
    test_noModificaLista();

    std::cout << "\n--- Funciones libres ---\n\n";

    // Funciones libres
    test_libre_listaVacia();
    test_libre_unNodo_sinCiclo();
    test_libre_listaSinCiclo();
    test_libre_cicloEnMedio();
    test_libre_cicloEnCabeza();
    test_libre_cicloEnCola_autociclo();
    test_libre_autoCiclo_unNodo();
    test_libre_cicloMutuo();
    test_libre_cicloLargo();

    std::cout << "\n========================================\n";
    std::cout << "Pruebas ejecutadas: " << pruebas_ejecutadas << "\n";
    std::cout << "Pruebas OK:         " << pruebas_ok         << "\n";
    std::cout << "========================================\n";

    if (pruebas_ejecutadas == pruebas_ok) {
        std::cout << "\nTodas las pruebas de Floyd pasaron ✔\n";
        return 0;
    } else {
        std::cout << "\nHubo fallos ✘\n";
        return 1;
    }
}

// cd cpp
// compilar y ejecutar

// g++ -std=c++17 -Wall -Wextra -o build/test_floyd tests/test_floyd.cpp && ./build/test_floyd

/*
===== TEST: Algoritmo de Floyd =====

[OK]   vacia: tieneCiclo() = false
[OK]   vacia: inicioCiclo() = nullptr
[OK]   un nodo: tieneCiclo() = false
[OK]   un nodo: inicioCiclo() = nullptr
[OK]   lista 1..5: tieneCiclo() = false
[OK]   lista 1..5: inicioCiclo() = nullptr
[OK]   no modifica: cabeza intacta
[OK]   no modifica: cola intacta
[OK]   no modifica: tamaño intacto

--- Funciones libres ---

[OK]   libre: vacía → false
[OK]   libre: vacía → nullptr
[OK]   libre: un nodo → false
[OK]   libre: un nodo → nullptr
[OK]   libre: 1..5 → false
[OK]   libre: 1..5 → nullptr
[OK]   libre: ciclo en medio → true
[OK]   libre: inicio != nullptr
[OK]   libre: inicio tiene dato = 3
[OK]   libre: ciclo en cabeza → true
[OK]   libre: inicio = cabeza
[OK]   libre: auto-ciclo en cola → true
[OK]   libre: inicio = último nodo
[OK]   libre: auto-ciclo 1 nodo → true
[OK]   libre: inicio = mismo nodo
[OK]   libre: ciclo mutuo → true
[OK]   libre: inicio = primer nodo
[OK]   libre: ciclo largo → true
[OK]   libre: ciclo largo → inicio != nullptr
[OK]   libre: ciclo largo → inicio = 5

========================================
Pruebas ejecutadas: 29
Pruebas OK:         29
========================================

Todas las pruebas de Floyd pasaron ✔
*/