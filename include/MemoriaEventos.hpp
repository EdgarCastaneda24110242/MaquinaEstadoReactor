#pragma once

#include <string>
#include <vector>

class MemoriaEventos {
public:
    void registrar(const std::string& evento);
    const std::vector<std::string>& consultar() const;

private:
    std::vector<std::string> eventos;
};