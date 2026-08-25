#include "SistemaEnfriamiento.hpp"

#include <algorithm>

void SistemaEnfriamiento::activar() { activo = true; }
void SistemaEnfriamiento::desactivar() { activo = false; }

void SistemaEnfriamiento::disiparCalor(double& temperatura) const {
    if (activo) {
        temperatura = std::max(20.0, temperatura - 18.0);
    }
}

bool SistemaEnfriamiento::estaActivo() const { return activo; }