#include "FuenteEnergia.hpp"

#include <algorithm>

FuenteEnergia::FuenteEnergia(double capacidad)
    : nivel(capacidad), capacidad(capacidad) {}

void FuenteEnergia::consumir(double cantidad) {
    nivel = std::max(0.0, nivel - cantidad);
}

void FuenteEnergia::recargar(double cantidad) {
    nivel = std::min(capacidad, nivel + cantidad);
}

bool FuenteEnergia::estaBaja() const {
    return nivel <= capacidad * 0.25;
}

double FuenteEnergia::obtenerNivel() const { return nivel; }
double FuenteEnergia::obtenerCapacidad() const { return capacidad; }