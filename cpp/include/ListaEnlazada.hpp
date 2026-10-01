// ============================================================
// ListaEnlazada.hpp
// Práctica N.º 05 — SIS210
// Lista enlazada simple genérica con gestión correcta de memoria.
// ============================================================

#pragma once

#include "Nodo.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>   // std::swap, std::move

template <typename T>
class ListaEnlazada {
private:
    Nodo<T>* cabeza_ = nullptr;
    Nodo<T>* cola_   = nullptr;
    int      n_      = 0;

public:
    // --------------------------------------------------------
    // Constructores / Destructor (Regla de Cinco)
    // --------------------------------------------------------
    ListaEnlazada() = default;

    // Constructor de copia (copia profunda)
    ListaEnlazada(const ListaEnlazada& otra) {
        for (Nodo<T>* a = otra.cabeza_; a != nullptr; a = a->siguiente) {
            insertarFinal(a->dato);
        }
    }

    // Constructor de movimiento
    ListaEnlazada(ListaEnlazada&& otra) noexcept
        : cabeza_(otra.cabeza_), cola_(otra.cola_), n_(otra.n_) {
        otra.cabeza_ = nullptr;
        otra.cola_   = nullptr;
        otra.n_      = 0;
    }

    // Asignación por copia (copy-and-swap)
    ListaEnlazada& operator=(ListaEnlazada otra) {
        swap(otra);
        return *this;
    }

    // Destructor: libera TODOS los nodos
    ~ListaEnlazada() {
        Nodo<T>* actual = cabeza_;
        while (actual != nullptr) {
            Nodo<T>* sig = actual->siguiente;
            delete actual;
            actual = sig;
        }
    }

    // Intercambio
    void swap(ListaEnlazada& otra) noexcept {
        std::swap(cabeza_, otra.cabeza_);
        std::swap(cola_,   otra.cola_);
        std::swap(n_,      otra.n_);
    }

    // --------------------------------------------------------
    // Inserción
    // --------------------------------------------------------
    void insertarInicio(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(std::move(dato));
        if (cabeza_ == nullptr) {
            cabeza_ = cola_ = nuevo;
        } else {
            nuevo->siguiente = cabeza_;
            cabeza_ = nuevo;
        }
        ++n_;
    }

    void insertarFinal(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(std::move(dato));
        if (cola_ == nullptr) {
            cabeza_ = cola_ = nuevo;
        } else {
            cola_->siguiente = nuevo;
            cola_ = nuevo;
        }
        ++n_;
    }

    void insertarEnPosicion(int posicion, T dato) {
        if (posicion < 0 || posicion > n_) {
            throw std::out_of_range("Posición fuera de rango");
        }
        if (posicion == 0) { insertarInicio(std::move(dato)); return; }
        if (posicion == n_) { insertarFinal(std::move(dato));  return; }

        Nodo<T>* anterior = cabeza_;
        for (int i = 0; i < posicion - 1; ++i) {
            anterior = anterior->siguiente;
        }
        Nodo<T>* nuevo = new Nodo<T>(std::move(dato));
        nuevo->siguiente = anterior->siguiente;
        anterior->siguiente = nuevo;
        ++n_;
    }

    // --------------------------------------------------------
    // Eliminación
    // --------------------------------------------------------
    bool eliminar(const T& dato) {
        Nodo<T>* actual   = cabeza_;
        Nodo<T>* anterior = nullptr;

        while (actual != nullptr) {
            if (actual->dato == dato) {
                if (anterior == nullptr) {
                    cabeza_ = actual->siguiente;
                    if (cabeza_ == nullptr) cola_ = nullptr;
                } else {
                    anterior->siguiente = actual->siguiente;
                    if (actual == cola_) cola_ = anterior;
                }
                delete actual;
                --n_;
                return true;
            }
            anterior = actual;
            actual   = actual->siguiente;
        }
        return false;
    }

    // --------------------------------------------------------
    // Búsqueda
    // --------------------------------------------------------
    Nodo<T>* buscar(const T& dato) {
        for (Nodo<T>* a = cabeza_; a != nullptr; a = a->siguiente) {
            if (a->dato == dato) return a;
        }
        return nullptr;
    }

    // --------------------------------------------------------
    // Inversión in-place con 3 punteros
    // --------------------------------------------------------
    void invertir() {
        Nodo<T>* prev    = nullptr;
        Nodo<T>* actual  = cabeza_;
        cola_ = cabeza_;                 // la cola pasa a ser la antigua cabeza

        while (actual != nullptr) {
            Nodo<T>* sig    = actual->siguiente;
            actual->siguiente = prev;
            prev    = actual;
            actual  = sig;
        }
        cabeza_ = prev;
    }

    // --------------------------------------------------------
    // Algoritmo de Floyd: ¿hay ciclo?
    // --------------------------------------------------------
    bool tieneCiclo() const {
        const Nodo<T>* lento  = cabeza_;
        const Nodo<T>* rapido = cabeza_;

        while (rapido != nullptr && rapido->siguiente != nullptr) {
            lento  = lento->siguiente;
            rapido = rapido->siguiente->siguiente;
            if (lento == rapido) return true;
        }
        return false;
    }

    // --------------------------------------------------------
    // Algoritmo de Floyd: nodo de inicio del ciclo (o nullptr)
    // --------------------------------------------------------
    Nodo<T>* inicioCiclo() {
        Nodo<T>* lento  = cabeza_;
        Nodo<T>* rapido = cabeza_;

        // Fase 1: detectar
        while (rapido != nullptr && rapido->siguiente != nullptr) {
            lento  = lento->siguiente;
            rapido = rapido->siguiente->siguiente;
            if (lento == rapido) break;
        }
        if (rapido == nullptr || rapido->siguiente == nullptr) {
            return nullptr;   // no hay ciclo
        }

        // Fase 2: localizar el inicio
        lento = cabeza_;
        while (lento != rapido) {
            lento  = lento->siguiente;
            rapido = rapido->siguiente;
        }
        return lento;
    }

    // --------------------------------------------------------
    // Utilidades
    // --------------------------------------------------------
    int size() const { return n_; }

    bool vacia() const { return n_ == 0; }

    Nodo<T>* cabeza() const { return cabeza_; }
    Nodo<T>* cola()   const { return cola_; }

    void imprimir() const {
        if (cabeza_ == nullptr) {
            std::cout << "NULL\n";
            return;
        }
        for (Nodo<T>* a = cabeza_; a != nullptr; a = a->siguiente) {
            std::cout << a->dato;
            if (a->siguiente) std::cout << " -> ";
        }
        std::cout << " -> NULL\n";
    }
};