# Jean Charles Phantom Kick

_Descubre las trepidantes aventuras de Jean Charles en su nuevo título: Phantom Kick. Este shoot 'em up de ritmo frenético convierte el esquivar proyectiles en un auténtico desafío. Con controles sencillos pero de alta curva de aprendizaje, te enfrentarás a hordas de enemigos que desplegarán espectaculares cortinas de proyectiles. Sumérgete en una historia dinámica, narrada mediante diálogos vibrantes entre los personajes, mientras perfeccionas tus reflejos partida tras partida. Proyecto académico desarrollado para la asignatura Fundamentos de los Videojuegos del Grado en Ingeniería Multimedia._

## Comenzando

_Estas instrucciones te permitiran obtener una copia del proyecto en funcionamiento en tu maquina local para propositos de desarrollo y pruebas._

### Pre-requisitos

_Debe tener instalado **CMake** (version 3.1 o superior) y un compilador compatible con C++17. Ejemplo para Ubuntu:_

```sh
sudo apt update
sudo apt install cmake g++ build-essential
```

_El juego requiere recursos graficos y de audio que se descargan mediante el script `setup.sh`:_

```sh
./setup.sh
```

### Instalacion

_En primer lugar, debemos clonar el proyecto desde nuestro repositorio._

```sh
git clone https://github.com/danilokev/jean-charles-phantom-kick.git
```

_Una vez clonado, compilamos el proyecto con CMake:_

```sh
cd NOMBRE_REPO
mkdir -p build
cd build
cmake ../src
make
```

_El ejecutable `MiJuego` se copiara automaticamente a la raiz del proyecto._

_Para lanzar el juego:_

```sh
./MiJuego
```

## Construido con

- [CMake](https://cmake.org/) - Herramienta de construccion multiplataforma.
- [C++17](https://en.cppreference.com/w/cpp/17) - Estandar de lenguaje C++.

## Autores

- **Kevin D. Analuisa Ortiz** - [danilokev](https://github.com/danilokev)
- **Alfonso Lopez Laforet** - [AlfonsoLafo](https://github.com/AlfonsoLafo)
- **Mario Gimenez Lopez-Torres** - [mgl126](https://github.com/mgl126)
- **Marco Lopez Mira** - [MarcosLopezMira](https://github.com/MarcosLopezMira)

## Licencia

Este proyecto esta bajo la Licencia MIT - mira el archivo [LICENSE](LICENSE) para detalles.
