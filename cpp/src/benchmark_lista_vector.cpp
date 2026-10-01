// ============================================================
// benchmark_lista_vector.cpp
// Práctica N.º 05 — SIS210
// Actividad 6: comparación empírica entre std::list, std::vector
// y nuestra ListaEnlazada<int> propia.
// ============================================================

#include "../include/ListaEnlazada.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using Clock = std::chrono::high_resolution_clock;

// ------------------------------------------------------------
// Utilidad: medir el tiempo de una función lambda en milisegundos
// ------------------------------------------------------------
template <typename Func>
double medir_ms(Func&& f) {
    auto t0 = Clock::now();
    f();
    auto t1 = Clock::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

// ------------------------------------------------------------
// Utilidad: imprimir fila de tabla
// ------------------------------------------------------------
static void fila(const std::string& op,
                 const std::string& estructura,
                 double ms) {
    std::cout << std::left
              << std::setw(28) << op
              << std::setw(22) << estructura
              << std::right << std::setw(12)
              << std::fixed << std::setprecision(3) << ms
              << " ms\n";
}

int main() {
    const int N = 100'000;

    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "BENCHMARK — Lista enlazada vs std::vector vs std::list\n";
    std::cout << "N = " << N << " elementos\n";
    std::cout << std::string(70, '=') << "\n\n";

    std::cout << std::left
              << std::setw(28) << "Operación"
              << std::setw(22) << "Estructura"
              << std::right << std::setw(12) << "Tiempo"
              << "\n";
    std::cout << std::string(70, '-') << "\n";

    // ========================================================
    // 1) Inserción al INICIO
    // ========================================================
    {
        std::list<int> lst;
        double ms = medir_ms([&] {
            for (int i = 0; i < N; ++i) lst.push_front(i);
        });
        fila("push_front (inicio)", "std::list", ms);
    }
    {
        std::vector<int> v;
        double ms = medir_ms([&] {
            for (int i = 0; i < N; ++i) v.insert(v.begin(), i);
        });
        fila("insert(begin) (inicio)", "std::vector", ms);
    }
    {
        ListaEnlazada<int> l;
        double ms = medir_ms([&] {
            for (int i = 0; i < N; ++i) l.insertarInicio(i);
        });
        fila("insertarInicio (inicio)", "ListaEnlazada propia", ms);
    }

    std::cout << std::string(70, '-') << "\n";

    // ========================================================
    // 2) Inserción al FINAL
    // ========================================================
    {
        std::list<int> lst;
        double ms = medir_ms([&] {
            for (int i = 0; i < N; ++i) lst.push_back(i);
        });
        fila("push_back (final)", "std::list", ms);
    }
    {
        std::vector<int> v;
        v.reserve(N);   // reservamos para que no cuente la reallocación
        double ms = medir_ms([&] {
            for (int i = 0; i < N; ++i) v.push_back(i);
        });
        fila("push_back (final)", "std::vector (reserve)", ms);
    }
    {
        ListaEnlazada<int> l;
        double ms = medir_ms([&] {
            for (int i = 0; i < N; ++i) l.insertarFinal(i);
        });
        fila("insertarFinal (final)", "ListaEnlazada propia", ms);
    }

    std::cout << std::string(70, '-') << "\n";

    // ========================================================
    // 3) Recorrido completo (suma)
    // ========================================================
    {
        std::vector<int> v(N, 1);
        volatile int suma = 0;
        double ms = medir_ms([&] {
            for (int i = 0; i < N; ++i) suma += v[i];
        });
        fila("recorrido (acceso O(1))", "std::vector", ms);
    }
    {
        std::list<int> lst(N, 1);
        volatile int suma = 0;
        double ms = medir_ms([&] {
            for (int x : lst) suma += x;
        });
        fila("recorrido completo", "std::list", ms);
    }
    {
        ListaEnlazada<int> l;
        for (int i = 0; i < N; ++i) l.insertarFinal(1);
        volatile int suma = 0;
        double ms = medir_ms([&] {
            for (Nodo<int>* a = l.cabeza(); a != nullptr; a = a->siguiente) {
                suma += a->dato;
            }
        });
        fila("recorrido completo", "ListaEnlazada propia", ms);
    }

    std::cout << std::string(70, '-') << "\n";

    // ========================================================
    // 4) Búsqueda de un elemento inexistente (peor caso)
    // ========================================================
    {
        std::vector<int> v(N, 1);
        volatile int encontrado = -1;
        double ms = medir_ms([&] {
            for (int i = 0; i < N; ++i) {
                if (v[i] == 999'999) { encontrado = i; break; }
            }
        });
        fila("búsqueda (peor caso)", "std::vector", ms);
    }
    {
        std::list<int> lst(N, 1);
        volatile int encontrado = -1;
        double ms = medir_ms([&] {
            int i = 0;
            for (int x : lst) {
                if (x == 999'999) { encontrado = i; break; }
                ++i;
            }
        });
        fila("búsqueda (peor caso)", "std::list", ms);
    }
    {
        ListaEnlazada<int> l;
        for (int i = 0; i < N; ++i) l.insertarFinal(1);
        double ms = medir_ms([&] {
            (void) l.buscar(999'999);
        });
        fila("buscar (peor caso)", "ListaEnlazada propia", ms);
    }

    std::cout << std::string(70, '-') << "\n";

    // ========================================================
    // 5) Eliminación del primer elemento repetidamente
    // ========================================================
    {
        std::list<int> lst(N, 1);
        double ms = medir_ms([&] {
            while (!lst.empty()) lst.pop_front();
        });
        fila("pop_front repetido", "std::list", ms);
    }
    {
        std::vector<int> v(N, 1);
        double ms = medir_ms([&] {
            while (!v.empty()) v.erase(v.begin());
        });
        fila("erase(begin) repetido", "std::vector", ms);
    }
    {
        ListaEnlazada<int> l;
        for (int i = 0; i < N; ++i) l.insertarFinal(1);
        double ms = medir_ms([&] {
            while (!l.vacia()) l.eliminar(l.cabeza()->dato);
        });
        fila("eliminar cabeza repetido", "ListaEnlazada propia", ms);
    }

    std::cout << std::string(70, '=') << "\n";
    std::cout << "Fin del benchmark\n";
    return 0;
}

// cd cpp
// g++ -std=c++17 -O2 -Wall -Wextra -o build/benchmark src/benchmark_lista_vector.cpp && ./build/benchmark

/*
======================================================================
BENCHMARK — Lista enlazada vs std::vector vs std::list
N = 100000 elementos
======================================================================

Operación                  Estructura                  Tiempo
----------------------------------------------------------------------
push_front (inicio)         std::list                    2.501 ms
insert(begin) (inicio)      std::vector                250.056 ms
insertarInicio (inicio)     ListaEnlazada propia         2.424 ms
----------------------------------------------------------------------
push_back (final)           std::list                    2.526 ms
push_back (final)           std::vector (reserve)        0.084 ms
insertarFinal (final)       ListaEnlazada propia         2.388 ms
----------------------------------------------------------------------
recorrido (acceso O(1))     std::vector                  0.022 ms
recorrido completo          std::list                    0.694 ms
recorrido completo          ListaEnlazada propia         0.578 ms
----------------------------------------------------------------------
búsqueda (peor caso)       std::vector                  0.035 ms
búsqueda (peor caso)       std::list                    0.466 ms
buscar (peor caso)          ListaEnlazada propia         0.000 ms
----------------------------------------------------------------------
pop_front repetido          std::list                    1.590 ms
erase(begin) repetido       std::vector               4265.913 ms
eliminar cabeza repetido    ListaEnlazada propia         1.586 ms
======================================================================
Fin del benchmark
*/