#pragma once

#include "Alarma.hpp"
#include "EstadoReactor.hpp"
#include "FuenteEnergia.hpp"
#include "MemoriaEventos.hpp"
#include "Sensor.hpp"
#include "SistemaEnfriamiento.hpp"

#include <memory>
#include <string>

class Reactor {
public:
    Reactor();

    void encender();
    void apagar();
    void ejecutarCiclo();
    void cambiarEstado(std::unique_ptr<EstadoReactor> nuevoEstado);
    void alimentar(double cantidad = 20.0);
    void realizarMantenimiento();
    void activarEmergencia(const std::string& motivo);

    const std::string obtenerEstado() const;
    double obtenerEnergia() const;
    double obtenerTemperatura() const;
    double obtenerRadiacion() const;
    double obtenerDinero() const;
    int obtenerHoras() const;
    const MemoriaEventos& obtenerMemoria() const;
    const Alarma& obtenerAlarma() const;
    bool necesitaMantenimiento() const;

    void consumirEnergia(double cantidad);
    void aumentarTemperatura(double cantidad);
    void reducirTemperatura(double cantidad);
    void registrarEvento(const std::string& evento);
    void generarIngresos();
    void avanzarHora();
    bool autodiagnosticoCorrecto() const;
    bool riesgoCritico() const;
    bool parametrosEstables() const;

private:
    std::unique_ptr<EstadoReactor> estadoActual;
    double temperatura = 20.0;
    double radiacion = 0.0;
    double dinero = 0.0;
    int horasTranscurridas = 0;
    int horasSinMantenimiento = 0;
    FuenteEnergia fuenteEnergia;
    SistemaEnfriamiento sistemaEnfriamiento;
    MemoriaEventos memoriaEventos;
    Alarma alarma;
    SensorTemperatura sensorTemperatura;
    SensorRadiacion sensorRadiacion;
    SensorEnergia sensorEnergia;
};