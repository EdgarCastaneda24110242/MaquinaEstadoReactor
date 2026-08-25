#include "Reactor.hpp"

#include <algorithm>
#include <iomanip>
#include <random>
#include <sstream>

namespace {
std::string formatear(double valor) {
    std::ostringstream salida;
    salida << std::fixed << std::setprecision(1) << valor;
    return salida.str();
}
}

Reactor::Reactor()
    : estadoActual(std::make_unique<EstadoApagado>()) {
    estadoActual->entrar(*this);
    memoriaEventos.registrar("Reactor creado en estado APAGADO");
}

void Reactor::encender() {
    if (obtenerEstado() == "APAGADO") {
        cambiarEstado(std::make_unique<EstadoInicializando>());
    }
}

void Reactor::apagar() {
    if (obtenerEstado() != "APAGADO") {
        cambiarEstado(std::make_unique<EstadoApagado>());
    }
}

void Reactor::ejecutarCiclo() {
    estadoActual->ejecutar(*this);
    sensorTemperatura.actualizar(temperatura);
    sensorRadiacion.actualizar(radiacion);
    sensorEnergia.actualizar(fuenteEnergia.obtenerNivel());
}

void Reactor::cambiarEstado(std::unique_ptr<EstadoReactor> nuevoEstado) {
    estadoActual->salir(*this);
    estadoActual = std::move(nuevoEstado);
    estadoActual->entrar(*this);
}

void Reactor::alimentar(double cantidad) {
    if (obtenerEstado() == "APAGADO" || obtenerEstado() == "MANTENIMIENTO") {
        memoriaEventos.registrar("Alimentacion ignorada: reactor no operativo");
        return;
    }

    fuenteEnergia.recargar(cantidad);
    aumentarTemperatura(cantidad * 0.25);
    memoriaEventos.registrar("Se agrego energia simulada: " + formatear(cantidad));

    const double exceso = std::max(0.0, fuenteEnergia.obtenerNivel() - 75.0);
    const double probabilidad = std::min(0.85, exceso / 100.0);
    std::uniform_real_distribution<double> azar(0.0, 1.0);
    static std::mt19937 generador(std::random_device{}());
    if (probabilidad > 0.0 && azar(generador) < probabilidad) {
        activarEmergencia("Sobrecarga por alimentar demasiado al reactor");
    }
}

void Reactor::realizarMantenimiento() {
    if (obtenerEstado() == "MANTENIMIENTO") {
        temperatura = 35.0;
        radiacion = 0.0;
        horasSinMantenimiento = 0;
        alarma.silenciar();
        memoriaEventos.registrar("Mantenimiento completado");
        cambiarEstado(std::make_unique<EstadoInicializando>());
    }
}

void Reactor::activarEmergencia(const std::string& motivo) {
    alarma.emitir(motivo);
    memoriaEventos.registrar("EMERGENCIA: " + motivo);
    if (obtenerEstado() != "EMERGENCIA" && obtenerEstado() != "SCRAM") {
        cambiarEstado(std::make_unique<EstadoEmergencia>());
    }
}

const std::string Reactor::obtenerEstado() const { return estadoActual->nombre(); }
double Reactor::obtenerEnergia() const { return fuenteEnergia.obtenerNivel(); }
double Reactor::obtenerTemperatura() const { return temperatura; }
double Reactor::obtenerRadiacion() const { return radiacion; }
double Reactor::obtenerDinero() const { return dinero; }
int Reactor::obtenerHoras() const { return horasTranscurridas; }
const MemoriaEventos& Reactor::obtenerMemoria() const { return memoriaEventos; }
const Alarma& Reactor::obtenerAlarma() const { return alarma; }
bool Reactor::necesitaMantenimiento() const { return horasSinMantenimiento >= 4; }

void Reactor::consumirEnergia(double cantidad) { fuenteEnergia.consumir(cantidad); }
void Reactor::aumentarTemperatura(double cantidad) { temperatura += cantidad; }
void Reactor::reducirTemperatura(double cantidad) {
    temperatura = std::max(20.0, temperatura - cantidad);
}
void Reactor::registrarEvento(const std::string& evento) { memoriaEventos.registrar(evento); }
void Reactor::generarIngresos() {
    dinero += 5.0 + fuenteEnergia.obtenerNivel() * 0.1;
}
void Reactor::avanzarHora() {
    ++horasTranscurridas;
    ++horasSinMantenimiento;
}
bool Reactor::autodiagnosticoCorrecto() const {
    return temperatura < 60.0 && radiacion < 30.0;
}
bool Reactor::riesgoCritico() const {
    return temperatura >= 120.0 || radiacion >= 100.0;
}
bool Reactor::parametrosEstables() const {
    return temperatura < 90.0 && radiacion < 80.0;
}