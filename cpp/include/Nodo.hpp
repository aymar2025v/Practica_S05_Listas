// ============================================================
// Nodo.hpp
// Práctica N.º 05 — SIS210
// Nodo genérico para listas enlazadas simples.
// ============================================================

#pragma once

#include <utility>   // std::move

template <typename T>
struct Nodo {
    T dato;
    Nodo* siguiente;

    // Constructor explícito: evita conversiones implícitas
    explicit Nodo(T d)
        : dato(std::move(d)), siguiente(nullptr) {}

    // Deshabilitamos copia para evitar dobles liberaciones accidentales
    Nodo(const Nodo&)            = delete;
    Nodo& operator=(const Nodo&) = delete;
};