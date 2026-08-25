#include "Alarma.hpp"

void Alarma::emitir(const std::string& nuevoMensaje) {
    activa = true;
    mensaje = nuevoMensaje;
}

void Alarma::silenciar() {
    activa = false;
    mensaje.clear();
}

bool Alarma::estaActiva() const { return activa; }
const std::string& Alarma::obtenerMensaje() const { return mensaje; }