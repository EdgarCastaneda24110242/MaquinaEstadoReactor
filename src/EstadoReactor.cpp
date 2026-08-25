#include "EstadoReactor.hpp"

#include "Reactor.hpp"

void EstadoApagado::entrar(Reactor& reactor) { reactor.reducirTemperatura(0.0); }
void EstadoApagado::ejecutar(Reactor&) {}
void EstadoApagado::salir(Reactor&) {}
const char* EstadoApagado::nombre() const { return "APAGADO"; }

void EstadoInicializando::entrar(Reactor& reactor) {
    reactor.avanzarHora();
}
void EstadoInicializando::ejecutar(Reactor& reactor) {
    if (reactor.autodiagnosticoCorrecto()) {
        reactor.cambiarEstado(std::make_unique<EstadoOperativo>());
    } else {
        reactor.activarEmergencia("Fallo en el autodiagnostico");
    }
}
void EstadoInicializando::salir(Reactor&) {}
const char* EstadoInicializando::nombre() const { return "INICIALIZANDO"; }

void EstadoOperativo::entrar(Reactor&) {}
void EstadoOperativo::ejecutar(Reactor& reactor) {
    reactor.avanzarHora();
    reactor.consumirEnergia(4.0);
    reactor.aumentarTemperatura(8.0);
    reactor.generarIngresos();
    if (reactor.obtenerTemperatura() >= 100.0) {
        reactor.activarEmergencia("Temperatura alta");
    } else if (reactor.obtenerEnergia() <= 25.0) {
        reactor.cambiarEstado(std::make_unique<EstadoBajaEnergia>());
    } else if (reactor.necesitaMantenimiento()) {
        reactor.cambiarEstado(std::make_unique<EstadoMantenimiento>());
    }
}
void EstadoOperativo::salir(Reactor&) {}
const char* EstadoOperativo::nombre() const { return "OPERATIVO"; }

void EstadoBajaEnergia::entrar(Reactor&) {}
void EstadoBajaEnergia::ejecutar(Reactor& reactor) {
    reactor.avanzarHora();
    reactor.consumirEnergia(1.0);
    reactor.aumentarTemperatura(2.0);
    if (reactor.obtenerTemperatura() >= 100.0) {
        reactor.activarEmergencia("La temperatura sigue subiendo con poca energia");
    } else if (reactor.obtenerEnergia() <= 0.0) {
        reactor.cambiarEstado(std::make_unique<EstadoMantenimiento>());
    } else if (reactor.obtenerEnergia() > 40.0) {
        reactor.cambiarEstado(std::make_unique<EstadoOperativo>());
    }
}
void EstadoBajaEnergia::salir(Reactor&) {}
const char* EstadoBajaEnergia::nombre() const { return "BAJA_ENERGIA"; }

void EstadoEmergencia::entrar(Reactor&) {}
void EstadoEmergencia::ejecutar(Reactor& reactor) {
    reactor.avanzarHora();
    reactor.aumentarTemperatura(5.0);
    if (reactor.riesgoCritico()) {
        reactor.cambiarEstado(std::make_unique<EstadoScram>());
    } else if (reactor.parametrosEstables()) {
        reactor.cambiarEstado(std::make_unique<EstadoOperativo>());
    }
}
void EstadoEmergencia::salir(Reactor&) {}
const char* EstadoEmergencia::nombre() const { return "EMERGENCIA"; }

void EstadoScram::entrar(Reactor& reactor) { reactor.registrarEvento("SCRAM activado"); }
void EstadoScram::ejecutar(Reactor& reactor) {
    reactor.cambiarEstado(std::make_unique<EstadoEnfriamiento>());
}
void EstadoScram::salir(Reactor&) {}
const char* EstadoScram::nombre() const { return "SCRAM"; }

void EstadoEnfriamiento::entrar(Reactor& reactor) { reactor.reducirTemperatura(5.0); }
void EstadoEnfriamiento::ejecutar(Reactor& reactor) {
    reactor.reducirTemperatura(18.0);
    if (reactor.obtenerTemperatura() <= 45.0) {
        reactor.cambiarEstado(std::make_unique<EstadoMantenimiento>());
    }
}
void EstadoEnfriamiento::salir(Reactor&) {}
const char* EstadoEnfriamiento::nombre() const { return "ENFRIAMIENTO"; }

void EstadoMantenimiento::entrar(Reactor& reactor) { reactor.avanzarHora(); }
void EstadoMantenimiento::ejecutar(Reactor& reactor) {
    reactor.avanzarHora();
    if (reactor.necesitaMantenimiento()) {
        reactor.apagar();
    }
}
void EstadoMantenimiento::salir(Reactor&) {}
const char* EstadoMantenimiento::nombre() const { return "MANTENIMIENTO"; }