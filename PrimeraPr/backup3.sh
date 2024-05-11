#!/bin/bash

# Comprobar si se proporciona una lista de archivos como argumento
if [ $# -eq 0 ]; then
  echo "No se ha recibido un archivo como argumento"
  exit 1
fi

# Comprobar si el directorio de copias de seguridad existe, y si no, crearlo
if [ ! -d "backup" ]; then
  mkdir backup
fi

# Obtener la fecha actual en el formato YYMMDD
date=$(date +%y%m%d)

# Crear el directorio de copias de seguridad con el nombre de la fecha, si no existe
if [ ! -d "backup/$date" ]; then
  mkdir "backup/$date"
fi

# Loop a través de cada archivo de entrada y crear una copia en la carpeta de copias de seguridad
for file in "$@"; do
  # Comprobar si el archivo existe
  if [ ! -f "$file" ]; then
    echo "$file no existe"
    continue
  fi
  
  # Crear el nombre de archivo para la copia sin la fecha en el formato YYMMDD_
  new_file="${file}"
  
  # Copiar el archivo original a la nueva ubicación dentro del directorio de copias de seguridad
  cp "$file" "backup/$date/$new_file"
  
  # Imprimir un mensaje de éxito
  echo "Copia creada: backup/$date/$new_file"
done

