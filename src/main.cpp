#include "ControladorReactor.hpp"

#include <iostream>
#include <string>

int main() {
    ControladorReactor controlador;
    std::cout << "=== Reactor Ficticio: nucleo logico ===\n"
              << "Estado inicial: APAGADO\n"
              << "Enter/ciclo: avanza una hora | +: una hora | ++: diez horas\n"
              << "Comandos: encender, apagar, alimentar, mantenimiento, estado, salir\n";
    controlador.procesarOrden("estado");

    std::string orden;
    while (true) {
        std::cout << "\n> ";
        if (!std::getline(std::cin, orden) || orden == "salir") {
            break;
        }
        controlador.procesarOrden(orden);
    }
    return 0;
}