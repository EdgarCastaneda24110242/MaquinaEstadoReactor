#pragma once

#include "MonitorSeguridad.hpp"
#include "Reactor.hpp"

#include <string>

class ControladorReactor {
public:
    ControladorReactor();

    void iniciarSistema();
    void detenerSistema();
    void procesarOrden(const std::string& orden);
    void avanzarTiempo(int horas = 1);
    void imprimirEstado() const;

private:
    Reactor reactor;
    MonitorSeguridad monitor;
};