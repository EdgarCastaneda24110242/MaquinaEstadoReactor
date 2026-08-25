#pragma once

#include "Alarma.hpp"
#include "Sensor.hpp"

class Reactor;

class MonitorSeguridad {
public:
    MonitorSeguridad();

    bool evaluarSensores(const Reactor& reactor);
    void activarEmergencia(Reactor& reactor);

private:
    double limiteTemperatura;
    double limiteRadiacion;
    SensorTemperatura sensorTemperatura;
    SensorRadiacion sensorRadiacion;
    Alarma alarma;
};