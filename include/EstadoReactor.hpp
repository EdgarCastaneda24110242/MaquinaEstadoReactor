#pragma once

#include <memory>

class Reactor;

class EstadoReactor {
public:
    virtual ~EstadoReactor() = default;
    virtual void entrar(Reactor& reactor) = 0;
    virtual void ejecutar(Reactor& reactor) = 0;
    virtual void salir(Reactor& reactor) = 0;
    virtual const char* nombre() const = 0;
};

class EstadoApagado final : public EstadoReactor {
public:
    void entrar(Reactor& reactor) override;
    void ejecutar(Reactor& reactor) override;
    void salir(Reactor& reactor) override;
    const char* nombre() const override;
};

class EstadoInicializando final : public EstadoReactor {
public:
    void entrar(Reactor& reactor) override;
    void ejecutar(Reactor& reactor) override;
    void salir(Reactor& reactor) override;
    const char* nombre() const override;
};

class EstadoOperativo final : public EstadoReactor {
public:
    void entrar(Reactor& reactor) override;
    void ejecutar(Reactor& reactor) override;
    void salir(Reactor& reactor) override;
    const char* nombre() const override;
};

class EstadoBajaEnergia final : public EstadoReactor {
public:
    void entrar(Reactor& reactor) override;
    void ejecutar(Reactor& reactor) override;
    void salir(Reactor& reactor) override;
    const char* nombre() const override;
};

class EstadoEmergencia final : public EstadoReactor {
public:
    void entrar(Reactor& reactor) override;
    void ejecutar(Reactor& reactor) override;
    void salir(Reactor& reactor) override;
    const char* nombre() const override;
};

class EstadoScram final : public EstadoReactor {
public:
    void entrar(Reactor& reactor) override;
    void ejecutar(Reactor& reactor) override;
    void salir(Reactor& reactor) override;
    const char* nombre() const override;
};

class EstadoEnfriamiento final : public EstadoReactor {
public:
    void entrar(Reactor& reactor) override;
    void ejecutar(Reactor& reactor) override;
    void salir(Reactor& reactor) override;
    const char* nombre() const override;
};

class EstadoMantenimiento final : public EstadoReactor {
public:
    void entrar(Reactor& reactor) override;
    void ejecutar(Reactor& reactor) override;
    void salir(Reactor& reactor) override;
    const char* nombre() const override;
};