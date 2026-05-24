#!/bin/bash

BASE_URL="https://github.com/danilokev/jean-charles-phantom-kick/releases/download/v1.0.0-assets"
RESOURCES_DIR="src/resources"

if [ -d "$RESOURCES_DIR" ]; then
  echo "Eliminando contenido anterior de la carpeta $RESOURCES_DIR..."
  rm -rf "$RESOURCES_DIR"/*
fi

mkdir -p "$RESOURCES_DIR"

echo "Descargando recursos desde GitHub Releases..."
wget "$BASE_URL/resources-part1.zip" -O resources-part1.zip
wget "$BASE_URL/resources-part2.zip" -O resources-part2.zip

if [ -f resources-part1.zip ] && [ -f resources-part2.zip ]; then
  echo "Descarga completa. Extrayendo recursos..."
  unzip -o resources-part1.zip -d "$RESOURCES_DIR"
  unzip -o resources-part2.zip -d "$RESOURCES_DIR"
  echo "Recursos extraidos exitosamente en $RESOURCES_DIR"
  rm -f resources-part1.zip resources-part2.zip
else
  echo "Error al descargar los recursos"
fi