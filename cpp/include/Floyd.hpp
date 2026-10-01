// ============================================================
// Floyd.hpp
// Práctica N.º 05 — SIS210
// Algoritmo de Floyd (tortuga y liebre) como funciones libres.
// Permite detectar y localizar el inicio de un ciclo en O(n)
// tiempo y O(1) espacio.
// ============================================================

#pragma once

#include "Nodo.hpp"

// ------------------------------------------------------------
// tieneCicloLibre:
//   Detecta si existe un ciclo en la lista cuyo primer nodo es 'cabeza'.
//   No modifica la lista.
// ------------------------------------------------------------
template <typename T>
bool tieneCicloLibre(const Nodo<T>* cabeza) {
    const Nodo<T>* lento  = cabeza;
    const Nodo<T>* rapido = cabeza;

    while (rapido != nullptr && rapido->siguiente != nullptr) {
        lento  = lento->siguiente;
        rapido = rapido->siguiente->siguiente;
        if (lento == rapido) return true;
    }
    return false;
}

// ------------------------------------------------------------
// inicioCicloLibre:
//   Retorna el nodo donde INICIA el ciclo, o nullptr si no hay.
//   Implementa las DOS fases del algoritmo de Floyd.
// ------------------------------------------------------------
template <typename T>
Nodo<T>* inicioCicloLibre(Nodo<T>* cabeza) {
    if (cabeza == nullptr) return nullptr;

    Nodo<T>* lento  = cabeza;
    Nodo<T>* rapido = cabeza;

    // Fase 1: detectar colisión
    while (rapido != nullptr && rapido->siguiente != nullptr) {
        lento  = lento->siguiente;
        rapido = rapido->siguiente->siguiente;
        if (lento == rapido) break;
    }
    if (rapido == nullptr || rapido->siguiente == nullptr) {
        return nullptr;   // no hay ciclo
    }

    // Fase 2: encontrar el inicio
    lento = cabeza;
    while (lento != rapido) {
        lento  = lento->siguiente;
        rapido = rapido->siguiente;
    }
    return lento;
}