#pragma once

#include <string>

class Sensor {
public:
    explicit Sensor(std::string nombre, double minimo, double maximo);
    virtual ~Sensor() = default;

    virtual double leer() const;
    bool estaDentroDeRango() const;
    const std::string& obtenerNombre() const;
    void actualizar(double valor);

protected:
    std::string nombre;
    double valor;
    double minimo;
    double maximo;
};

class SensorTemperatura final : public Sensor {
public:
    SensorTemperatura();
};

class SensorRadiacion final : public Sensor {
public:
    SensorRadiacion();
};

class SensorEnergia final : public Sensor {
public:
    SensorEnergia();
};