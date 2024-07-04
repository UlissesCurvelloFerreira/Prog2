#!/bin/bash

# Diretório onde os arquivos PNG estão armazenados (diretório atual)
directory="."

# Contador para o novo nome do arquivo
counter=1

# Contar o número de arquivos PNG no diretório atual
num_files=$(ls $directory/*.png 2>/dev/null | wc -l)

# Verificar se há arquivos PNG no diretório atual
if [ $num_files -eq 0 ]; then
  echo "Nenhum arquivo PNG encontrado no diretório atual."
  exit 1
fi

# Iterar sobre cada arquivo PNG no diretório, ordenados pelo nome
for file in $(ls $directory/*.png | sort); do
  # Novo nome do arquivo
  new_name=$(printf "%d.png" $counter)
  # Caminho completo para o arquivo antigo e novo
  src="$directory/$(basename $file)"
  dst="$directory/$new_name"
  
  # Renomear o arquivo
  mv "$src" "$dst"
  
  # Imprimir o resultado da renomeação
  echo "Renamed: $src -> $dst"
  
  # Incrementar o contador
  ((counter++))
done
