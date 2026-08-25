#pragma once

class FuenteEnergia {
public:
    explicit FuenteEnergia(double capacidad = 100.0);

    void consumir(double cantidad);
    void recargar(double cantidad);
    bool estaBaja() const;
    double obtenerNivel() const;
    double obtenerCapacidad() const;

private:
    double nivel;
    double capacidad;
};