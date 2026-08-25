#include "MemoriaEventos.hpp"

void MemoriaEventos::registrar(const std::string& evento) {
    eventos.push_back(evento);
}

const std::vector<std::string>& MemoriaEventos::consultar() const {
    return eventos;
}