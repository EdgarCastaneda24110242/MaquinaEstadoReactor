#include "MonitorSeguridad.hpp"

#include "Reactor.hpp"

MonitorSeguridad::MonitorSeguridad()
    : limiteTemperatura(100.0), limiteRadiacion(80.0) {}

bool MonitorSeguridad::evaluarSensores(const Reactor& reactor) {
    sensorTemperatura.actualizar(reactor.obtenerTemperatura());
    sensorRadiacion.actualizar(reactor.obtenerRadiacion());
    return sensorTemperatura.estaDentroDeRango() && sensorRadiacion.estaDentroDeRango();
}

void MonitorSeguridad::activarEmergencia(Reactor& reactor) {
    if (reactor.obtenerTemperatura() >= limiteTemperatura) {
        alarma.emitir("Temperatura fuera de rango");
        reactor.activarEmergencia(alarma.obtenerMensaje());
    } else if (reactor.obtenerRadiacion() >= limiteRadiacion) {
        alarma.emitir("Radiacion fuera de rango");
        reactor.activarEmergencia(alarma.obtenerMensaje());
    }
}