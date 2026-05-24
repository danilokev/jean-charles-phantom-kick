# Para ejecutar de manera más rápida, este script hace clean.sh, build.sh, make y run
rm -r -f build

mkdir build
cmake -H. -Bbuild -Wno-dev -DCMAKE_CXX_FLAGS="-w"

make -C  build

if [ $? -eq 0 ]; then
    echo "Compilación exitosa. Ejecutando MiJuego..."
    ./build/MiJuego
else
    echo "Error en la compilación. No se ejecutará el juego."
fi
