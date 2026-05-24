# Manual de Usuario — Jean Charles Phantom Kick

Bienvenido a **Jean Charles Phantom Kick**, un shooter arcade vertical desarrollado en C++ con el motor gráfico **SFML**, organizado modularmente mediante **CMake**. Este documento sirve como guía completa para compilar, instalar y ejecutar el juego en sistemas compatibles con Unix/Linux.

---

## Requisitos del Sistema

Para compilar y ejecutar correctamente **Jean Charles Phantom Kick**, necesitas tener instalado:

- Un compilador de C++ compatible con **C++17** (como `g++`, `clang++`)
- **CMake** versión **≥ 3.1**
- **SFML** versión 2.x (system, window, graphics, network, audio)
- Conexión a Internet (únicamente durante la descarga de recursos inicial)

### Lista de dependencias

| Dependencia | Versión mínima | Uso principal |
|-------------|----------------|----------------|
| SFML        | 2.x            | Gráficos, audio, input |
| CMake       | 3.1            | Compilación del proyecto |
| g++ / clang++ | C++17        | Compilador de C++ |
| Make        | cualquier versión reciente | Compilación a través de `make` |

---

## Instalación y ejecución

### Descargar los recursos del juego

Desde la carpeta raíz del proyecto, ejecuta `./setup.sh`

### Compilar y ejecutar el juego

Desde el subdirectorio src, corre:
`cd src`
`./run.sh`

Este script hace todo por ti:

Limpieza de builds anteriores `(rm -rf build)`

Configuración de CMake `(cmake -H. -Bbuild)`

Compilación mediante `make`

Ejecución automática del binario generado `(./build/MiJuego)`

### Estructura del proyecto
MiJuego/
├── cmake_modules/        # Módulos adicionales de CMake
├── core/                 # Núcleo del motor del juego
├── engine/               # Estados del juego, lógica principal
├── entities/             # Enemigos, jugador, balas, items
├── resources/            # Sprites, sonidos, fuentes, mapas TMX
├── src/                  # Scripts de construcción, main.cpp
│   ├── build.sh
│   ├── clean.sh
│   ├── run.sh
│   └── main.cpp
├── setup.sh              # Descarga automática de recursos
├── README.md             # Este archivo
└── CMakeLists.txt        # Configuración principal del proyecto

### Info debug
Se ha mantenido la información debug en la HUD del juego, por si fuese necesaria