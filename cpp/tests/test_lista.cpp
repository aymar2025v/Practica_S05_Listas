// ============================================================
// test_lista.cpp
// Práctica N.º 05 — SIS210
// Pruebas unitarias con assert para ListaEnlazada<T>.
// Compilar: g++ -std=c++17 -Wall -Wextra -o build/test_lista tests/test_lista.cpp
// ============================================================

#include "../include/ListaEnlazada.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// ------------------------------------------------------------
// Contadores globales de pruebas
// ------------------------------------------------------------
static int pruebas_ejecutadas = 0;
static int pruebas_ok         = 0;

// Macro de aserción que imprime el nombre de la prueba
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

// ------------------------------------------------------------
// Utilidad: convertir una lista a std::vector para comparar
// ------------------------------------------------------------
template <typename T>
std::vector<T> aVector(const ListaEnlazada<T>& l) {
    std::vector<T> v;
    for (Nodo<T>* a = l.cabeza(); a != nullptr; a = a->siguiente) {
        v.push_back(a->dato);
    }
    return v;
}

// ============================================================
// PRUEBAS
// ============================================================

static void test_insertarInicio() {
    ListaEnlazada<int> l;
    l.insertarInicio(3);
    l.insertarInicio(2);
    l.insertarInicio(1);
    CHECK(l.size() == 3, "insertarInicio: tamaño = 3");
    CHECK(l.cabeza()->dato == 1, "insertarInicio: cabeza = 1");
    CHECK(l.cola()->dato   == 3, "insertarInicio: cola = 3");
    CHECK((aVector(l) == std::vector<int>{1, 2, 3}), "insertarInicio: orden correcto");
}

static void test_insertarFinal() {
    ListaEnlazada<int> l;
    for (int v : {10, 20, 30, 40, 50}) l.insertarFinal(v);
    CHECK(l.size() == 5, "insertarFinal: tamaño = 5");
    CHECK(l.cabeza()->dato == 10, "insertarFinal: cabeza = 10");
    CHECK(l.cola()->dato   == 50, "insertarFinal: cola = 50");
    CHECK((aVector(l) == std::vector<int>{10, 20, 30, 40, 50}), "insertarFinal: orden correcto");
}

static void test_insertarEnPosicion() {
    ListaEnlazada<int> l;
    for (int v : {10, 20, 30, 40, 50}) l.insertarFinal(v);

    l.insertarEnPosicion(0, 5);   // al inicio
    CHECK(l.cabeza()->dato == 5, "insertarEnPosicion(0): cabeza = 5");

    l.insertarEnPosicion(l.size(), 60);  // al final
    CHECK(l.cola()->dato == 60, "insertarEnPosicion(size): cola = 60");

    l.insertarEnPosicion(2, 15);  // en medio
    CHECK((aVector(l) == std::vector<int>{5, 10, 15, 20, 30, 40, 50, 60}),
          "insertarEnPosicion(2, 15): orden correcto");
}

static void test_insertarEnPosicionInvalida() {
    ListaEnlazada<int> l;
    l.insertarFinal(1);
    bool lanzo = false;
    try {
        l.insertarEnPosicion(100, -1);
    } catch (const std::out_of_range&) {
        lanzo = true;
    }
    CHECK(lanzo, "insertarEnPosicion(100): lanza out_of_range");

    lanzo = false;
    try {
        l.insertarEnPosicion(-1, -1);
    } catch (const std::out_of_range&) {
        lanzo = true;
    }
    CHECK(lanzo, "insertarEnPosicion(-1): lanza out_of_range");
}

static void test_eliminarCabeza() {
    ListaEnlazada<int> l;
    for (int v : {10, 20, 30, 40, 50}) l.insertarFinal(v);
    CHECK(l.eliminar(10), "eliminar(10): retorna true");
    CHECK(l.size() == 4, "eliminar(10): tamaño = 4");
    CHECK(l.cabeza()->dato == 20, "eliminar(10): nueva cabeza = 20");
}

static void test_eliminarCola() {
    ListaEnlazada<int> l;
    for (int v : {10, 20, 30, 40, 50}) l.insertarFinal(v);
    CHECK(l.eliminar(50), "eliminar(50): retorna true");
    CHECK(l.size() == 4, "eliminar(50): tamaño = 4");
    CHECK(l.cola()->dato == 40, "eliminar(50): nueva cola = 40");
}

