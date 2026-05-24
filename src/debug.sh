rm -r -f build

mkdir build
cmake -H. -Bbuild

make -C build

if [ $? -eq 0 ]; then
    echo "Compilación exitosa. Ejecutando MiJuego con gdb..."

    # Ejecutar bajo GDB
    gdb -ex run --args ./build/MiJuego

else
    echo "Error en la compilación. No se ejecutará el juego."
fi

# Al recibir
# ---
# Program received signal SIGSEGV, Segmentation fault.
# 0x0000000000451010 in ?? ()
# ---

# Escribir el comando
# bt

# Para salir, Ctrl + Z