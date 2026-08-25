# MaquinaEstadoReactor

Nucleo logico de un reactor nuclear ficticio, desarrollado en C++ con el
patron de diseno State. Esta primera etapa se enfoca en el comportamiento del
modelo; la ventana grafica y los recursos multimedia se integraran despues.

## Compilar

Se necesita `g++` y `mingw32-make` de MinGW-w64:

```text
mingw32-make
```

El ejecutable generado es `reactor.exe`. Para limpiar los archivos de
compilacion:

```text
mingw32-make clean
```

## Probar la logica

```text
reactor.exe
```

Comandos disponibles:

- `encender`: pasa de `APAGADO` a `INICIALIZANDO` y luego a `OPERATIVO`.
- `Enter`, `ciclo` o `+`: avanza una hora del reactor.
- `++`: avanza diez horas para acelerar la simulacion.
- `alimentar`: agrega energia simulada y aumenta el riesgo de sobrecarga.
- `mantenimiento`: repara el reactor cuando esta en mantenimiento.
- `apagar`: apaga el reactor.
- `estado`: muestra sus parametros actuales.

Cada ciclo representa una hora. El modelo consume energia, aumenta la
temperatura, genera ingresos simulados y activa protecciones cuando detecta
condiciones peligrosas. El combustible mencionado en el proyecto es solo una
variable ficticia: no representa una operacion nuclear real.

## Estructura

- `include/`: interfaces y clases del modelo orientado a objetos.
- `src/`: implementacion del reactor, estados, sensores y controlador.
- `docs/`: diagramas PlantUML.
- `assets/`: espacio para fuentes, sonidos, imagenes y videos futuros.# MaquinaEstadoReactor

Nucleo logico de un reactor nuclear ficticio, desarrollado en C++ con el
patron de diseno State. Esta primera etapa se enfoca en el comportamiento del
modelo; la ventana grafica y los recursos multimedia se integraran despues.

## Compilar

Se necesita `g++` y `mingw32-make` de MinGW-w64:

```text
mingw32-make
```

El ejecutable generado es `reactor.exe`. Para limpiar los archivos de
compilacion:

```text
mingw32-make clean
```

## Probar la logica

```text
reactor.exe
```

Comandos disponibles:

- `encender`: pasa de `APAGADO` a `INICIALIZANDO` y luego a `OPERATIVO`.
- `Enter`, `ciclo` o `+`: avanza una hora del reactor.
- `++`: avanza diez horas para acelerar la simulacion.
- `alimentar`: agrega energia simulada y aumenta el riesgo de sobrecarga.
- `mantenimiento`: repara el reactor cuando esta en mantenimiento.
- `apagar`: apaga el reactor.
- `estado`: muestra sus parametros actuales.

Cada ciclo representa una hora. El modelo consume energia, aumenta la
temperatura, genera ingresos simulados y activa protecciones cuando detecta
condiciones peligrosas. El combustible mencionado en el proyecto es solo una
variable ficticia: no representa una operacion nuclear real.

## Estructura

- `include/`: interfaces y clases del modelo orientado a objetos.
- `src/`: implementacion del reactor, estados, sensores y controlador.
- `docs/`: diagramas PlantUML.
- `assets/`: espacio para fuentes, sonidos, imagenes y videos futuros.