static void test_eliminarIntermedio() {
    ListaEnlazada<int> l;
    for (int v : {10, 20, 30, 40, 50}) l.insertarFinal(v);
    CHECK(l.eliminar(30), "eliminar(30): retorna true");
    CHECK((aVector(l) == std::vector<int>{10, 20, 40, 50}),
          "eliminar(30): orden correcto");
}

static void test_eliminarInexistente() {
    ListaEnlazada<int> l;
    for (int v : {10, 20, 30}) l.insertarFinal(v);
    CHECK(!l.eliminar(999), "eliminar(999): retorna false");
    CHECK(l.size() == 3, "eliminar(999): tamaño intacto = 3");
}

static void test_eliminarUnicoNodo() {
    ListaEnlazada<int> l;
    l.insertarFinal(42);
    CHECK(l.eliminar(42), "eliminar único: retorna true");
    CHECK(l.cabeza() == nullptr, "eliminar único: cabeza = nullptr");
    CHECK(l.cola()   == nullptr, "eliminar único: cola = nullptr");
    CHECK(l.vacia(), "eliminar único: lista vacía");
}

static void test_buscar() {
    ListaEnlazada<int> l;
    for (int v : {10, 20, 30, 40, 50}) l.insertarFinal(v);
    CHECK(l.buscar(30) != nullptr, "buscar(30): encontrado");
    CHECK(l.buscar(30)->dato == 30, "buscar(30): dato = 30");
    CHECK(l.buscar(99) == nullptr, "buscar(99): nullptr");
}

static void test_invertir() {
    ListaEnlazada<int> l;
    for (int v : {10, 20, 30, 40, 50}) l.insertarFinal(v);
    l.invertir();
    CHECK((aVector(l) == std::vector<int>{50, 40, 30, 20, 10}),
          "invertir: orden correcto");
    CHECK(l.cabeza()->dato == 50, "invertir: nueva cabeza = 50");
    CHECK(l.cola()->dato   == 10, "invertir: nueva cola = 10");
}

static void test_invertirUnNodo() {
    ListaEnlazada<int> l;
    l.insertarFinal(7);
    l.invertir();
    CHECK(l.size() == 1, "invertir 1 nodo: tamaño = 1");
    CHECK(l.cabeza()->dato == 7, "invertir 1 nodo: cabeza = 7");
    CHECK(l.cola()->dato   == 7, "invertir 1 nodo: cola = 7");
}

static void test_invertirVacia() {
    ListaEnlazada<int> l;
    l.invertir();   // no debe romper
    CHECK(l.vacia(), "invertir vacía: sigue vacía");
}

static void test_copiaProfunda() {
    ListaEnlazada<int> original;
    for (int v : {1, 2, 3}) original.insertarFinal(v);
    ListaEnlazada<int> copia = original;
    original.insertarFinal(99);
    CHECK(copia.size() == 3, "copia profunda: tamaño = 3");
    CHECK((aVector(copia) == std::vector<int>{1, 2, 3}),
          "copia profunda: no se ve afectada por el original");
    CHECK(copia.cabeza() != original.cabeza(),
          "copia profunda: punteros distintos");
}

static void test_asignacionCopia() {
    ListaEnlazada<int> a;
    for (int v : {1, 2, 3}) a.insertarFinal(v);
    ListaEnlazada<int> b;
    for (int v : {9, 9}) b.insertarFinal(v);
    b = a;
    a.insertarFinal(100);
    CHECK(b.size() == 3, "asignación copia: tamaño = 3");
    CHECK((aVector(b) == std::vector<int>{1, 2, 3}),
          "asignación copia: valores correctos");
}

static void test_movimiento() {
    ListaEnlazada<int> a;
    for (int v : {1, 2, 3, 4}) a.insertarFinal(v);
    ListaEnlazada<int> b = std::move(a);
    CHECK(b.size() == 4, "movimiento: tamaño del destino = 4");
    CHECK(a.vacia(), "movimiento: origen queda vacío");
    CHECK((aVector(b) == std::vector<int>{1, 2, 3, 4}),
          "movimiento: valores transferidos");
}

