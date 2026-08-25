#pragma once

#include <string>

class Alarma {
public:
    void emitir(const std::string& mensaje);
    void silenciar();
    bool estaActiva() const;
    const std::string& obtenerMensaje() const;

private:
    bool activa = false;
    std::string mensaje;
};