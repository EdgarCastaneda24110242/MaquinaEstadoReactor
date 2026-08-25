#pragma once

class SistemaEnfriamiento {
public:
    void activar();
    void desactivar();
    void disiparCalor(double& temperatura) const;
    bool estaActivo() const;

private:
    bool activo = false;
};