static void test_templateString() {
    ListaEnlazada<std::string> l;
    l.insertarFinal("UNAP");
    l.insertarFinal("Puno");
    l.insertarFinal("2026");
    l.insertarInicio("SIS210");
    CHECK(l.size() == 4, "template string: tamaño = 4");
    CHECK(l.cabeza()->dato == "SIS210", "template string: cabeza = SIS210");
    CHECK(l.cola()->dato   == "2026",   "template string: cola = 2026");
}

// ============================================================
// MAIN
// ============================================================
int main() {
    std::cout << "\n===== TEST: ListaEnlazada<T> =====\n\n";

    test_insertarInicio();
    test_insertarFinal();
    test_insertarEnPosicion();
    test_insertarEnPosicionInvalida();
    test_eliminarCabeza();
    test_eliminarCola();
    test_eliminarIntermedio();
    test_eliminarInexistente();
    test_eliminarUnicoNodo();
    test_buscar();
    test_invertir();
    test_invertirUnNodo();
    test_invertirVacia();
    test_copiaProfunda();
    test_asignacionCopia();
    test_movimiento();
    test_templateString();

    std::cout << "\n========================================\n";
    std::cout << "Pruebas ejecutadas: " << pruebas_ejecutadas << "\n";
    std::cout << "Pruebas OK:         " << pruebas_ok         << "\n";
    std::cout << "========================================\n";

    if (pruebas_ejecutadas == pruebas_ok) {
        std::cout << "\nTodas las pruebas pasaron ✔\n";
        return 0;
    } else {
        std::cout << "\nHubo fallos ✘\n";
        return 1;
    }
}

// cd cpp
// compilacion y ejecucion

// g++ -std=c++17 -Wall -Wextra -o build/test_lista tests/test_lista.cpp && ./build/test_lista

/*
===== TEST: ListaEnlazada<T> =====

[OK]   insertarInicio: tamaño = 3
[OK]   insertarInicio: cabeza = 1
[OK]   insertarInicio: cola = 3
[OK]   insertarInicio: orden correcto
[OK]   insertarFinal: tamaño = 5
[OK]   insertarFinal: cabeza = 10
[OK]   insertarFinal: cola = 50
[OK]   insertarFinal: orden correcto
[OK]   insertarEnPosicion(0): cabeza = 5
[OK]   insertarEnPosicion(size): cola = 60
[OK]   insertarEnPosicion(2, 15): orden correcto
[OK]   insertarEnPosicion(100): lanza out_of_range
[OK]   insertarEnPosicion(-1): lanza out_of_range
[OK]   eliminar(10): retorna true
[OK]   eliminar(10): tamaño = 4
[OK]   eliminar(10): nueva cabeza = 20
[OK]   eliminar(50): retorna true
[OK]   eliminar(50): tamaño = 4
[OK]   eliminar(50): nueva cola = 40
[OK]   eliminar(30): retorna true
[OK]   eliminar(30): orden correcto
[OK]   eliminar(999): retorna false
[OK]   eliminar(999): tamaño intacto = 3
[OK]   eliminar único: retorna true
[OK]   eliminar único: cabeza = nullptr
[OK]   eliminar único: cola = nullptr
[OK]   eliminar único: lista vacía
[OK]   buscar(30): encontrado
[OK]   buscar(30): dato = 30
[OK]   buscar(99): nullptr
[OK]   invertir: orden correcto
[OK]   invertir: nueva cabeza = 50
[OK]   invertir: nueva cola = 10
[OK]   invertir 1 nodo: tamaño = 1
[OK]   invertir 1 nodo: cabeza = 7
[OK]   invertir 1 nodo: cola = 7
[OK]   invertir vacía: sigue vacía
[OK]   copia profunda: tamaño = 3
[OK]   copia profunda: no se ve afectada por el original
[OK]   copia profunda: punteros distintos
[OK]   asignación copia: tamaño = 3
[OK]   asignación copia: valores correctos
[OK]   movimiento: tamaño del destino = 4
[OK]   movimiento: origen queda vacío
[OK]   movimiento: valores transferidos
[OK]   template string: tamaño = 4
[OK]   template string: cabeza = SIS210
[OK]   template string: cola = 2026

========================================
Pruebas ejecutadas: 48
Pruebas OK:         48
========================================

Todas las pruebas pasaron ✔
*/