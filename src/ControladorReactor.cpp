#include "ControladorReactor.hpp"

#include <iostream>

ControladorReactor::ControladorReactor() = default;

void ControladorReactor::iniciarSistema() {
    reactor.encender();
    reactor.ejecutarCiclo();
    imprimirEstado();
}

void ControladorReactor::detenerSistema() {
    reactor.apagar();
    imprimirEstado();
}

void ControladorReactor::avanzarTiempo(int horas) {
    for (int hora = 0; hora < horas; ++hora) {
        reactor.ejecutarCiclo();
        if (!monitor.evaluarSensores(reactor)) {
            monitor.activarEmergencia(reactor);
        }
    }
    imprimirEstado();
}

void ControladorReactor::procesarOrden(const std::string& orden) {
    if (orden.empty() || orden == "ciclo" || orden == "+") {
        avanzarTiempo();
    } else if (orden == "++") {
        avanzarTiempo(10);
    } else if (orden == "encender") {
        iniciarSistema();
    } else if (orden == "apagar") {
        detenerSistema();
    } else if (orden == "alimentar") {
        reactor.alimentar();
        imprimirEstado();
    } else if (orden == "mantenimiento") {
        reactor.realizarMantenimiento();
        imprimirEstado();
    } else if (orden == "estado") {
        imprimirEstado();
    } else {
        std::cout << "Orden no reconocida. Usa: encender, apagar, ciclo, +, ++, "
                     "alimentar, mantenimiento, estado, salir\n";
    }
}

void ControladorReactor::imprimirEstado() const {
    std::cout << "\n[Hora " << reactor.obtenerHoras() << "] "
              << reactor.obtenerEstado()
              << " | energia: " << reactor.obtenerEnergia()
              << " | temperatura: " << reactor.obtenerTemperatura()
              << " | radiacion: " << reactor.obtenerRadiacion()
              << " | dinero: " << reactor.obtenerDinero() << '\n';
    if (reactor.obtenerAlarma().estaActiva()) {
        std::cout << "ALARMA: " << reactor.obtenerAlarma().obtenerMensaje() << '\n';
    }
}