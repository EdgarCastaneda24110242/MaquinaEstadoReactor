#include "Sensor.hpp"

Sensor::Sensor(std::string nombre, double minimo, double maximo)
    : nombre(std::move(nombre)), valor(0.0), minimo(minimo), maximo(maximo) {}

double Sensor::leer() const { return valor; }
bool Sensor::estaDentroDeRango() const { return valor >= minimo && valor <= maximo; }
const std::string& Sensor::obtenerNombre() const { return nombre; }
void Sensor::actualizar(double nuevoValor) { valor = nuevoValor; }

SensorTemperatura::SensorTemperatura()
    : Sensor("temperatura", 0.0, 100.0) {}

SensorRadiacion::SensorRadiacion()
    : Sensor("radiacion", 0.0, 80.0) {}

SensorEnergia::SensorEnergia()
    : Sensor("energia", 0.0, 100.0) {}