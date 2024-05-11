#!/bin/bash

# Comprobar si se proporciona una lista de archivos como argumento
if [ $# -eq 0 ]; then
  echo "No se ha recibido un archivo como argumento"
  exit 1
fi

# Loop a través de cada archivo de entrada y crear una copia con la fecha en el nombre del archivo
for file in "$@"; do
  # Comprobar si el archivo existe
  if [ ! -f "$file" ]; then
    echo "$file no existe"
    continue
  fi
  
  # Obtener la fecha actual en el formato YYMMDD
  date=$(date +%y%m%d)
  
  # Crear el nombre de archivo para la copia con la fecha en el formato YYMMDD_
  new_file="${date}_${file}"
  
  # Copiar el archivo original a la nueva ubicación con el nuevo nombre de archivo
  cp "$file" "$new_file"
  
  # Imprimir un mensaje de éxito
  echo "Copia creada: $new_file"
done

